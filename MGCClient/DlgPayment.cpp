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
	str.Format( L"��ϵ�ͷ��� %s", SetMgr()->_strPhone );
	FindCtrl( L"lbService" )->SetText( str );


	CSettingMgr		*s = SetMgr();

	CStringW		str1, str2, str3;
	int		minute = s->_duration.ToInt();
	if( minute >= 60 )
	{
		if( minute%60 == 0 )
			str1.Format( L"%dСʱ", minute/60 );
		else
			str1.Format( L"%dСʱ%d����", minute/60, minute%60 );
	}
	else
		str1.Format( L"%d����", minute );

	str2.Format( L"%sԪ/����", s->_charging );
	str3.Format( L"%sԪ", s->_cost );

	FindCtrl( L"lb1" )->SetText( str1 );
	FindCtrl( L"lb2" )->SetText( str2 );
	FindCtrl( L"lb3" )->SetText( str3 );


	paint_manager_.SetDefaultLinkFontColor( 0xFFffffff );
	paint_manager_.SetDefaultLinkHoverFontColor( 0xFFffffff );

	//// 3���� = 180��
	m_nLeftSecond = 180;
	::SetTimer( m_hWnd, 1, 1000, NULL );
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
			MsgBox( L"���Ƕ�ά��λ�ã�ɨ�踶���PC�˽�������Ŀǰ���ԣ�ֻ�ǹص�����" );
			EndModal( IDOK );
		}
	}
	else if( strType == L"link" )
	{
		//MsgBox( L"������" );
		//PostQuitMessage( 0 );
		CMainHelper::Reboot();
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
		//{f 1}180����������...{a 1}��������{/a}{/f}
		//::KillTimer( m_hWnd, 1 );

		if( m_nLeftSecond <= 0 )
		{
			::KillTimer( m_hWnd, 1 );
			ShowWindow( false );
			CMainHelper::Reboot();
			PostQuitMessage( 0 );
			return 1;
		}

		CStringW	str;
		str.Format( L"{f 2}%d����������...{a 1}��������{/a}{/f}", m_nLeftSecond );
		FindCtrl( L"txInfo" )->SetText( str );

		m_nLeftSecond--;

		//maps2s			m;
		//maps2s_shell	ms( &m );
		//int nRet = CMainHelper::webStatus_client( m );
		//if( nRet > 0 )
		//{
		//	int		onlineStatus = 	ms.intValue("onlineStatus");
		//	//if( onlineStatus == 1 )
		//	{
		//		MessageBox( m_hWnd, L"֧�������ϻ�״̬!!!\n��������»������������", L"", 0 );
		//		//Hide();
		//		EndModal( 0 );
		//	}
		//}
	}

	return CDialogBase::HandleMessage(uMsg, wParam, lParam);
}