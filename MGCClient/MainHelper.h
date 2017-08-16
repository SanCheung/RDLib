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
	static CStringW webServiceNum();

	// api名称：获取用户上机状态
	// 功能描述：获取用户当前的上机状态
	// http://47.92.114.240:8080/rest/status_client?clientId=1
	static int		webStatus_client( maps2s &m );
};


#define  mgTrace	CMainHelper::traceToFile
#define  mgTraceA	CMainHelper::traceToFileA

