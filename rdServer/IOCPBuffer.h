#pragma once


#define	IOCP_OPERATION_NONE		(0)
#define IOCP_OPERATION_RECV		(1)
#define IOCP_OPERATION_SEND		(2)
#define	IOCP_OPERATION_RECVFROM	(3)
#define IOCP_OPERATION_SENDTO	(4)
#define IOCP_OPERAITON_CLOSE	(5) // πÿ±’¡¨Ω”
#define IOCP_OPERAITON_TIMING	(6)
#define IOCP_OPERAITON_CUSTOME	(7)

#define IOCP_BUF_SIZE		(4096)


namespace winapi
{

class CIOCPBuffer
{
public:
	CIOCPBuffer(void);
	~CIOCPBuffer(void);
public:
	void	Init();
	void	Clear();
	WSABUF*	GetWSABuf();
	short	GetOperation();
	void	SetOperation(short nOperation);
	void	AddUsed(UINT dwBytes);
	void	SetUsed(UINT dwBytes);
	const char*	GetData();
	BOOL	SetData( const char *pData, UINT nSize );
	UINT	GetUsedLen();

	NOCOPY_CLASS( CIOCPBuffer )
	//// No copies do not implement
	//CIOCPBuffer(const CIOCPBuffer &rhs);
	//CIOCPBuffer &operator=(const CIOCPBuffer &rhs);
	//protected:
public:
	WSAOVERLAPPED	m_OverLapped;
	WSABUF			m_wsaBuf;
	short			m_nOperation;
	UINT			m_nBufSize;
	UINT			m_nBufUsed;
	char			*m_pchBuf;
public:
	sockaddr_in		m_RemoteAddr;
	int				m_nAddrSize;
};

}


