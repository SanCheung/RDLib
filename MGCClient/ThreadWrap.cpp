#include "StdAfx.h"
#include "ThreadWrap.h"

//#include "Helper.h"
#include <process.h>

namespace xdmp
{


ThreadWrap::ThreadWrap(void)
	: _hThread( nullptr )
	, _bExitFlag( false )
{
	//_hExitEvent = ::CreateEvent( &CGlobalSecurityAttributes::instance(), FALSE, FALSE, L"" );
	_hExitEvent = ::CreateEvent( NULL, FALSE, FALSE, L"" );
}


ThreadWrap::~ThreadWrap(void)
{
	thClose();

	if( nullptr != _hExitEvent )
	{
		CloseHandle( _hExitEvent );
		_hExitEvent = nullptr;
	}
}


UINT WINAPI ThreadWrap::funProcess( LPVOID pVoid )
{
	ThreadWrap *pThis = (ThreadWrap *)pVoid;
	//ASSERT(pThis!=nullptr);
	if(pThis)
	{
		pThis->thread_main();
		pThis->thClose();
	}
	
	return 0;
}

void ThreadWrap::thClose()
{
	if( _hThread != nullptr )
	{
		::CloseHandle( _hThread );
		_hThread = nullptr;
	}
}

int ThreadWrap::thWait( int nMs /*= 3000 */ )
{
	return ::WaitForSingleObject( _hThread, nMs );
}


int ThreadWrap::thWaitEvent( int nMs /*= 3000 */ )
{
	return ::WaitForSingleObject( _hExitEvent, nMs );
}


void ThreadWrap::thread_main()
{
	//ASSERT( FALSE );
}

void ThreadWrap::thStart()
{
	if( thIsRunning() )
		return;

	thClose();

	DWORD	_dwWriteThreadID= 0;
	_hThread = (HANDLE)_beginthreadex(	NULL, 0, funProcess, this, CREATE_SUSPENDED,
		(unsigned int*)&_dwWriteThreadID);

	if ( _hThread )
		ResumeThread( _hThread );
}

//void ThreadWrap::Shutdown()
//{
//	if( !IsRunning() )
//		return;
//
//	SetExitFlag();
//
//	int nRet = Wait(INFINITE);
//	Close();
//	//if( WAIT_TIMEOUT == nRet )
//	//	Close();
//}


void ThreadWrap::thShutdownUseEvent()
{
	if( !thIsRunning() )
		return;

	::SetEvent( _hExitEvent );
	int nRet = thWait(INFINITE);
	thClose();
}


bool ThreadWrap::thIsRunning()
{
	return ( _hThread != nullptr );
}


}