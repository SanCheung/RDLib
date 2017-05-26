#pragma once
#include <Windows.h>

//�ٽ����ǿ������, ����ע��enter �� leave �Ĵ���Ҫ���,��������, deadlock����ӭ��
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