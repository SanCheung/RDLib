#include "StdAfx.h"
#include "TCPServer.h"


CTCPServer::CTCPServer(void)
	: m_pool( this )
{
	//m_nMinSupportVer = 0; // 0 表示没有版本限制
	//m_LastTimeEvent = 0;
	m_SessionSeed = 1000;
	m_Stoped = false;
}


CTCPServer::~CTCPServer(void)
{
}

BOOL CTCPServer::Init( int nPort )
{
	if( !CIOCPServer::Init( nPort ) )
		return FALSE;

	if( ::listen( m_hSocket, SOMAXCONN ) == SOCKET_ERROR )
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
		return FALSE;
	}

	return TRUE;
}

BOOL CTCPServer::Start()
{
	if( !CIOCPServer::Start() )
		return FALSE;

	// 创建Accept监听线程
	m_listenThread.SetIOCPServer( this );
	m_listenThread.Start();

	TimeEvent();

	for (int i = 0; i < 2; ++i)
	{
		PrepareDoProcess();
	}
	return TRUE;
}

BOOL CTCPServer::Stop()
{
	m_Stoped = true;
	return CIOCPServer::Stop();
}

void CTCPServer::DoWork( DWORD dwBytes, ULONG_PTR ck, LPOVERLAPPED lpOverlapped )
{
	CAntConnection	*pConnection = (CAntConnection*)ck;
	CIOCPBuffer		*pBuffer = (CIOCPBuffer*)lpOverlapped;
	
	/*
	if( dwBytes == 0 )
	{
		// 关闭Connection连接

	}*/
	int operation = pBuffer->GetOperation();
	if( operation == IOCP_OPERATION_SEND )
	{
		if( dwBytes == 0 )
		{
			DWORD dwErr = WSAGetLastError();
			ATLTRACE(_T("WSASend:%d\r\n"), dwBytes);
		}
		// 发送
		ATLTRACE(_T("WSASend:%d\r\n"), dwBytes);
		ReleaseIOCPBuffer(pBuffer);
		pConnection->OnSendCompleted();
	}
	else if( operation == IOCP_OPERATION_RECV )
	{
		ATLTRACE(_T("WSARecv:%d\r\n"), dwBytes);
		pBuffer->SetUsed(dwBytes);
		BOOL bReturn = pConnection->OnReceivedCompleted(pBuffer);
	}
	else if ( operation == IOCP_OPERAITON_TIMING)
	{
		ReleaseIOCPBuffer(pBuffer);
		OnTime();
	}
	else if ( operation == IOCP_OPERAITON_CUSTOME)
	{
		ReleaseIOCPBuffer(pBuffer);
		DoProcess();
	}
	else{
		int i = 0;
	}
	return;
}

BOOL CTCPServer::AcceptSocket( SOCKET socket )
{
	CAntConnection *pConnection = m_ConnectionPool.GetObj();

	if( pConnection == NULL )
		return FALSE;

	{
		CLockScope lock(m_LockConnections);

		pConnection->SetSessionID(m_SessionSeed);
		m_Connections[m_SessionSeed++] = pConnection;

		pConnection->SetTCPServer(this);
		pConnection->SetSocket( socket );
		m_IOCP.Attach( (HANDLE)socket, (ULONG_PTR)pConnection );
		pConnection->ReqRecvData(NULL);
	}
	return TRUE;
}

void CTCPServer::PrepareDoProcess()
{
	CIOCPBuffer *pBuffer = GetIOCPBuffer();
	pBuffer->Init();
	pBuffer->SetOperation( IOCP_OPERAITON_CUSTOME);

	PostWork(0, 0, (WSAOVERLAPPED*)pBuffer);
}

void CTCPServer::TimeEvent()
{
	CIOCPBuffer *pBuffer = GetIOCPBuffer();
	pBuffer->Init();
	pBuffer->SetOperation( IOCP_OPERAITON_TIMING );

	PostWork(0, 0, (WSAOVERLAPPED*)pBuffer);
}

