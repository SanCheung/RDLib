#include "StdAfx.h"
#include "DemoDlg.h"

#include "VlcWindow.h"
#include "WindowTip.h"

#include "InfoWindow.h"
#include "DlgConfirm.h"
#include "ChargeWnd.h"

#include "DlgPayment.h"


CDemoDlg::CDemoDlg(void)
{
}


CDemoDlg::~CDemoDlg(void)
{
}

void CDemoDlg::Init()
{
	::SetTimer( m_hWnd, 1, 1000, NULL );
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
			{
				CVlcWindow::Hide();
			}
			else
			{
				CVlcWindow::Show( m_hWnd );
			}
		}								   
		else if( strSenderName == L"bn2" )
		{
			if( CInfoWindow::IsShow() )
			{
				CInfoWindow::Hide();
			}
			else
			{
				CInfoWindow::Show( m_hWnd );
			}

		}
		else if( strSenderName == L"bn3" )
		{
			CChargeWnd::Show( m_hWnd );
		}
		else if( strSenderName == L"bn4" )
		{
			PostMessage( WM_SHOWA, 4 );
		}
		else if( strSenderName == L"bn5" )
		{
			PostMessage( WM_SHOWA, 5 );
		}			
		else if( strSenderName == L"bn6" )
		{
			/////隐藏任务栏和开始菜单
			HWND hWndST = FindWindow( L"Shell_TrayWnd", NULL );
			HWND hWndStart = FindWindowEx( NULL, NULL, L"Button", NULL );

			int		nNewSW = ::IsWindowVisible(hWndST) ? SW_HIDE:SW_SHOW;
			::ShowWindow( hWndST, nNewSW );
			::ShowWindow( hWndStart, nNewSW );
		}
		else if( strSenderName == L"bn7" )
		{
			//ShowInfo( L"hello world" );
			thStart();
		}
	}
}

LRESULT CDemoDlg::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( WM_TIMER == uMsg )
	{
		//PostMessage( WM_SHOWA, 1 );
		KillTimer( m_hWnd, 1 );
	}
	else if( WM_SHOWA == uMsg )
	{
		if( wParam == 1 )
			CVlcWindow::Show( m_hWnd );
		else if( wParam == 2 )
			CInfoWindow::Show( m_hWnd );
		else if( wParam == 3 )
			CChargeWnd::Show( m_hWnd );
		else if( wParam == 4 )
		{
			CDlgConfirm		dlg;
			dlg.DoModalNoCaption( m_hWnd );
		}
		else if( wParam == 5 )
		{
			CDlgPayment		dlg;
			dlg.DoModalNoCaption( m_hWnd );
		}

	}
	else if( WM_LBUTTONDOWN == uMsg )
	{
		int x = LOWORD( wParam );
		int y = HIWORD( wParam );
	}

	return CDialogBase::HandleMessage(uMsg, wParam, lParam);
}

void CDemoDlg::thread_main()
{
	while( 1 )
	{
		DWORD tNow = ::GetTickCount();
		DWORD tLast = IdleTrackerGetLastTickCount();

		CStringW	str;
		str.Format( L"%d", tNow-tLast );
		ShowInfo( str );

		Sleep(1000);
	}
}

void CDemoDlg::ShowInfo( CStringW strInfo )
{
   CLabelUI	 *lb = FindCtrl<CLabelUI>( L"lb" );
   lb->SetText( strInfo );
}
