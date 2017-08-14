#include "StdAfx.h"
#include "AntConnection.h"
#include "TCPServer.h"
//#include "MeetingErrorDef.h"

CAntConnection::CAntConnection(void)
{
	m_IsReaping = false;
	m_IsDoClosing = false;
	m_Verifying = false;
	m_Verified = false;
	m_socket		= INVALID_SOCKET;
	m_nValidDataLen = -1;
	m_MeetingID = 0;
	m_nState		= CONNECTION_STATE_NONE;
	m_nIOPending	= 0;
	m_dwKeepAlive	= 0;
	m_CloseRetryCount = 0;

	memset( m_addr, 0, 32 );
}


CAntConnection::~CAntConnection(void)
{
	while(m_pServer && m_listSendData.size()){
		CIOCPBuffer* pBuffer = m_listSendData.front();
		m_listSendData.pop_front();
		m_pServer->ReleaseIOCPBuffer(pBuffer);
	}
}

void CAntConnection::SetSocket(SOCKET socket)
{
	m_socket = socket;
	m_nState = CONNECTION_STATE_CONNECTED;

	//SOCKADDR	sa = {0};
	//int			salen = sizeof(sa);
	//getpeername( m_socket, &sa, &salen );

	//sockaddr_in		*sai = (sockaddr_in *)&sa;
	//sprintf( m_addr, "%s:%d",  
	//	inet_ntoa( sai->sin_addr ), 
	//	ntohs(sai->sin_port) );

	sockaddr_in		sa;
	int				salen = sizeof(sa);
	getpeername( m_socket, (SOCKADDR *)&sa, &salen );
	sprintf( m_addr, "%s:%d", inet_ntoa( sa.sin_addr ), ntohs(sa.sin_port) );

	printf( "NewConnection! %s\r\n", m_addr );
}

void CAntConnection::SetTCPServer( CTCPServer *pServer )
{
	m_pServer = pServer;
}

void CAntConnection::SetSessionID( UINT sessionID )
{
	m_SessionID = sessionID;
}

UINT CAntConnection::GetSessionID()
{
	return m_SessionID;
}

CTCPServer* CAntConnection::GetServer()
{
	return m_pServer;
}


// ��ʼ��Socket�ж�ȡ����
BOOL CAntConnection::ReqRecvData(CIOCPBuffer *pBuffer)
{
	BOOL result = FALSE;
	CLockScope lock(m_LockSocketStatus);	
	if (m_nState == CONNECTION_STATE_CONNECTED)
	{	
		if( pBuffer == NULL )
			pBuffer = m_pServer->GetIOCPBuffer();

		if( pBuffer)
		{
			pBuffer->Init();
			pBuffer->SetUsed(0);
			pBuffer->SetOperation( IOCP_OPERATION_RECV );

			DWORD		dwFlags = 0;
			DWORD		dwRecvd = 0;

			IncreaseIOPending();

			if( ::WSARecv( m_socket, pBuffer->GetWSABuf(), 1, &dwRecvd, &dwFlags, (WSAOVERLAPPED*)pBuffer, NULL ) == SOCKET_ERROR )
			{
				int		nLastError = WSAGetLastError();
				result = nLastError == WSA_IO_PENDING;
				if( !result )
				{
					ATLTRACE( _T("ReadData Error:%d\r\n"), nLastError);
					// ����IOBuffer���ر�Connection
					DecreaseIOPending();
					m_pServer->ReleaseIOCPBuffer(pBuffer);
					PrepareClose();
				}
			}
		}
	}
	return TRUE;
}

BOOL CAntConnection::ReqSendData(const char *pData, int nSize)
{
	BOOL result = FALSE;
	CLockScope lock(m_LockSocketStatus);	
	if(m_nState == CONNECTION_STATE_CONNECTED &&  pData && nSize > 0 )
	{	
		CIOCPBuffer *pBuffer = m_pServer->GetIOCPBuffer();

		pBuffer->Init();
		pBuffer->SetUsed(0);
		pBuffer->SetData(pData, nSize);
		pBuffer->SetOperation( IOCP_OPERATION_SEND );

		if( m_listSendData.size() == 0 ){
			result = ReqSendBuffer(pBuffer);
		}
		else{
			m_listSendData.push_back(pBuffer);
			result = TRUE;
		}
	}
	return TRUE;
}

