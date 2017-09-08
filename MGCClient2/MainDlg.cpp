#include "StdAfx.h"
#include "MainDlg.h"

//#include "VlcWindow.h"
//#include "WindowTip.h"
//
//#include "InfoWindow.h"
//#include "DlgConfirm.h"
//#include "ChargeWnd.h"
//#include "DlgPayment.h"


#include "SettingMgr.h"
#include "MainHelper.h"

#include "01WndScanEnter.h"
#include "02GamePadDlg.h"


CMainDlg::CMainDlg(void)
	//: m_pDlgPayment( nullptr )
{
}


CMainDlg::~CMainDlg(void)
{
	PATerm();
}

void CMainDlg::Init()
{
	::SetTimer( m_hWnd, 1, 1000, NULL );
	thStart();
}

LRESULT CMainDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	thShutdownUseEvent();

	//if( m_pDlgPayment != nullptr && IsWindow( m_pDlgPayment->GetHWND() ) )
	//{
	//	DestroyWindow( m_pDlgPayment->GetHWND() );
	//	DEL( m_pDlgPayment );
	//}

	PostQuitMessage( 0 );
	return 0;
}

CControlUI* CMainDlg::CreateControl( LPCTSTR pstrClass )
{
	return NULL;
}

void CMainDlg::Notify( TNotifyUI& msg )
{
	CStringW	strSenderName = msg.pSender->GetName();
	CStringW	strType = msg.sType;

	if( strType == L"click")
	{
		if( strSenderName == L"bn1" )
		{
			m_nCurrentPage = 1;

			if( C01WndScanEnter::IsShow() )
				C01WndScanEnter::Hide();
			else
				C01WndScanEnter::Show( m_hWnd );

			//if( CVlcWindow::IsShow() )
			//{
			//	CVlcWindow::Hide();
			//}
			//else
			//{
			//	CVlcWindow::Show( m_hWnd );
			//}
		}								   
		else if( strSenderName == L"bn2" )
		{
			C02GamePadDlg		dlg;
			dlg.DoModal( m_hWnd, L"" );
		}
		else if( strSenderName == L"bn3" )
		{
			//CChargeWnd::Show( m_hWnd );
		}
		else if( strSenderName == L"bn4" )
		{
			//PostMessage( WM_SHOWA, 4 );
		}
		else if( strSenderName == L"bn5" )
		{
			//PostMessage( WM_SHOWA, 5 );
		}			
		else if( strSenderName == L"bn6" )
		{
			///////隐藏任务栏和开始菜单
			//HWND hWndST = FindWindow( L"Shell_TrayWnd", NULL );
			//HWND hWndStart = FindWindowEx( NULL, NULL, L"Button", NULL );

			//int		nNewSW = ::IsWindowVisible(hWndST) ? SW_HIDE:SW_SHOW;
			//::ShowWindow( hWndST, nNewSW );
			//::ShowWindow( hWndStart, nNewSW );
		}
		else if( strSenderName == L"bn7" )
		{
			//ShowInfo( L"hello world" );
			thStart();
		}
	}
}

