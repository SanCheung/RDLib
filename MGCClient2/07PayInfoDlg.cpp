#include "StdAfx.h"
#include "07PayInfoDlg.h"

C07PayInfoDlg::C07PayInfoDlg(void)
{
}


C07PayInfoDlg::~C07PayInfoDlg(void)
{
}

void C07PayInfoDlg::Init()
{
	paint_manager_.SetDefaultLinkFontColor( 0xFF27b5ff );
	paint_manager_.SetDefaultLinkHoverFontColor( 0xFF27b5ff );

	SetTimer( m_hWnd, 1, 1000, nullptr );

	::ShowWindow( m_hWnd, SW_MAXIMIZE );
}

LRESULT C07PayInfoDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	EndModal( IDCANCEL );
	return 0;
}

CControlUI* C07PayInfoDlg::CreateControl( LPCTSTR pstrClass )
{
	return NULL;
}

void C07PayInfoDlg::Notify( TNotifyUI& msg )
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
	else if( strType == L"link" )
	{
		MsgBox( L"重启！" );

	}
}

LRESULT C07PayInfoDlg::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( WM_TIMER == uMsg )
	{
		static int nLeftTime = 180;

		if( nLeftTime <= 0 )
		{
			::KillTimer( m_hWnd, 1 );
			EndModal(1);
			return CDialogBase::HandleMessage(uMsg, wParam, lParam);
		}

		secondToUI( nLeftTime-- );
	}

	return CDialogBase::HandleMessage(uMsg, wParam, lParam);
}

void C07PayInfoDlg::secondToUI( int nSecond )
{
	CStringW	str;
	str.Format( L"{f 0}{c #ffffff}%3d {/c}秒后机器自动重启 {a 1}立即重启{/a} {/f}", nSecond );
	FindCtrl( L"txInfo" )->SetText( str );
}
