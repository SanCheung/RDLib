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

#define CONNECTION_STATE_NONE		(0) // ��ʼ״̬
#define CONNECTION_STATE_CONNECTED	(1) // ��ʾ�Ѿ����ӳɹ�
#define CONNECTION_STATE_VERIFIED	(2) // ��ʾ�����Ӿ�������֤
#define CONNECTION_STATE_CLOSING	(3) // ��ʾ���ӽ�Ҫ���ر�
#define CONNECTION_STATE_CLOSED		(4) // ��ʾ���ӽ��Ѿ��ر�

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
	//Meeting::CUserItem			m_UserItem; // �����ӵ��û���Ϣ

protected:
	SOCKET				m_socket;

	CTCPServer			*m_pServer;

	CBuffer				m_DataCache; // �����ݱ�����ǰ����
	int					m_nValidDataLen; // ��Ч���ݳ���,-1 ��ʾδ��ֵ
	list<CIOCPBuffer*>	m_listSendData; // ������Ҫ���͵�����
	int					m_nState; // ����״̬
	volatile LONG		m_nIOPending; // ��ǰ���ڴ����IO
	DWORD				m_dwKeepAlive; // ���ɹ��������ݻ��߷������ݵ�ʱ���

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

