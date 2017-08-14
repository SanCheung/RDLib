#pragma once

class CIOCPServer;

template<class T>
class TThreadPool
{
public:

	TThreadPool(void)
	{
		m_nMinThreads = 5;
		m_nMaxThreads = 10;
	}

	~TThreadPool(void)
	{
		Stop();
	}

public:
	BOOL	Init(UINT nMinThreads, UINT nMaxThreads)
	{
		m_nMinThreads = nMinThreads;
		m_nMaxThreads = nMaxThreads;
		return m_IOCP.Init();
	}

	void	Start()
	{
		for( UINT nIndex = 0; nIndex < m_nMinThreads; nIndex ++ )
		{
			T	*pThread = new T;

			m_listWorkThread.push_back(pThread);
			pThread->SetIOCP(&m_IOCP);
			pThread->SetIOCPServer(m_pServer);

			pThread->Start();
		}
	}

	void	Stop()
	{
		T	*pThread = NULL;

		list<T*>::iterator	item = m_listWorkThread.begin();

		while( item != m_listWorkThread.end() )
		{
			pThread = *item;
			pThread->Stop();
			delete pThread;
			item++;
		}
		m_listWorkThread.clear();
	}

	BOOL	Post(DWORD dwBytes, ULONG_PTR lpCompletionKey, LPOVERLAPPED lpOverlapped )
	{
		return m_IOCP.Post( dwBytes, lpCompletionKey, lpOverlapped );
	}
	
	void	SetIOCPServer(CIOCPServer *pServer)
	{
		m_pServer = pServer;
	}


protected:
	UINT	m_nMinThreads;
	UINT	m_nMaxThreads;
	CIOCP	m_IOCP;	// 用来调度线程
	CIOCPServer	*m_pServer;
	list<T*>	m_listWorkThread;
};

