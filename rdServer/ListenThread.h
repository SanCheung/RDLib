#pragma once

#include "Thread.h"
#include "Event.h"

class CTCPServer;
class CListenThread : public winapi::CThread
{
public:
	CListenThread(void);
	virtual ~CListenThread(void);
public:
	virtual BOOL	Start();
	virtual void	Run();
	virtual BOOL	Stop();
public:
	void	SetIOCPServer(CTCPServer *pServer);
protected:
	winapi::CEvent	m_eventStop;
	CTCPServer		*m_pServer;
};

