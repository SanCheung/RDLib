#include "StdAfx.h"
#include "DemoDlg.h"

#include "VlcWindow.h"
#include "InfoWindow.h"
#include "DlgConfirm.h"
#include "ExitTPWindow.h"


CDemoDlg::CDemoDlg(void)
{
}


CDemoDlg::~CDemoDlg(void)
{
}

void CDemoDlg::Init()
{

}

LRESULT CDemoDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	PostQuitMessage( 0 );
	return 0;
}

CControlUI* CDemoDlg::CreateControl( LPCTSTR pstrClass )
{
	return NULL;
}

void CDemoDlg::Notify( TNotifyUI& msg )
{
	CStringW	strSenderName = msg.pSender->GetName();
	CStringW	strType = msg.sType;

	if( strType == L"click")
	{
		if( strSenderName == L"bn1" )
		{
			if( CVlcWindow::IsShow() )
				CVlcWindow::Hide();
			else
				CVlcWindow::Show( m_hWnd );
		}								   
		else if( strSenderName == L"bn2" )
		{
			if( CInfoWindow::IsShow() )
				CInfoWindow::Hide();
			else
				CInfoWindow::Show( m_hWnd );
		}
		else if( strSenderName == L"bn3" )
		{
			CExitTPWindow::Show( m_hWnd );
		}
		else if( strSenderName == L"bn4" )
		{
			CDlgConfirm		dlg;
			dlg.DoModalNoCaption( GetDesktopWindow() );
		}
		else if( strSenderName == L"bn5" )
		{
			/////隐藏任务栏和开始菜单
			HWND hWndST = FindWindow( L"Shell_TrayWnd", NULL );
			HWND hWndStart = FindWindowEx( NULL, NULL, L"Button", NULL );

			int		nNewSW = ::IsWindowVisible(hWndST) ? SW_HIDE:SW_SHOW;
			::ShowWindow( hWndST, nNewSW );
			::ShowWindow( hWndStart, nNewSW );
		}
	}
	else if( strType == L"timer" )
	{
	}
}

LRESULT CDemoDlg::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( WM_APP+1 == uMsg )
	{
	}
	else if( WM_LBUTTONDOWN == uMsg )
	{
		int x = LOWORD( wParam );
		int y = HIWORD( wParam );
	}

	return CDialogBase::HandleMessage(uMsg, wParam, lParam);
}
