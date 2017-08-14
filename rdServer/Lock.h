#pragma once

namespace winapi
{


class CCriticalSection
{
public:
	CCriticalSection(void);
	~CCriticalSection(void);
public:
	void	Init();
	void	Lock();
	BOOL	TryLock();
	void	UnLock();
	void	Term();
protected:
	CRITICAL_SECTION	m_sec;
};

class CLockScope
{
public:
	CLockScope(CCriticalSection& lock);
	~CLockScope();
	CCriticalSection&	m_Lock;
};


}

