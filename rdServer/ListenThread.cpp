#include "StdAfx.h"
#include "ListenThread.h"
#include "TCPServer.h"

CListenThread::CListenThread(void)
{
	m_pServer = NULL;
}


CListenThread::~CListenThread(void)
{
}

BOOL CListenThread::Start()
{
	if( m_pServer == NULL )
		return FALSE;

	m_eventStop.Init( NULL, TRUE, FALSE, NULL );

	return CThread::Start();
}

void CListenThread::Run()
{
	DWORD		dwReturn = 0;
	struct sockaddr_in	saClient;
	int			nSize = sizeof(saClient);

	while( true )
	{
		dwReturn = ::WaitForSingleObject( m_eventStop.GetHandle(), 0 );
		if( dwReturn == WAIT_OBJECT_0 )
			break;
		else if( dwReturn == WAIT_FAILED )
		{
			ATLTRACE( _T("CistenThread::Run:%d\r\n"), GetLastError() );
			break;
		}

		// accept socket connect
		SOCKET	socket = ::WSAAccept( m_pServer->m_hSocket, (SOCKADDR*)&saClient, &nSize, 0, 0 );
		if( socket != INVALID_SOCKET )
		{
		//	ATLTRACE( _T("WSAAccept error:%d\r\n"), WSAGetLastError() );
			int ret = 0;
			/*
			在time_wait状态的端口可以继续利用
			*/
			static int reuseAddress = 1;
			ret = setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuseAddress,sizeof(int));

			static linger s_linger;
			s_linger.l_onoff = 1; 
			s_linger.l_linger = 5; 
			ret = setsockopt(socket, SOL_SOCKET, SO_LINGER, (const char*)&s_linger, sizeof(linger));

			m_pServer->AcceptSocket( socket );
		}
		else
		{
			ATLTRACE( _T("WSAAccept error:%d\r\n"), WSAGetLastError() );
		}
	}

	ATLTRACE( _T("Thread over\r\n") );
}

BOOL CListenThread::Stop()
{
	m_eventStop.SetEvent();
	DWORD dwReturn = WaitForSingleObject( m_hThread, INFINITE );
	if( dwReturn == WAIT_OBJECT_0 )
	{
		::CloseHandle( m_hThread );
		m_hThread = NULL;
		return TRUE;
	}

	return FALSE;
}

void CListenThread::SetIOCPServer( CTCPServer *pServer )
{
	m_pServer = pServer;
}