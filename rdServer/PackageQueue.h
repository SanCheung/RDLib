#pragma once

#include <memory>
#include <list>

using namespace std;
using namespace std::tr1;

#include "Lock.h"
#include "Event.h"
#include "Package.h"

using namespace winapi;



typedef list<CAntPackagePtr>		AntPackageQueue;
typedef list<CAntPackagePtr>::iterator AntPackageQueueItr;

class CPackageQueue
{
public:
	CPackageQueue(void);
	~CPackageQueue(void);

	void Push(CAntPackagePtr& package);
	
	void PushHead(CAntPackagePtr& package);

	CAntPackagePtr Pop();

	int	Size();

	bool WaitPop(int ms);

private:
	CCriticalSection		m_LockQueue;
	AntPackageQueue			m_Queue;

	CEvent					m_EventPop;
};