LRESULT CMainDlg::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( WM_TIMER == uMsg )
	{
		//m_nCurrentPage = 1;
		//CVlcWindow::Show( m_hWnd );

		//PAInit();

		////// 单独测试计费窗口
		////CChargeWnd::Show( m_hWnd );

		KillTimer( m_hWnd, 1 );
	}
	else if( WM_SHOW_WINDOW == uMsg )
	{
		//if( wParam == 1 && m_nCurrentPage == 2 )
		//	CVlcWindow::Show( m_hWnd );
		//else if( wParam == 2 && m_nCurrentPage == 1 )
		//	CInfoWindow::Show( m_hWnd );
		//else if( wParam == 3 )
		//{
		//	PATerm();
		//	CChargeWnd::Show( m_hWnd );
		//}
		//else if( wParam == 4 )
		//{
		//	//CDlgConfirm		dlg;
		//	//dlg.DoModalNoCaption( GetDesktopWindow() );
		//}
		//else if( wParam == 5 )
		//{
		//	//CDlgPayment		dlg;
		//	//dlg.DoModalNoCaption( GetDesktopWindow() );

		//	if( nullptr == m_pDlgPayment )
		//	{
		//		m_pDlgPayment = new CDlgPayment;
		//		m_pDlgPayment->Create( GetDesktopWindow(), L"", WS_POPUPWINDOW, WS_EX_WINDOWEDGE );
		//		m_pDlgPayment->CenterWindow();
		//	}

		//	m_pDlgPayment->ShowWindow();
		//}

		////m_nCurrentPage = wParam;
		//::InterlockedExchange( (LPLONG)&m_nCurrentPage, wParam );
	}
	else if( WM_LBUTTONDOWN == uMsg )
	{
		int x = LOWORD( wParam );
		int y = HIWORD( wParam );
	}
	else if( WM_CLOSE == uMsg )
	{
		int i = 0;
	}

	return CDialogBase::HandleMessage(uMsg, wParam, lParam);
}

void CMainDlg::thread_main()
{
	////while( 1 )
	////{
	////	///原来用于测试Idle时间
	////	//DWORD tNow = ::GetTickCount();
	////	//DWORD tLast = IdleTrackerGetLastTickCount();

	////	//CStringW	str;
	////	//str.Format( L"%d", tNow-tLast );
	////	//ShowInfo( str );

	////	//Sleep(1000);

	////}


	//maps2s			m;
	//maps2s_shell	ms( &m );
	//CSettingMgr		*s = SetMgr();

	//int			nTime = 0;
	//while(1)
	//{
	//	if( m_nCurrentPage == 5 )
	//	{
	//		int nRet = CMainHelper::web_orderStatus( m );
	//		if( nRet <= 0 || 0 == ms.intValue("data") )
	//		{
	//			mgTrace( L"获取订单状态 orderStatus fail! %d", nRet );
	//		}
	//		else
	//		{
	//			if( 1 == ms.intValue("orderStatus") )
	//			{
	//				CMainHelper::Reboot();
	//				break;
	//			}
	//		}

	//		// 5s轮询一次
	//		if( WAIT_OBJECT_0 == thWaitEvent(5000) )
	//			break;
	//	}
	//	else
	//	{
	//		int nRet = CMainHelper::web_status_client( m );
	//		if( nRet <= 0 || 0 == ms.intValue("data") )
	//		{
	//			mgTrace( L"获取上机状态 status_client fail! %d", nRet );
	//		}
	//		else
	//		{
	//			s->_duration = ms.stringValue( "duration" ).c_str();
	//			s->_charging = ms.stringValue( "charging" ).c_str();
	//			s->_cost = ms.stringValue( "cost" ).c_str();
	//			int	 onlineStatus = ms.intValue("onlineStatus");

	//			if( onlineStatus == 2 )
	//			{
	//				if( m_nCurrentPage == 1 )
	//				{
	//					PostMessage( WM_SHOWA, 3 );
	//					CVlcWindow::Hide();
	//					CInfoWindow::Hide();
	//				}
	//				else if( m_nCurrentPage == 2 )
	//				{
	//					PostMessage( WM_SHOWA, 3 );
	//					CVlcWindow::Hide();
	//					CInfoWindow::Hide();
	//				}
	//			}
	//			else if( m_nCurrentPage == 3 )
	//			{
	//				CChargeWnd::Hide();
	//				CMainHelper::Reboot();
	//			}
	//		}

	//		if( nTime >= 10 )
	//		{
	//			nTime = 0;
	//			// 每隔20s上报一次客户端状态
	//			CMainHelper::web_clientStatus();
	//		}

	//		nTime++;

	//		// 两秒轮询一次
	//		if( WAIT_OBJECT_0 == thWaitEvent(2000) )
	//			break;
	//	}
	//}
}

void CMainDlg::ShowInfo( CStringW strInfo )
{
   CLabelUI	 *lb = FindCtrl<CLabelUI>( L"lb" );
   lb->SetText( strInfo );
}
