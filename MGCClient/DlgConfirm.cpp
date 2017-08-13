#include "StdAfx.h"
#include "DlgConfirm.h"

#include "SettingMgr.h"


CDlgConfirm::CDlgConfirm(void)
{
}


CDlgConfirm::~CDlgConfirm(void)
{
}

void CDlgConfirm::Init()
{
	CStringW		str;
	str.Format( L"联系客服： %s", SetMgr()->_strPhone );
	FindCtrl( L"lbService" )->SetText( str );
}

LRESULT CDlgConfirm::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	EndModal( 0 );
	return 0;
}

CControlUI* CDlgConfirm::CreateControl( LPCTSTR pstrClass )
{
	return NULL;
}

void CDlgConfirm::Notify( TNotifyUI& msg )
{
	CStringW	strSenderName = msg.pSender->GetName();
	CStringW	strType = msg.sType;

	if( strType == L"click")
	{
		if( strSenderName == L"bnOK" )
		{
			EndModal( IDOK );
		}
		else if( strSenderName == L"bnCancel" )
		{
			EndModal( IDCANCEL );
		}
	}
	else if( strType == L"timer" )
	{
	}
}

LRESULT CDlgConfirm::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
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