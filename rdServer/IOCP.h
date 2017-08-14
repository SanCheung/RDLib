#pragma once

namespace winapi
{


class CIOCP
{
public:
	CIOCP(void);
	~CIOCP(void);
public:
	BOOL	Init();
	BOOL	Attach( HANDLE hDevice, ULONG_PTR ck );
	BOOL	GetStatus( LPDWORD lpSize, PULONG_PTR ck, LPOVERLAPPED *lpOverlapped, DWORD ms );
	BOOL	Post( DWORD dwSize, ULONG_PTR ck, LPOVERLAPPED lpOverlapped );

protected:
	HANDLE	m_hIOCP;
};


}