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
	str.Format( L"��ϵ�ͷ��� %s", SetMgr()->_strPhone );
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
			MsgBox( L"���Ƕ�ά��λ�ã�ɨ�踶���PC�˽�������Ŀǰ���ԣ�ֻ�ǹص�����" );
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