#pragma once

#include "Thread.h"
#include "IOCP.h"

class CIOCPServer;

class CIOCPWorkThread : public winapi::CThread
{
public:
	CIOCPWorkThread(void);
	virtual ~CIOCPWorkThread(void);
public:
	virtual BOOL	Start();
	virtual BOOL	Stop();
	virtual void	Run();
public:
	void	SetIOCP( winapi::CIOCP *pIOCP);
	void	SetIOCPServer(CIOCPServer *pServer);
protected:
	winapi::CIOCP	*m_pIOCP;
	CIOCPServer		*m_pServer;
};

