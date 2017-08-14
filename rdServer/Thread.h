#pragma once

namespace winapi
{

class CThread
{
public:
	CThread(void);
	virtual ~CThread(void);
public:
	virtual BOOL	Start();
	virtual BOOL	Stop();
	virtual void	Run() = 0;

public:
	HANDLE	GetHandle()		{ return m_hThread; }
	UINT	GetTheadID()	{ return m_nThreadID; }

protected:
	static UINT __stdcall ThreadFunction(void *pV);
	HANDLE	m_hThread;
	UINT	m_nThreadID;
};

}
