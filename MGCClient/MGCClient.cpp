// MGCClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MGCClient.h"

#include "ExitTPWindow.h"
#include "DlgTestVideo.h"
#include "VlcWindow.h"
#include "InfoWindow.h"

#include "WindowTip.h"

#include "DemoDlg.h"

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

	CDemoDlg		*pMainWnd = new CDemoDlg;

	pMainWnd->Create( NULL, L"MGC Demo", UI_WNDSTYLE_FRAME, 0 );
	//pMainWnd->CenterWindow();
	::SetWindowPos( pMainWnd->GetHWND(), NULL, GetSystemMetrics( SM_CXSCREEN )-300, 100, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

	CAppData::MessageLoop();
	DEL( pMainWnd );


	////// ¸¡¶¯´°¿Ú²âÊÔ
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

	::GdiplusShutdown(token);
	CoUninitialize();

	return 0;
}
