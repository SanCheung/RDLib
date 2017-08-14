#include "StdAfx.h"
#include "PackageQueue.h"


CPackageQueue::CPackageQueue(void)
{
	m_EventPop.Init(NULL, FALSE, FALSE, NULL);
}


CPackageQueue::~CPackageQueue(void)
{
}

void CPackageQueue::Push( CAntPackagePtr& package )
{
	CLockScope lock(m_LockQueue);
	m_Queue.push_back(package);
	m_EventPop.SetEvent();
}

void CPackageQueue::PushHead( CAntPackagePtr& package )
{
	CLockScope lock(m_LockQueue);
	m_Queue.push_front(package);
	m_EventPop.SetEvent();
}

CAntPackagePtr CPackageQueue::Pop()
{
	CAntPackagePtr package;
	CLockScope lock(m_LockQueue);
	if (m_Queue.size())
	{
		package = m_Queue.front();
		m_Queue.pop_front();
	}
	return package;
}

int CPackageQueue::Size()
{
	CLockScope lock(m_LockQueue);
	return m_Queue.size();
}

bool CPackageQueue::WaitPop( int ms )
{
	return m_EventPop.Wait(ms) == WAIT_OBJECT_0;
}
