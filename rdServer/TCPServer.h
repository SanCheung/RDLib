#pragma once

#include "IOCPServer.h"
#include "ListenThread.h"
#include "TObjPool.h"
#include "AntConnection.h"
#include "TThreadPool.h"
#include "PackagePool.h"

class CTCPServer  : public CIOCPServer
{
public:
	CTCPServer(void);
	virtual ~CTCPServer(void);

public:
	virtual BOOL	Init( int nPort );
	virtual BOOL	Start();
	virtual BOOL	Stop();
	virtual void	DoWork(DWORD dwBytes, ULONG_PTR ck, LPOVERLAPPED lpOverlapped);

public:
	BOOL	AcceptSocket( SOCKET socket );

	CAntPackage*	GetPackage();
	void			ReleasePackage(CAntPackage *pPackage);

	void	PrepareDoProcess();
	void	DoProcess();

	void	TimeEvent();
	void	OnTime();

	void	OnProcessPackage(CAntConnection* pSession,  CAntPackagePtr& pPackage );

	CAntConnection* GetSession( UINT sessionID );

	bool	ResponseToSession( UINT sessionID, CBuffer& response );
	void	AddDelayReapSession( UINT sessionID );

protected:
	bool						m_Stoped;
	CListenThread				m_listenThread;

	TObjPool<CAntConnection>	m_ConnectionPool;
	TObjPool<CAntPackage>		m_PackagePool;

	CCriticalSection			m_LockMeetingMap;

	UINT						m_nMinSupportVer; // 当前最小支持的版本

	//__int64						m_LastTimeEvent;
	//Meeting::CMeetingTimetable	m_MeetingTimetable;
	//CCriticalSection			m_LockMeetingTimeoutCheckMap;
	//MeetingTimeMap				m_MeetingTimeoutCheckMap;

	list<CAntConnection*>		m_DelayReapConnections;
	CCriticalSection			m_LockDealyReapConnectionList;

	typedef map<UINT, CAntConnection*>	ConnectionSessions;
	typedef map<UINT, CAntConnection*>::iterator	ConnectionSessionsItr;
	ConnectionSessions			m_Connections;
	CCriticalSection			m_LockConnections;
	
	UINT						m_SessionSeed;

	//Meeting::CMeetingProcess	m_MeetingProcess;
	CPackagePool				m_pool;
};

