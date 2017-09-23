#include "StdAfx.h"
#include "05NetErrorDlg.h"


C05NetErrorDlg::C05NetErrorDlg(void)
{
}


C05NetErrorDlg::~C05NetErrorDlg(void)
{
}

void C05NetErrorDlg::Init()
{
	tryTimeToUI( rand()%5 );

	::ShowWindow( m_hWnd, SW_MAXIMIZE );
}

LRESULT C05NetErrorDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//EndModal( IDCANCEL );
	ShowWindow( false );
	return 0;
}

CControlUI* C05NetErrorDlg::CreateControl( LPCTSTR pstrClass )
{
	return NULL;
}

void C05NetErrorDlg::Notify( TNotifyUI& msg )
{
	CStringW	strSenderName = msg.pSender->GetName();
	CStringW	strType = msg.sType;

	if( strType == L"click")
	{
		if( strSenderName == L"bnOK" )
		{
			EndModal( IDOK );
		}
	}

}

LRESULT C05NetErrorDlg::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return CDialogBase::HandleMessage(uMsg, wParam, lParam);
}

void C05NetErrorDlg::tryTimeToUI( int nTryTime )
{
	CStringW	str;
	str.Format( L"{f 1}正在进行第 {c #ffffff}%d{/c} 次尝试重新连接...{/f}", nTryTime );
	FindCtrl( L"lbInfo" )->SetText( str );
}
