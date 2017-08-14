#include "StdAfx.h"
#include "Lock.h"

namespace winapi
{

CCriticalSection::CCriticalSection(void)
{
	Init();
}


CCriticalSection::~CCriticalSection(void)
{
	Term();
}

void CCriticalSection::Init()
{
	::InitializeCriticalSectionAndSpinCount(&m_sec, 1500);
}

void CCriticalSection::Term()
{
	::DeleteCriticalSection(&m_sec);
}

void CCriticalSection::Lock()
{
	::EnterCriticalSection(&m_sec);
}

void CCriticalSection::UnLock()
{
	::LeaveCriticalSection(&m_sec);
}

BOOL CCriticalSection::TryLock()
{
	return ::TryEnterCriticalSection(&m_sec);
}

CLockScope::CLockScope( CCriticalSection& lock ) :m_Lock(lock)
{
	m_Lock.Lock();
}

CLockScope::~CLockScope()
{
	m_Lock.UnLock();
}

}
