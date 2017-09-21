// MGCClient2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MGCClient2.h"

#include "SettingMgr.h"

#include "MainDlg.h"
#include "MainHelper.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	ULONG_PTR token;
	GdiplusStartupInput gsi;
	::GdiplusStartup(&token, &gsi, NULL);

	CAppData::SetInstance(hInstance);
	CAppData::SetResourcePath( CAppData::GetInstancePath() + L"mgc2/" );


	if( nullptr == SetMgr() )
	{
		CStringW strInfo;
		strInfo.Format( L"请检查配置文件%s", CONFIG_INI );
		MessageBox( NULL, strInfo, APPTITLE, MB_OK|MB_ICONERROR);
		return -1;
	}

	mgTraceA( "run!" );

	//// 如果无法获取客服电话，网络未设置，或web服务未通
	//CStringW	strPhone;
	//while(1)
	//{
	//	strPhone = CMainHelper::web_serviceNum();
	//	if( !strPhone.IsEmpty() )
	//		break;

	//	mgTrace( L"无法获取客服电话" );
	//	Sleep(1000);
	//	//MessageBox( GetDesktopWindow(), L"无法获取客服电话，程序将退出！", 0, MB_OK );
	//	//return -1;
	//}

	//SetMgr()->_strPhone = strPhone;

	//CMainHelper::web_download();

	//SetMgr()->_strPhone = L"999-888-777";

	IdleTrackerInit();


	CMainDlg		*pMainWnd = new CMainDlg;

	//pMainWnd->Create( NULL, APPTITLE, WS_VISIBLE|WS_BORDER|WS_OVERLAPPEDWINDOW, WS_EX_TOOLWINDOW );
	pMainWnd->Create( NULL, APPTITLE, WS_VISIBLE|WS_BORDER|WS_OVERLAPPEDWINDOW, 0 );

	//pMainWnd->CenterWindow();
	//pMainWnd->ShowWindow( false );
	::SetWindowPos( pMainWnd->GetHWND(), NULL, GetSystemMetrics( SM_CXSCREEN )-440, 50, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

	//CMainHelper::RunAsAdmin( L"mcGuardian.exe" );

	CAppData::MessageLoop();
	DEL( pMainWnd );


	IdleTrackerTerm();

	mgTraceA( "Quit!" );

	::GdiplusShutdown(token);
	CoUninitialize();

	return 0;
}

