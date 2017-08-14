#include "StdAfx.h"
#include "Thread.h"

namespace winapi
{

CThread::CThread(void)
{
	m_hThread = 0;
	m_nThreadID = 0;
}


CThread::~CThread(void)
{
	if( m_hThread != NULL )
	{
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}

BOOL CThread::Start()
{
	m_hThread = (HANDLE)::_beginthreadex( 0, 0, ThreadFunction, (void*)this, 0/*CREATE_SUSPENDED*/, &m_nThreadID );
	if( m_hThread == 0 )
		return FALSE;

	return TRUE;
}

//===========================================
// 此处的Stop函数为强制结束线程，不建议使用
//===========================================
BOOL CThread::Stop()
{
	//_endthreadex(-1);
	//
	//if( m_hThread != 0 )
	//{
	//	::CloseHandle( m_hThread );
	//	m_hThread = 0;
	//}

	return FALSE;
}

UINT __stdcall CThread::ThreadFunction(void *pV)
{
	//crInstallToCurrentThread2(0);
	CThread *pThread = (CThread*)pV;

	if( pThread != NULL )
		pThread->Run();
	//crUninstallFromCurrentThread();
	return 0;
}


}