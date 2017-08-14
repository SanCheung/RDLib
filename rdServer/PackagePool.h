#pragma once

#include "Package.h"
#include "PackageQueue.h"

class CTCPServer;
class CPackagePool
{
public:
	CPackagePool( CTCPServer* pServer );
	virtual ~CPackagePool(void);

	void Process();
	bool Idle();
	void AddInPackage(CAntPackagePtr& package);
	bool WaitProcess(int ms);


private:
	bool OnProcessPackage( UINT sessionID, UINT userID, UINT meetingID, CAntPackagePtr& package);

private:
	CPackageQueue			m_PackageInQueue;		
	CTCPServer*				m_pServer;	


	//stat
	DWORD					m_MaxElapsed;
	winapi::CCriticalSection		m_LockStat;
};

