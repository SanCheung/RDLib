#pragma once
#include <Windows.h>

//临界区是可重入的, 但是注意enter 和 leave 的次数要相等,否则害死人, deadlock将欢迎你
namespace CASYNC
{
class XDBASE_API CSectionCritical 
{
public:
	CSectionCritical();
	~CSectionCritical();

	virtual void Enter();
	virtual void Leave();
	virtual bool TryEnter();

private:
	CRITICAL_SECTION	m_CriticalSection;
	LONG		m_EnterCount;
};

};