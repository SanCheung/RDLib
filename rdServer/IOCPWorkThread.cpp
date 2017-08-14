#include "StdAfx.h"
#include "IOCPWorkThread.h"
#include "IOCPServer.h"

CIOCPWorkThread::CIOCPWorkThread(void)
{
	m_pIOCP = NULL;
	m_pServer = NULL;
}


CIOCPWorkThread::~CIOCPWorkThread(void)
{
}

BOOL CIOCPWorkThread::Start()
{
	if( m_pIOCP == NULL )
		return FALSE;
	
	return winapi::CThread::Start();
}

BOOL CIOCPWorkThread::Stop()
{
//	if( m_pIOCP != NULL )
//		m_pIOCP->PostStatus(-1, NULL, NULL);

	DWORD dwReturn = ::WaitForSingleObject(m_hThread, 2000);

	if( dwReturn == WAIT_TIMEOUT || dwReturn == WAIT_FAILED )
		return winapi::CThread::Stop();

	return TRUE;

}

void CIOCPWorkThread::Run()
{
	if( m_pIOCP == NULL || m_pServer == NULL )
			return;

	DWORD			dwBytes = 0;
	ULONG_PTR		lpCompletionKey;
	LPOVERLAPPED	lpOverlapped;
	BOOL			bReturn = FALSE;

	while(true)
	{
		bReturn = m_pIOCP->GetStatus( &dwBytes, &lpCompletionKey, &lpOverlapped, INFINITE );
		
		if( !bReturn )
		{
			ATLTRACE( _T("CIOCPWorkThread GetStatus Error:%d\r\n"), GetLastError() );
		}

		// 发送到事务处理线程处理
		m_pServer->DoWork(dwBytes, lpCompletionKey, lpOverlapped);
	}
}

void CIOCPWorkThread::SetIOCP(CIOCP *pIOCP)
{
	m_pIOCP = pIOCP;
}

void CIOCPWorkThread::SetIOCPServer(CIOCPServer *pServer)
{
	m_pServer = pServer;
}