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

#include "02FloatWnd.h"
#include "04ConfirmDlg.h"

#include "05NetErrorDlg.h"
#include "06AdminLoginDlg.h"

#include "07PayInfoDlg.h"
#include "08NetInterruptDlg.h"

#include "XTask.h"


CMainDlg::CMainDlg(void)
	//: m_pDlgPayment( nullptr )
{
	_pDlgCurrent = nullptr;
}


CMainDlg::~CMainDlg(void)
{
	PATerm();

	for( auto it = _mapDlgs.begin(); it != _mapDlgs.end(); ++it )
	{
		delete it->second;
	}

	_mapDlgs.clear();
}

void CMainDlg::Init()
{
	::SetTimer( m_hWnd, 1, 1000, NULL );
	thStart();

	CControlUI	*pCtrl = FindCtrl(L"bn3");
	pCtrl->OnEvent += MakeDelegate(this, &CMainDlg::OnChildEvent);

	_task1.reset( new XTask );
	_task2.reset( new XTask );

	
	_mapDlgs.insert( make_pair( 1, new C01ScanEnterDlg ) );
	_mapDlgs.insert( make_pair( 2, new C02GamePadDlg ) );
	_mapDlgs.insert( make_pair( 4, new C04ConfirmDlg ) );
	_mapDlgs.insert( make_pair( 5, new C05NetErrorDlg ) );
	_mapDlgs.insert( make_pair( 6, new C06AdminLoginDlg ) );
	_mapDlgs.insert( make_pair( 7, new C07PayInfoDlg ) );
	_mapDlgs.insert( make_pair( 8, new C08NetInterruptDlg ) );

	for( auto it = _mapDlgs.begin(); it != _mapDlgs.end(); ++it )
	{
		CDialogBase	*pDlg = it->second;
		pDlg->Create( m_hWnd, L"", WS_POPUPWINDOW, WS_EX_WINDOWEDGE );
		pDlg->ShowWindow( false );
	}
}

bool CMainDlg::OnChildEvent( void* paramters )
{
	TEventUI* pEvent = (TEventUI*)paramters;	
	//if (pEvent->Type > UIEVENT__MOUSEBEGIN && pEvent->Type < UIEVENT__MOUSEEND)
	if( pEvent->Type == UIEVENT_MOUSEHOVER )
	{
		CControlUI	*pCtrl = FindCtrl(L"bn3");
		pCtrl->GetManager()->SendNotify( pCtrl, L"mouse_hover_on_button");
	}

	return true;
}

LRESULT CMainDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	thShutdownUseEvent();

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
			//m_nCurrentPage = 1;

			//if( C01WndScanEnter::IsShow() )
			//	C01WndScanEnter::Hide();
			//else
			//	C01WndScanEnter::Show( m_hWnd );

			//C01ScanEnterDlg	dlg;
			//dlg.DoModalNoCaption( m_hWnd );
			
			//onDlgShowModal(1);
			onDlgShow(1);

			C01ScanEnterDlg *pDlg = (C01ScanEnterDlg *)_mapDlgs[1];
			pDlg->setQRCode( L"download\\QR.jpg");
		}								   
		else if( strSenderName == L"bn2" )
		{
			//C02GamePadDlg		dlg;
			//dlg.DoModalNoCaption( m_hWnd );

			//onDlgShowModal(2);
			onDlgShow(2);
		}
		else if( strSenderName == L"bn3" )
		{
			MsgBox( L"Call sesame!");
		}
		else if( strSenderName == L"bn4" )
		{
			//C04ConfirmDlg		dlg;
			//dlg.DoModalNoCaption( m_hWnd );

			//dlg.Create( m_hWnd, L"", WS_POPUP|WS_MAXIMIZE, WS_EX_LAYERED );
			//dlg.ShowModal();

			//onDlgShowModal(4);
			onDlgShow(4);
		}
		else if( strSenderName == L"bn5" )
		{
			//C05NetErrorDlg		dlg;
			//dlg.DoModalNoCaption( m_hWnd );
			
			//onDlgShowModal(5);
			onDlgShow(5);
		}			
		else if( strSenderName == L"bn6" )
		{
			//C06AdminLoginDlg	dlg;
			//dlg.DoModalNoCaption( m_hWnd );

			//onDlgShowModal(6);
			onDlgShow(6);
		}
		else if( strSenderName == L"bn7" )
		{
			//C07PayInfoDlg		dlg;
			//dlg.DoModalNoCaption( m_hWnd );

			//onDlgShowModal(7);
			onDlgShow(7);
		}
		else if( strSenderName == L"bn8" )
		{
			//C08NetInterruptDlg dlg;
			//dlg.DoModalNoCaption( m_hWnd );
			////dlg.Create( m_hWnd, L"", WS_POPUP|WS_MAXIMIZE, WS_EX_LAYERED );
			////dlg.ShowModal();

			//onDlgShowModal(8);
			onDlgShow(8);
		}
		else if( strSenderName == L"bnTask1" )
		{
			_task1->startTask( TASKBINDFN( CMainDlg::fnTask1 ) );
		}	
		else if( strSenderName == L"bnTask2" )
		{
			_task2->startTask( TASKBINDFN( CMainDlg::fnTask2 ) );
		}
	}
	else if( L"mouse_hover_on_button" == strType )
	{
		ATLTRACE( L"hover\n" );
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

void CMainDlg::fnTask1()
{
	int i = 0;
	while(1)
	{
		if( WAIT_OBJECT_0 == _task1->thWaitEvent( 5000 ) )
			break;

		//ATLTRACE( L"task1 %d\n", i++ );

		onDlgShowModal( 5 );
	}
}

void CMainDlg::fnTask2()
{
	int i = 0;
	while(1)
	{
		if( WAIT_OBJECT_0 == _task1->thWaitEvent( 3000 ) )
			break;

		ATLTRACE( L"task2 %d\n", i++ );
	}
}

void CMainDlg::onDlgShowModal( int id )
{
	auto	it = _mapDlgs.find( id );
	if( it == _mapDlgs.end() )
		return;

	CDialogBase *pDlg = it->second;

	if( _pDlgCurrent != pDlg )
	{
		if( _pDlgCurrent != nullptr )
			_pDlgCurrent->EndModal( 0 );

		_pDlgCurrent = pDlg;

		pDlg->Create( m_hWnd, L"", WS_POPUPWINDOW, WS_EX_WINDOWEDGE );
		pDlg->ShowModal();
		_pDlgCurrent = nullptr;
	}
}

void CMainDlg::onDlgShow( int id )
{
	auto	it = _mapDlgs.find( id );
	if( it == _mapDlgs.end() )
		return;

	CDialogBase *pDlg = it->second;

	if( _pDlgCurrent != pDlg || 
		(_pDlgCurrent != nullptr && !::IsWindowVisible(_pDlgCurrent->GetHWND()) )  )
	{
		if( _pDlgCurrent != nullptr )
		{
			_pDlgCurrent->ShowWindow( false );
		}

		_pDlgCurrent = pDlg;
		pDlg->ShowWindow( true );
	}
}