// ������Buffer
BOOL CAntConnection::ReqSendBuffer(CIOCPBuffer *pBuffer)
{
	BOOL result = FALSE;
	CLockScope lock(m_LockSocketStatus);	
	if( pBuffer && m_nState == CONNECTION_STATE_CONNECTED )
	{	
		DWORD		dwFlags = 0;
		DWORD		dwSent = 0;

		IncreaseIOPending();

		if( ::WSASend( m_socket, pBuffer->GetWSABuf(), 1, &dwSent, dwFlags, (WSAOVERLAPPED*)pBuffer, NULL ) == SOCKET_ERROR )
		{
			int		nLastError = WSAGetLastError();
			result = nLastError == WSA_IO_PENDING;
			if( !result )
			{
				ATLTRACE( _T("SendData Error:%d\r\n"), nLastError);
				// ����IOBuffer���ر�Connection
				m_pServer->ReleaseIOCPBuffer(pBuffer);
				DecreaseIOPending();
				
				PrepareClose();
			}
		}
	}
	return result;
}

// ���������б��е���һ������
BOOL CAntConnection::OnSendCompleted()
{
	BOOL result = TRUE;

	CLockScope lock(m_LockSocketStatus);	
	
	DecreaseIOPending();

	if (m_nState == CONNECTION_STATE_CONNECTED)
	{	
		CIOCPBuffer		*pBuffer = NULL;

		if( !m_listSendData.empty() )
		{
			list<CIOCPBuffer*>::const_iterator begin = m_listSendData.begin();

			pBuffer = *begin;
			m_listSendData.pop_front();
		}

		if( pBuffer != NULL )
		{
			result = ReqSendBuffer(pBuffer);
		}

	}

	return result;
}

// ���յ�����
// ���ݳ����Ϊ 4096=4K
BOOL CAntConnection::OnReceivedCompleted(CIOCPBuffer *pBuffer)
{
	BOOL result = FALSE;

	CLockScope lock(m_LockSocketStatus);	

	DecreaseIOPending();

	if( pBuffer)
	{
		bool	needCloseConnection = false;
		UINT	uLen = pBuffer->GetUsedLen(); 

		// RecvData==0��Ҫ�ر�����
		if( uLen == 0 )
		{
			m_pServer->ReleaseIOCPBuffer(pBuffer);
			needCloseConnection = true;
		}
		else
		{
			// ������4���ֽڵĳ��ȣ�Ȼ������
			m_DataCache.Append((char*)pBuffer->GetData(), uLen);

			if( ParseData() == 0)
			{
				m_pServer->ReleaseIOCPBuffer(pBuffer);
				needCloseConnection = true;
			}
			else
			{  
				printf( "[%d]", uLen );

				// ����������
				result = ReqRecvData(pBuffer);
			}
		}

		if (needCloseConnection){
			PrepareClose();
		}
	}
	else{
		ATLASSERT("OnReceivedCompleted buffer is null");
	}
	
	return result;
}

// ��������
// -1 ��ʾ��û���յ�����������
// 1 ��ʾ�ɹ�
// 0 ��ʾ����ʧ�ܣ���Ҫ�Ͽ���������
int CAntConnection::ParseData()
{
	if( m_DataCache.GetSize() < 4 )
		return -1;

	if( m_nValidDataLen == -1 )
	{
		memcpy(&m_nValidDataLen, m_DataCache.GetData(), 4 );
	//	m_nValidDataLen = ::ntohl(m_nValidDataLen); 
		// ��������쳣 ��Ͽ�����
		if(m_nValidDataLen >= 8192 || m_nValidDataLen <= 0 )
			return 0;
	}

	// ˵���Ѿ�ȡ�������������ݰ�
	if( m_nValidDataLen != -1 && m_DataCache.GetSize() >= (m_nValidDataLen + 4) )
	{
		CAntPackagePtr	pPackage( new CAntPackage);
		
		//if( m_AntParser.ParseData( (char*)m_DataCache.GetData() + 4, m_nValidDataLen, pPackage.get() ) )
		if( CAntPackage::ParseData( (char*)m_DataCache.GetData() + 4, m_nValidDataLen, pPackage.get() ) )
		{
			if( !ProcessPackage( pPackage, (char*)m_DataCache.GetData(), m_nValidDataLen + 4 ) )
				return 0;
		}

		m_DataCache.MoveData(0, m_nValidDataLen + 4, m_DataCache.GetSize() - m_nValidDataLen - 4 );
		m_nValidDataLen = -1;
		
		//m_pServer->ReleasePackage(pPackage);
	}
	else
		return -1;
	
	// ����cache�е�����
	return ParseData();
}

