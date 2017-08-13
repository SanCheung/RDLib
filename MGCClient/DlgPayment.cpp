#include "StdAfx.h"
#include "DlgPayment.h"

#include "SettingMgr.h"

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
}

LRESULT CDlgPayment::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	EndModal( 0 );
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

	return CDialogBase::HandleMessage(uMsg, wParam, lParam);
}