// MGCClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MGCClient.h"

#include "ChargeWnd.h"
#include "DlgTestVideo.h"
#include "VlcWindow.h"
#include "InfoWindow.h"

#include "WindowTip.h"

#include "DemoDlg.h"

#include "MainHelper.h"

#include "SettingMgr.h"

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
	CAppData::SetResourcePath( CAppData::GetInstancePath() + L"mgc/" );

	if( nullptr == SetMgr() )
	{
		CStringW strInfo;
		strInfo.Format( L"���������ļ�%s", CONFIG_INI );
		MessageBox( NULL, strInfo, L"MGCClient", MB_OK|MB_ICONERROR);
		return -1;
	}

	//// ����޷���ȡ�ͷ��绰������δ���ã���web����δͨ
	//CStringW	strPhone = CMainHelper::webServiceNum();
	//if( strPhone.IsEmpty() )
	//{
	//	mgTrace( L"�޷���ȡ�ͷ��绰" );
	//	return -1;
	//}
	//SetMgr()->_strPhone = strPhone;
	
	
	SetMgr()->_strPhone = L"999-888-777";
	


	IdleTrackerInit();


	CDemoDlg		*pMainWnd = new CDemoDlg;

	pMainWnd->Create( NULL, L"MGC Demo", UI_WNDSTYLE_FRAME, 0 );
	//pMainWnd->CenterWindow();
	//pMainWnd->ShowWindow( false );
	::SetWindowPos( pMainWnd->GetHWND(), NULL, GetSystemMetrics( SM_CXSCREEN )-440, 100, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

	CAppData::MessageLoop();
	DEL( pMainWnd );


	////// �������ڲ���
	////CExitTPWindow::Show( ::GetDesktopWindow() );

	////CDlgTestVideo		dlg;
	//////dlg.DoModalNoCaption( NULL );
	////dlg.DoModal( NULL, L"test" );


	//HWND hWndVlc = CVlcWindow::Show( ::GetDesktopWindow() );
	//CWindowTip::Show( hWndVlc );


	//CAppData::MessageLoop();

	//CWindowTip::Hide();
	//CWindowTip::Release();

	//CVlcWindow::Hide();
	//CVlcWindow::Release();


	////CExitTPWindow::Hide();
	////CExitTPWindow::Release();

	IdleTrackerTerm();

	::GdiplusShutdown(token);
	CoUninitialize();

	return 0;
}
