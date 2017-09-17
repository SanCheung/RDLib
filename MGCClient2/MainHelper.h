#pragma once
class CMainHelper
{
public:
	CMainHelper(void);
	virtual ~CMainHelper(void);


	static void AddRoundRect( GraphicsPath &gp, INT x, INT y, INT width, INT height, INT cornerX, INT cornerY);
	static void	ShowTrayWndAndStartWnd( bool bShow = true );


	static FILE*	traceFile();
	static void		traceToFile( LPCWSTR format, ... );
	static void		traceToFileA( LPCSTR format, ... );


	static bool	TestTimeOut( int nTimeOut );

	// http://47.92.114.240:8080/rest/serviceNum
	static CStringW web_serviceNum();

	// api���ƣ���ȡ�û��ϻ�״̬
	// ������������ȡ�û���ǰ���ϻ�״̬
	// http://47.92.114.240:8080/rest/status_client?clientId=1
	static int		web_status_client( maps2s &m );

	// api���ƣ��û������ϻ�
	// ���������������û��ϻ�����Ϣ��״̬��״̬��Ϊ�����ϻ���
	// http://47.92.114.240:8080/rest/offline?clientId=1
	static bool		web_offline();

	// api���ƣ��ͻ���״̬�ϱ�
	// �ͻ���״̬��1δ���С�2�����С�
	// ����������ͻ���id���ͻ���״̬ clientId clientStatus
	// http://ip:port/rest/clientStatus  
	static bool		web_clientStatus();

	//	api���ƣ���ȡ����״̬
	//		������������ȡ�û�������״̬�Լ���Ϣ
	//		�������������orderId
	//		���ؽ�������1-���óɹ�{����id��������ʼʱ�䡢��������ʱ�䡢����ʱ���������Ʒѱ�׼�������ͻ���id������״̬��������ʵ��������ʱ��}�����0-����ʧ��
	//�����ַ��http://ip:port/rest/orderStatus
	static bool		web_orderStatus( maps2s &m );

	static int		web_download();

	static int		Reboot();

	static CStringW	 maps2sToString( maps2s &m );

	static bool		RunAsAdmin( LPCWSTR strExe, int nShow = SW_SHOW );

	static BOOL		InstallFont(void);
};


#define  mgTrace	CMainHelper::traceToFile
#define  mgTraceA	CMainHelper::traceToFileA

