#include "StdAfx.h"
#include "DemoDlg.h"

#include "VlcWindow.h"
#include "WindowTip.h"

#include "InfoWindow.h"
#include "DlgConfirm.h"
#include "ChargeWnd.h"

#include "DlgPayment.h"
#include "SettingMgr.h"
#include "MainHelper.h"


CDemoDlg::CDemoDlg(void)
	: m_pDlgPayment( nullptr )
{
}


CDemoDlg::~CDemoDlg(void)
{
}

void CDemoDlg::Init()
{
	::SetTimer( m_hWnd, 1, 1000, NULL );
	thStart();
}

LRESULT CDemoDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	thShutdownUseEvent();

	if( m_pDlgPayment != nullptr && IsWindow( m_pDlgPayment->GetHWND() ) )
	{
		DestroyWindow( m_pDlgPayment->GetHWND() );
		DEL( m_pDlgPayment );
	}

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
			/////�����������Ϳ�ʼ�˵�
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
			//CDlgConfirm		dlg;
			//dlg.DoModalNoCaption( GetDesktopWindow() );
		}
		else if( wParam == 5 )
		{
			//CDlgPayment		dlg;
			//dlg.DoModalNoCaption( GetDesktopWindow() );

			if( nullptr == m_pDlgPayment )
			{
				m_pDlgPayment = new CDlgPayment;
				m_pDlgPayment->Create( GetDesktopWindow(), L"", WS_POPUPWINDOW, WS_EX_WINDOWEDGE );
				m_pDlgPayment->CenterWindow();
			}

			m_pDlgPayment->ShowWindow();
		}

		m_nCurrentPage = wParam;
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
	//while( 1 )
	//{
	//	///ԭ�����ڲ���Idleʱ��
	//	//DWORD tNow = ::GetTickCount();
	//	//DWORD tLast = IdleTrackerGetLastTickCount();

	//	//CStringW	str;
	//	//str.Format( L"%d", tNow-tLast );
	//	//ShowInfo( str );

	//	//Sleep(1000);

	//}


	maps2s			m;
	maps2s_shell	ms( &m );
	CSettingMgr		*s = SetMgr();

	int			nTime = 0;
	while(1)
	{
		int nRet = CMainHelper::web_status_client( m );
		if( nRet <= 0 || 0 == ms.intValue("data") )
		{
			mgTrace( L"webStatus_client fail! %d", nRet );
		}
		else
		{
			s->_duration = ms.stringValue( "duration" ).c_str();
			s->_charging = ms.stringValue( "charging" ).c_str();
			s->_cost = ms.stringValue( "cost" ).c_str();
			int	 onlineStatus = ms.intValue("onlineStatus");

			if( onlineStatus == 2 )
			{
				if( m_nCurrentPage == 1 )
				{
					CVlcWindow::Hide();
					PostMessage( WM_SHOWA, 3 );
				}
				else if( m_nCurrentPage == 2 )
				{
					CInfoWindow::Hide();
					PostMessage( WM_SHOWA, 3 );
				}	
			}
			else if( m_nCurrentPage == 3 )
			{
				MessageBox( m_hWnd, L"���ϻ���!!!\n��ʽ�棬��������»������������", L"", 0 );
				CChargeWnd::Hide();
			}
			else if( m_nCurrentPage == 5 && onlineStatus == 1 )
			{
				MessageBox( m_hWnd, L"���ϻ���!!!\n��ʽ�棬��������»������������", L"", 0 );
				m_pDlgPayment->ShowWindow( false );
			}
		}


		if( nTime >= 10 )
		{
			nTime = 0;
			CMainHelper::web_clientStatus();
		}

		nTime++;

		// ������ѯһ��
		if( WAIT_OBJECT_0 == thWaitEvent(2000) )
			break;

	}
}

void CDemoDlg::ShowInfo( CStringW strInfo )
{
   CLabelUI	 *lb = FindCtrl<CLabelUI>( L"lb" );
   lb->SetText( strInfo );
}
