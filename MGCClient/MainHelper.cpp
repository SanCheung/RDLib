#include "StdAfx.h"
#include "MainHelper.h"

#include "VlcWindow.h"
#include "InfoWindow.h"

#include "Helper.h"
#include "SettingMgr.h"


CMainHelper::CMainHelper(void)
{
}


CMainHelper::~CMainHelper(void)
{
}


FILE* CMainHelper::traceFile()
{
	setlocale(LC_ALL, "");

	CStringW	strPath = CHelper::GetCurrentPath();
	CStringA	strPathA = strPath;
	strPathA += LOG_FILE;

	FILE		*f = NULL;
	errno_t		err = fopen_s( &f, strPathA, "a+" );
	if( 0 != err )
		return NULL;

	struct tm		tmNow = {0};
	time_t			now;
	time( &now );
	localtime_s( &tmNow, &now );

	fprintf( f, "[%02d-%02d-%02d %02d:%02d:%02d]", (tmNow.tm_year+1900)%1000, tmNow.tm_mon+1, tmNow.tm_mday,
		tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec );

	return f;
}

void CMainHelper::traceToFileA( LPCSTR format, ... )
{
	//#ifdef DEBUG

	FILE *f = traceFile();
	if( f == NULL )
		return;

	va_list		arglist;
	int			nRet;

	va_start(arglist, format);
	nRet = vfprintf( f, format, arglist );
	va_end(arglist);

	fprintf( f, "\n" );
	fclose( f );

	//#endif

}


void CMainHelper::traceToFile( LPCWSTR format, ... )
{
	//#ifdef DEBUG

	FILE *f = traceFile();
	if( f == NULL )
		return;

	va_list		arglist;
	int			nRet;

	va_start(arglist, format);
	nRet = vfwprintf( f, format, arglist );
	va_end(arglist);

	fprintf( f, "\n" );
	fclose( f );
	//#endif
}

void CMainHelper::AddRoundRect( GraphicsPath &gp, INT x, INT y, INT width, INT height, INT cornerX, INT cornerY )
{
	INT elWid = 2*cornerX;  
	INT elHei = 2*cornerY;  

	gp.AddArc(x,y,elWid,elHei,180,90); // ◊Û…œΩ«‘≤ª°  
	gp.AddLine(x+cornerX,y,x+width-cornerX,y); // …œ±ﬂ  

	gp.AddArc(x+width-elWid,y, elWid,elHei,270,90); // ”“…œΩ«‘≤ª°  
	gp.AddLine(x+width,y+cornerY, x+width,y+height-cornerY);// ”“±ﬂ  

	gp.AddArc(x+width-elWid,y+height-elHei, elWid,elHei,0,90); // ”“œ¬Ω«‘≤ª°  
	gp.AddLine(x+width-cornerX,y+height, x+cornerX,y+height); // œ¬±ﬂ  

	gp.AddArc(x,y+height-elHei, elWid,elHei,90,90);   
	gp.AddLine(x,y+cornerY, x, y+height-cornerY); 
}

void CMainHelper::ShowTrayWndAndStartWnd( bool bShow )
{
	HWND hWndST = FindWindow( L"Shell_TrayWnd", NULL );
	HWND hWndStart = FindWindowEx( NULL, NULL, L"Button", NULL );

	int nCmd = bShow ? SW_SHOW : SW_HIDE;
	::ShowWindow( hWndST, nCmd );
	::ShowWindow( hWndStart, nCmd );
}

void CMainHelper::ShowA( HWND hWnd )
{
	if( CVlcWindow::IsShow() )
		return;

	CVlcWindow::Show( hWnd );
}

void CMainHelper::ShowB( HWND hWnd )
{
	if( CInfoWindow::IsShow() )
		return;

	CInfoWindow::Show( hWnd );
}

bool CMainHelper::TestTimeOut( int nTimeOut )
{
	DWORD tNow = ::GetTickCount();
	DWORD tLast = IdleTrackerGetLastTickCount();

	if( tNow - tLast < (DWORD)nTimeOut )
		return false;

	return true;
}

CStringW CMainHelper::web_serviceNum()
{
	CStringA	strWeb = (CStringA)SetMgr()->_strWeb;
	strWeb += "/rest/serviceNum";

	maps2s			m;
	int nRes = urlGetInfo( (string)strWeb, m );

	maps2s_shell	ms( &m );
	int data = ms.intValue( "data" );
	if( data == 0 )
		return L"";

	return (CStringW)ms.stringValue( "phone" ).c_str();
}

int CMainHelper::web_status_client( maps2s &m )
{
	CStringA	strWeb = (CStringA)SetMgr()->_strWeb;
	int			id = SetMgr()->_strClientID;

	// http://47.92.114.240:8080/rest/status_client?clientId=1
	CStringA	strURL;
	strURL.Format( "%s/rest/status_client?clientId=%d", strWeb, id );

	int nRet = urlGetInfo( (string)strURL, m );
	return nRet;
}


bool CMainHelper::web_clientStatus()
{
	CStringA	strWeb = (CStringA)SetMgr()->_strWeb;
	int			id = SetMgr()->_strClientID;

	// http://47.92.114.240:8080/rest/clientStatus?clientId=1&clientStatus=2
	CStringA	strURL;
	strURL.Format( "%s/rest/clientStatus?clientId=%d&clientStatus=2", strWeb, id );

	maps2s			m;
	int nRes = urlGetInfo( (string)strWeb, m );

	maps2s_shell	ms( &m );
	int data = ms.intValue( "data" );
	if( data == 0 )
	{
		mgTrace( L"web clientStatus fail. %s", maps2sToString(m) );
		return false;
	}

	return true;
}


bool CMainHelper::web_offline()
{
	CStringA	strWeb = (CStringA)SetMgr()->_strWeb;
	int			id = SetMgr()->_strClientID;

	// http://47.92.114.240:8080/rest/offline?clientId=1
	CStringA	strURL;
	strURL.Format( "%s/rest/offline?clientId=%d", strWeb, id );

	maps2s			m;
	int nRes = urlGetInfo( (string)strWeb, m );

	maps2s_shell	ms( &m );
	int data = ms.intValue( "data" );
	if( data == 0 )
	{
		mgTrace( L"web offline fail. %s", maps2sToString(m) );
		return false;
	}

	return true;
}

CStringW CMainHelper::maps2sToString( maps2s &m )
{
	CStringW	str;
	for( auto it = m.begin(); it != m.end(); ++it )
	{
		CStringW strKV;
		strKV.Format( L"%s:%s\n", it->first.c_str(), it->second.c_str() );
		str += strKV;
	}

	return str;
}
