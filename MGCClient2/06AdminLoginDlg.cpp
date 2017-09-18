#include "StdAfx.h"
#include "06AdminLoginDlg.h"


C06AdminLoginDlg::C06AdminLoginDlg(void)
{
}


C06AdminLoginDlg::~C06AdminLoginDlg(void)
{
}

void C06AdminLoginDlg::Init()
{
	::ShowWindow( m_hWnd, SW_MAXIMIZE );
}

LRESULT C06AdminLoginDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	EndModal( IDCANCEL );
	return 0;
}

CControlUI* C06AdminLoginDlg::CreateControl( LPCTSTR pstrClass )
{
	return NULL;
}

void C06AdminLoginDlg::Notify( TNotifyUI& msg )
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

LRESULT C06AdminLoginDlg::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
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