// ����package
// pchData Ϊδ�����ԭʼ���ݰ���ǰ4���ֽڵ����ݳ���
BOOL CAntConnection::ProcessPackage(CAntPackagePtr& pPackage, char *pchData, int nSize)
{
	BOOL result = TRUE;
	m_pServer->OnProcessPackage(this, pPackage);
	return result;
}

void CAntConnection::PrepareClose()
{
	CLockScope lock(m_LockSocketStatus);	
	if (!m_IsDoClosing)
	{
		m_IsDoClosing = true;
		m_nState = CONNECTION_STATE_CLOSING;

		/*
		To assure that all data is sent and received on a connected socket before it is closed,
		an application should use shutdown to close connection before calling closesocket. For example, to initiate a graceful disconnect:

		Call WSAAsyncSelect to register for FD_CLOSE notification. 
		Call shutdown with how=SD_SEND. 
		When FD_CLOSE received, call recv until zero returned, or SOCKET_ERROR. 
		Call closesocket. 
		Note  The shutdown function does not block regardless of the SO_LINGER setting on the socket.

		An application should not rely on being able to reuse a socket after it has been shut down. In particular, 
		a Windows Sockets provider is not required to support the use of connect on a socket that has been shut down.
		*/
		printf( "Shutdown! %s\r\n", m_addr );


		::shutdown(m_socket, SD_BOTH);
		::Sleep(100); //so_linger

		CAntPackagePtr	pPackage(m_pServer->GetPackage());
		pPackage->SetMethod("disconnection", 13);
		m_pServer->OnProcessPackage(this, pPackage);
		
		/*		
		if (!m_IsReaping){
			m_IsReaping = true;
			m_pServer->AddDelayReapConnection(this);
		}	
		*/	
	}
}

// ����
// 0��ʾ���ӱ��ر�
// -1��ʾû�������رգ��������п���IO������û����ɡ�
// 
int CAntConnection::DoClose()
{	
 	int result = -1;
	CLockScope lock(m_LockSocketStatus);
	if (m_IsDoClosing)
	{						
		if( ::InterlockedCompareExchange(&m_nIOPending, 0, 0 ) == 0 )
		{
			//must wait for all pending io are canceled or completed, then we can release the connection object
			if (m_nState != CONNECTION_STATE_CLOSED)
			{
				::closesocket(m_socket);
				m_socket = INVALID_SOCKET;
				m_nState = CONNECTION_STATE_CLOSED;
			}
			result = 0;
		}
		else
		{
			if (++m_CloseRetryCount == 3)
			{
				//we need to cancel all pending io , so we force close
				::closesocket(m_socket);
				m_socket = INVALID_SOCKET;
				m_nState = CONNECTION_STATE_CLOSED;
			}
		}	
	}
	else{
		PrepareClose();
	}
	return result;
}

//
LONG CAntConnection::IncreaseIOPending()
{
	return ::InterlockedIncrement(&m_nIOPending);
}

LONG CAntConnection::DecreaseIOPending()
{
	return ::InterlockedDecrement(&m_nIOPending);
}


//void CAntConnection::SetUser( Meeting::CUserItem& user )
//{
//	CLockScope lock(m_LockSocketStatus);
//	if (m_Verified)
//	{
//		m_UserItem = user;
//	}
//}

//UINT CAntConnection::GetUserID()
//{
//	CLockScope lock(m_LockSocketStatus);
//	UINT id = 0;
//	if (m_Verified)
//	{
//		id = m_UserItem.GetID();;
//	}
//	return id;
//}

void CAntConnection::SetVerified( bool verifyed )
{
	CLockScope lock(m_LockSocketStatus);
	m_Verified = verifyed;
}

bool CAntConnection::IsVerified()
{
	CLockScope lock(m_LockSocketStatus);
	bool result = m_Verified;
	return result;
}

bool CAntConnection::IsVerifying()
{
	CLockScope lock(m_LockSocketStatus);
	bool result = m_Verifying;
	return result;
}

void CAntConnection::SetVerifying( bool b )
{
	CLockScope lock(m_LockSocketStatus);
	m_Verifying = b;
}

void CAntConnection::SetMeetingID( UINT meetingID )
{
	CLockScope lock(m_LockSocketStatus);
	m_MeetingID = meetingID;
}

UINT CAntConnection::GetMeetingID()
{
	CLockScope lock(m_LockSocketStatus);
	UINT result = m_MeetingID;
	return result;
}
