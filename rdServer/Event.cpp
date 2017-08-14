#include "StdAfx.h"
#include "Event.h"

namespace winapi
{


CEvent::CEvent(void)
{
	m_hHandle = NULL;
}


CEvent::~CEvent(void)
{
	CloseHandle( m_hHandle );
}

BOOL CEvent::Init( LPSECURITY_ATTRIBUTES lpSA, BOOL bManualReset, BOOL bInitialState, LPCTSTR lpName )
{
	if( m_hHandle != NULL )
		return FALSE;

	m_hHandle = ::CreateEvent( lpSA, bManualReset, bInitialState, lpName );
	if( m_hHandle == NULL )
	{
		ATLTRACE( _T("CAntEvent::Init:%d\r\n"), GetLastError() );
		return FALSE;
	}

	return TRUE;
}

HANDLE	CEvent::GetHandle()
{
	return m_hHandle;
}

BOOL CEvent::SetEvent()
{
	return ::SetEvent( m_hHandle );
}

BOOL CEvent::Reset()
{
	return ::ResetEvent(m_hHandle);
}

DWORD CEvent::Wait( DWORD ms )
{
	return ::WaitForSingleObject(m_hHandle, ms );
}


}