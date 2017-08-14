#pragma once

#include "stdafx.h"

#include "IOCPBuffer.h"
#include "IOCP.h"

#include "IOCPWorkThread.h"

#include "TObjPool.h"
#include "TThreadPool.h"

#include <vector>

class CIOCPWorkThread;

using namespace std;
using namespace winapi;

class CIOCPServer
{
public:
	CIOCPServer(void);
	virtual ~CIOCPServer(void);
public:
	virtual BOOL	Init( int nPort );
	virtual BOOL	Start();
	virtual BOOL	Stop();
	virtual void	DoWork( DWORD dwBytes, ULONG_PTR lpCompletionKey, LPOVERLAPPED lpOverlapped );

public:
	CIOCPBuffer*	GetIOCPBuffer();
	void			ReleaseIOCPBuffer(CIOCPBuffer *pBuffer);

	void	PostWork( DWORD dwBytes, ULONG_PTR lpCompletionKey, LPOVERLAPPED lpOverlapped );

protected:
	BOOL	StartIOCPWorkThreads();
	void	StopIOCPWorkThreads();

public:
	SOCKET			m_hSocket;
	u_short			m_nPort;

protected:		
	CIOCP		m_IOCP;

	vector<CIOCPWorkThread*>		m_IOCPThreads;
	TThreadPool<CIOCPWorkThread>	m_WorkThreadPool;

	TObjPool<CIOCPBuffer>			m_IOCPBufferPool;
};


