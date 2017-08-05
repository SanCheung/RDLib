// MGCClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MGCClient.h"

#include "ExitTPWindow.h"
#include "DlgTestVideo.h"
#include "VlcWindow.h"


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

	//// ¸¡¶¯´°¿Ú²âÊÔ
	//CExitTPWindow::Show( ::GetDesktopWindow() );

	//CDlgTestVideo		dlg;
	////dlg.DoModalNoCaption( NULL );
	//dlg.DoModal( NULL, L"test" );


	CVlcWindow::Show( ::GetDesktopWindow() );


	CAppData::MessageLoop();

	CVlcWindow::Hide();
	CVlcWindow::Release();


	//CExitTPWindow::Hide();
	//CExitTPWindow::Release();

	::GdiplusShutdown(token);
	CoUninitialize();

	return 0;
}
