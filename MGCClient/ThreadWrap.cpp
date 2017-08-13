#include "StdAfx.h"
#include "ThreadWrap.h"

#include "Helper.h"
#include <process.h>

namespace xdmp
{


ThreadWrap::ThreadWrap(void)
	: _hThread( nullptr )
	, _bExitFlag( false )
{
	_hExitEvent = ::CreateEvent( &CGlobalSecurityAttributes::instance(), FALSE, FALSE, L"" );
}


ThreadWrap::~ThreadWrap(void)
{
	Close();

	if( nullptr != _hExitEvent )
	{
		CloseHandle( _hExitEvent );
		_hExitEvent = nullptr;
	}
}


UINT WINAPI ThreadWrap::funProcess( LPVOID pVoid )
{
	ThreadWrap *pThis = (ThreadWrap *)pVoid;
	ASSERT(pThis!=nullptr);
	if(pThis){
		pThis->thread_main();
		pThis->Close();
	}
	
	return 0;
}

void ThreadWrap::Close()
{
	if( _hThread != nullptr )
	{
		::CloseHandle( _hThread );
		_hThread = nullptr;
	}
}

int ThreadWrap::Wait( int nMs /*= 3000 */ )
{
	return ::WaitForSingleObject( _hThread, nMs );
}


int ThreadWrap::WaitEvent( int nMs /*= 3000 */ )
{
	return ::WaitForSingleObject( _hExitEvent, nMs );
}


void ThreadWrap::thread_main()
{
	ASSERT( FALSE );
}

void ThreadWrap::Start()
{
	if( IsRunning() )
		return;

	Close();

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


void ThreadWrap::ShutdownUseEvent()
{
	if( !IsRunning() )
		return;

	::SetEvent( _hExitEvent );
	int nRet = Wait(INFINITE);
	Close();
}


bool ThreadWrap::IsRunning()
{
	return ( _hThread != nullptr );
}


}