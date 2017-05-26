#include "StdAfx.h"
#include "SectionCritical.h"

namespace CASYNC
{


CSectionCritical::CSectionCritical()
{
	InitializeCriticalSectionAndSpinCount(&m_CriticalSection, 1000);
	m_EnterCount = 0;
}

CSectionCritical::~CSectionCritical()
{
	DeleteCriticalSection(&m_CriticalSection);
}

void CSectionCritical::Enter()
{
	EnterCriticalSection(&m_CriticalSection);
	InterlockedIncrement(&m_EnterCount);
}

void CSectionCritical::Leave()
{
	LeaveCriticalSection(&m_CriticalSection);
	InterlockedDecrement(&m_EnterCount);
}

bool CSectionCritical::TryEnter()
{
	return (bool)TryEnterCriticalSection(&m_CriticalSection);
}

};