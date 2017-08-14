#include "StdAfx.h"
#include "IOCPServer.h"
#include "IOCPWorkThread.h"


CIOCPServer::CIOCPServer(void)
{
	m_hSocket = INVALID_SOCKET;
	m_nPort = 36500;
}


CIOCPServer::~CIOCPServer(void)
{
}

BOOL CIOCPServer::Init( int nPort )
{
	m_nPort = nPort;

	WSAData		wsaData;

	if( WSAStartup( MAKEWORD(2,2), &wsaData ) != 0 )
	{
		ATLTRACE( _T("CIOCPServer::Init:%d\r\n"), WSAGetLastError() );
		return FALSE;
	}

	if( m_hSocket != INVALID_SOCKET )
		return FALSE;

	m_hSocket = ::WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED );
	if( m_hSocket == INVALID_SOCKET )
	{
		ATLTRACE( _T("CIOCPServer::Start:%d"), WSAGetLastError() );
		return FALSE;
	}
	
	sockaddr_in		service; 

	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(m_nPort);

	int nReturn = bind( m_hSocket, (SOCKADDR*)&service, sizeof(service) );

	if( nReturn == SOCKET_ERROR )
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
		return FALSE;
	}

	///////////////////////////////////
	char	name[155];
	//获取机器名 
	if( gethostname ( name, sizeof(name)) == 0)
	{   
		PHOSTENT hostinfo;
		//这些就是获得IP的函数	
		if((hostinfo = gethostbyname(name)) != NULL)
		{
			std::string strIp;
			strIp = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list); //获取本机ip

			printf( "-->%s:%d\n", strIp.c_str(), m_nPort );
		}
	}

	return TRUE;
}

BOOL CIOCPServer::Start()
{
	// 启动IOCP
	m_IOCP.Init();

	// 启动IOCP Workthread
	StartIOCPWorkThreads();

	// 启动AntWorkThreadPool
	m_WorkThreadPool.SetIOCPServer(this);
	m_WorkThreadPool.Init(5,10);
	m_WorkThreadPool.Start();

	return TRUE;
}

BOOL CIOCPServer::Stop()
{
	if( m_hSocket != INVALID_SOCKET )
	{
		::closesocket(m_hSocket);
	}
	WSACleanup();
	return TRUE;
}

//==========================
// 
//==========================
BOOL CIOCPServer::StartIOCPWorkThreads()
{
	SYSTEM_INFO		info;

	::GetSystemInfo(&info);
	
	DWORD dwThreads = info.dwNumberOfProcessors;

	CIOCPWorkThread		*pThread = NULL;

	while( dwThreads > 0 )
	{
		pThread = new CIOCPWorkThread;
		m_IOCPThreads.push_back(pThread);
		
		pThread->SetIOCPServer(this);
		pThread->SetIOCP(&m_IOCP);
		pThread->Start();
		dwThreads --;
	}
	return TRUE;
}

void CIOCPServer::StopIOCPWorkThreads()
{
	CIOCPWorkThread	*pThread = NULL;

	for( DWORD dwIndex = 0; dwIndex < m_IOCPThreads.size(); dwIndex ++ )
	{
		pThread = m_IOCPThreads.at(dwIndex);
		pThread->Stop();
		delete pThread;
	}

	m_IOCPThreads.erase(m_IOCPThreads.begin(), m_IOCPThreads.end());
}

void CIOCPServer::PostWork(DWORD dwBytes, ULONG_PTR lpCompletionKey, LPOVERLAPPED lpOverlapped )
{
	m_WorkThreadPool.Post(dwBytes, lpCompletionKey, lpOverlapped);
}

void CIOCPServer::DoWork(DWORD dwBytes, ULONG_PTR lpCompletionKey, LPOVERLAPPED lpOverlapped)
{

}

CIOCPBuffer* CIOCPServer::GetIOCPBuffer()
{
	return m_IOCPBufferPool.GetObj();
}

void CIOCPServer::ReleaseIOCPBuffer(CIOCPBuffer *pBuffer)
{
	m_IOCPBufferPool.ReleaseObj(pBuffer);
}