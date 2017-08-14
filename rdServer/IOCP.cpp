#include "StdAfx.h"
#include "IOCP.h"

namespace winapi
{


CIOCP::CIOCP(void)
{
	m_hIOCP = INVALID_HANDLE_VALUE;
}


CIOCP::~CIOCP(void)
{
	::CloseHandle( m_hIOCP );
}

BOOL CIOCP::Init()
{
	if( m_hIOCP != INVALID_HANDLE_VALUE )
		return FALSE;

	m_hIOCP = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );

	if( m_hIOCP == NULL )
	{
		ATLTRACE( _T("CAntIOCP::Init:%d\r\n"), GetLastError() );
		return FALSE;
	}
	return TRUE;
}

BOOL CIOCP::Attach( HANDLE hDevice, ULONG_PTR ck )
{
	if( m_hIOCP == INVALID_HANDLE_VALUE )
		return FALSE;

	if( m_hIOCP != ::CreateIoCompletionPort( hDevice, m_hIOCP, ck, 0 ) )
		return FALSE;

	return TRUE;
}

BOOL CIOCP::GetStatus( LPDWORD lpSize, PULONG_PTR ck, LPOVERLAPPED *lpOverlapped, DWORD ms )
{
	return ::GetQueuedCompletionStatus( m_hIOCP, lpSize, ck, lpOverlapped, ms );
}

BOOL CIOCP::Post( DWORD dwSize, ULONG_PTR ck, LPOVERLAPPED lpOverlapped )
{
	return ::PostQueuedCompletionStatus( m_hIOCP, dwSize, ck, lpOverlapped );
}


}