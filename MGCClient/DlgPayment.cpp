#include "StdAfx.h"
#include "DlgPayment.h"

#include "SettingMgr.h"
#include "MainHelper.h"

CDlgPayment::CDlgPayment(void)
{
}


CDlgPayment::~CDlgPayment(void)
{
}

void CDlgPayment::Init()
{
	CStringW		str;
	str.Format( L"联系客服： %s", SetMgr()->_strPhone );
	FindCtrl( L"lbService" )->SetText( str );


	CSettingMgr		*s = SetMgr();

	CStringW		str1, str2, str3;
	int		minute = s->_duration.ToInt();
	if( minute >= 60 )
	{
		if( minute%60 == 0 )
			str1.Format( L"%d小时", minute/60 );
		else
			str1.Format( L"%d小时%d分钟", minute/60, minute%60 );
	}
	else
		str1.Format( L"%d分钟", minute );

	str2.Format( L"%s元/分钟", s->_charging );
	str3.Format( L"%s元", s->_cost );

	FindCtrl( L"lb1" )->SetText( str1 );
	FindCtrl( L"lb2" )->SetText( str2 );
	FindCtrl( L"lb3" )->SetText( str3 );

	// 3分钟 = 180秒
	::SetTimer( m_hWnd, 1, 5000, NULL );
}

LRESULT CDlgPayment::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	EndModal( IDCANCEL );
	return 0;
}

CControlUI* CDlgPayment::CreateControl( LPCTSTR pstrClass )
{
	return NULL;
}

void CDlgPayment::Notify( TNotifyUI& msg )
{
	CStringW	strSenderName = msg.pSender->GetName();
	CStringW	strType = msg.sType;

	if( strType == L"click")
	{
		if( strSenderName == L"bn" )
		{
			MsgBox( L"这是二维码位置，扫描付款后，PC端将重启！目前测试，只是关掉窗口" );
			EndModal( IDOK );
		}
	}
	else if( strType == L"timer" )
	{
	}
}

LRESULT CDlgPayment::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( WM_APP+1 == uMsg )
	{
	}
	else if( WM_LBUTTONDOWN == uMsg )
	{
		int x = LOWORD( wParam );
		int y = HIWORD( wParam );
	}
	else if( WM_TIMER == uMsg )
	{
		//::KillTimer( m_hWnd, 1 );

		//maps2s			m;
		//maps2s_shell	ms( &m );
		//int nRet = CMainHelper::webStatus_client( m );
		//if( nRet > 0 )
		//{
		//	int		onlineStatus = 	ms.intValue("onlineStatus");
		//	//if( onlineStatus == 1 )
		//	{
		//		MessageBox( m_hWnd, L"支付晚，待上机状态!!!\n这种情况下会重启计算机！", L"", 0 );
		//		//Hide();
		//		EndModal( 0 );
		//	}
		//}
	}

	return CDialogBase::HandleMessage(uMsg, wParam, lParam);
}