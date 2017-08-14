#pragma once

//#include "UserItem.h"
//#include "AntPackageParser.h"
#include "IOCPBuffer.h"
#include "Lock.h"
#include "Package.h"

#include <list>
using namespace std;

using namespace winapi;

#define	MAX_DATA_LENGTH		(4096)

class CTCPServer;
//class CMeetingItem;

#define CONNECTION_STATE_NONE		(0) // 初始状态
#define CONNECTION_STATE_CONNECTED	(1) // 表示已经连接成功
#define CONNECTION_STATE_VERIFIED	(2) // 表示此链接经过了验证
#define CONNECTION_STATE_CLOSING	(3) // 表示连接将要被关闭
#define CONNECTION_STATE_CLOSED		(4) // 表示连接将已经关闭

class CAntConnection
{
public:
	CAntConnection(void);
	virtual ~CAntConnection(void);

public:
	void	SetSocket(SOCKET socket);
	void	SetTCPServer(CTCPServer *pServer);
	CTCPServer* GetServer();
	void	SetMeetingID(UINT meetingID);
	UINT	GetMeetingID();
	void	SetSessionID( UINT sessionID );
	UINT	GetSessionID();

	//void	SetUser( Meeting::CUserItem& user );
	UINT	GetUserID();


	BOOL	ReqSendBuffer(CIOCPBuffer *pBuffer);

	BOOL	ReqSendData(const char *pData, int nSize);	

	BOOL	ReqRecvData(CIOCPBuffer *pBuffer);

	BOOL	OnSendCompleted();

	BOOL	OnReceivedCompleted(CIOCPBuffer *pBuffer);

	void	PrepareClose();
	int		DoClose();

	bool IsVerified();
	void SetVerified(bool verifyed);

	bool IsVerifying();
	void SetVerifying( bool b );

protected:
	LONG	IncreaseIOPending();
	LONG	DecreaseIOPending();

	BOOL	ProcessPackage(CAntPackagePtr& package, char *pchData, int nSize);
	int		ParseData();


public:
	//Meeting::CUserItem			m_UserItem; // 此连接的用户信息

protected:
	SOCKET				m_socket;

	CTCPServer			*m_pServer;

	CBuffer				m_DataCache; // 在数据被解析前缓存
	int					m_nValidDataLen; // 有效数据长度,-1 表示未赋值
	list<CIOCPBuffer*>	m_listSendData; // 保存需要发送的数据
	int					m_nState; // 连接状态
	volatile LONG		m_nIOPending; // 当前正在处理的IO
	DWORD				m_dwKeepAlive; // 最后成功接收数据或者发送数据的时间戳

	CCriticalSection	m_LockSocketStatus; 
	int					m_CloseRetryCount;
	bool				m_IsReaping;
	bool				m_IsDoClosing;
	
	bool				m_Verifying;
	bool				m_Verified;
	UINT				m_MeetingID;
	UINT				m_SessionID;

	char				m_addr[32];

};

