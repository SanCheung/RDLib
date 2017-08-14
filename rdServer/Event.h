#pragma once

namespace winapi
{

class CEvent
{
public:
	CEvent(void);
	virtual ~CEvent(void);
public:
	BOOL	Init( LPSECURITY_ATTRIBUTES lpSA, BOOL bManualReset, 
				  BOOL bInitialState, LPCTSTR lpName );
	HANDLE	GetHandle();
	BOOL	SetEvent();
	BOOL	Reset();
	DWORD	Wait(DWORD ms);
protected:
	HANDLE	m_hHandle;
};


}

