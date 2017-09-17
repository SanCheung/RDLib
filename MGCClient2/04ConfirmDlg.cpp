#include "StdAfx.h"
#include "04ConfirmDlg.h"


C04ConfirmDlg::C04ConfirmDlg(void)
{
}


C04ConfirmDlg::~C04ConfirmDlg(void)
{
}

void C04ConfirmDlg::Init()
{
	//CStringW		str;
	//str.Format( L"联系客服： %s", SetMgr()->_strPhone );
	//FindCtrl( L"lbService" )->SetText( str );
}

LRESULT C04ConfirmDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	EndModal( IDCANCEL );
	return 0;
}

CControlUI* C04ConfirmDlg::CreateControl( LPCTSTR pstrClass )
{
	return NULL;
}

void C04ConfirmDlg::Notify( TNotifyUI& msg )
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

LRESULT C04ConfirmDlg::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
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