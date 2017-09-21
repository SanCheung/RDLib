#include "StdAfx.h"
#include "08NetInterruptDlg.h"
#include "LayeredWindowDrawer.h"


C08NetInterruptDlg::C08NetInterruptDlg(void)
{
}


C08NetInterruptDlg::~C08NetInterruptDlg(void)
{
}

void C08NetInterruptDlg::Init()
{
	SetTimer( m_hWnd, 1, 1000, nullptr );
	//::ShowWindow( m_hWnd, SW_MAXIMIZE );

	CLayeredDialogBase::Init();
}

LRESULT C08NetInterruptDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	EndModal( IDCANCEL );
	return 0;
}

CControlUI* C08NetInterruptDlg::CreateControl( LPCTSTR pstrClass )
{
	return NULL;
}

void C08NetInterruptDlg::Notify( TNotifyUI& msg )
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

LRESULT C08NetInterruptDlg::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( WM_TIMER == uMsg )
	{
		static int nLeftTime = 60;

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

void C08NetInterruptDlg::secondToUI( int nSecond )
{
	CStringW	str;
	str.Format( L"{f 1}{c #27b5ff}%2d{c #999999} 秒后未重新连接{/c}{/f}", nSecond );
	FindCtrl( L"lbInfo" )->SetText( str );
}
