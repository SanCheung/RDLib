#pragma once
class CMainHelper
{
public:
	CMainHelper(void);
	virtual ~CMainHelper(void);


	static void AddRoundRect( GraphicsPath &gp, INT x, INT y, INT width, INT height, INT cornerX, INT cornerY);
	static void	ShowTrayWndAndStartWnd( bool bShow = true );

	static void ShowA( HWND hWnd );
	static void ShowB( HWND hWnd );

	static FILE*	traceFile();
	static void		traceToFile( LPCWSTR format, ... );
	static void		traceToFileA( LPCSTR format, ... );


	static bool	TestTimeOut( int nTimeOut );

	// http://47.92.114.240:8080/rest/serviceNum
	static CStringW web_serviceNum();

	// api名称：获取用户上机状态
	// 功能描述：获取用户当前的上机状态
	// http://47.92.114.240:8080/rest/status_client?clientId=1
	static int		web_status_client( maps2s &m );

	// api名称：用户结束上机
	// 功能描述：更改用户上机的信息及状态（状态变为不可上机）
	// http://47.92.114.240:8080/rest/offline?clientId=1
	static bool		web_offline();

	// api名称：客户端状态上报
	// 客户端状态【1未运行、2运行中】
	// 输入参数：客户端id、客户端状态 clientId clientStatus
	// http://ip:port/rest/clientStatus  
	static bool		web_clientStatus();


	static int		web_download();


	static CStringW	 maps2sToString( maps2s &m );
};


#define  mgTrace	CMainHelper::traceToFile
#define  mgTraceA	CMainHelper::traceToFileA