void CTCPServer::OnTime()
{
	////销毁过期的meeting
	//if (m_MeetingTimeoutCheckMap.size())
	//{
	//	CLockScope lock(m_LockMeetingTimeoutCheckMap);
	//	for(MeetingTimeMap::iterator itr = m_MeetingTimeoutCheckMap.begin(); itr != m_MeetingTimeoutCheckMap.end(); )
	//	{
	//		if( Meeting::CMeetingTimetable::IsTimeout(itr->second) )
	//		{			
	//			UINT meetingID = itr->first;
	//			m_MeetingProcess.CloseMeeting(meetingID, MEETING_CLOSE_BY_TIMEOUT);
	//			itr = m_MeetingTimeoutCheckMap.erase( itr);

	//			m_MeetingTimetable.RemoveTimeoutTimeItem();
	//		}
	//		else
	//		{
	//			itr++;
	//		}
	//	}
	//}

	//__int64 now = Ticks::GetTicks();
	//if (m_LastTimeEvent == 0){
	//	m_LastTimeEvent = now;
	//}
	//else if (now - m_LastTimeEvent < 50)
	//{
	//	Sleep( 50 - DWORD(now - m_LastTimeEvent));
	//}
	//m_LastTimeEvent = Ticks::GetTicks();

	Sleep( 100 );

	TimeEvent();
}

void CTCPServer::DoProcess()
{
	while (!m_Stoped)
	{
		if (m_pool.WaitProcess(1))
		{
			m_pool.Process();
		}
		else
		{
			//移除无效的连接
			if (m_DelayReapConnections.size())
			{
				CLockScope lock(m_LockDealyReapConnectionList);
				for(list<CAntConnection*>::iterator itr = m_DelayReapConnections.begin(); itr != m_DelayReapConnections.end(); )
				{
					if( (*itr)->DoClose() == 0 )
					{			
						m_ConnectionPool.ReleaseObj(*itr);
						itr = m_DelayReapConnections.erase( itr);
					}
					else
					{
						itr++;
					}
				}
			}
		}
	}
}

void CTCPServer::OnProcessPackage( CAntConnection* pConnection, CAntPackagePtr& pPackage )
{
	UINT userID = 0;
	UINT meetingID = 0;
	UINT sessionID = pConnection->GetSessionID();
	bool progress = true;
	if (pConnection->IsVerified())
	{
		//userID = pConnection->GetUserID();
		//meetingID = pConnection->GetMeetingID();
	}
	else
	{
		//if (!pSession->IsVerifying() && pPackage->GetMethod() == "evm")
		//{
		//	userID = atoi(pPackage->GetProperty("userid").c_str());
		//	meetingID = atoi(pPackage->GetProperty("meetingid").c_str());
		//	pSession->SetVerifying(true);
		//}
		//else if(!pSession->IsVerifying())
		//{
		//	progress = false;
		//}
	}

	if (progress)
	{
		pPackage->AddProperty("Internal_SessionID", sessionID);
		//pPackage->AddProperty("Internal_UserID", userID);
		//pPackage->AddProperty("Internal_MeetingID", meetingID);

		m_pool.AddInPackage( pPackage );
	}
	else
	{
		AddDelayReapSession(sessionID);
	}
}

CAntPackage* CTCPServer::GetPackage()
{
	return m_PackagePool.GetObj();
}

void CTCPServer::ReleasePackage( CAntPackage *pPackage )
{
	m_PackagePool.ReleaseObj(pPackage);
}


CAntConnection* CTCPServer::GetSession( UINT sessionID )
{
	CAntConnection* pConnection = NULL;
	CLockScope lock(m_LockConnections);
	ConnectionSessionsItr itr =  m_Connections.find(sessionID);
	if (itr != m_Connections.end()){
		pConnection = itr->second;
	}
	return pConnection;
}

bool CTCPServer::ResponseToSession( UINT sessionID, CBuffer& response )
{
	bool result = false;
	CLockScope lock(m_LockConnections);
	CAntConnection* pSession = GetSession(sessionID);
	if (pSession)
	{
		result = (TRUE == pSession->ReqSendData(response, response.GetSize()) );
	}
	return result;
}

void CTCPServer::AddDelayReapSession( UINT sessionID )
{
	CLockScope lock(m_LockConnections);
	CAntConnection* pSession = GetSession(sessionID);
	if (pSession)
	{
		pSession->SetVerified(false);
		pSession->SetVerifying(false);
		m_LockDealyReapConnectionList.Lock();
		m_DelayReapConnections.push_back(pSession);
		m_LockDealyReapConnectionList.UnLock();

		m_Connections.erase(sessionID);
	}
}