#include "StdAfx.h"
#include "SwhMainLayout.h"


CSwhMainLayout::CSwhMainLayout(void)
{
}


CSwhMainLayout::~CSwhMainLayout(void)
{
}

CControlUI* CSwhMainLayout::CreateControl( LPCTSTR pstrClass )
{
	return NULL;
}

LRESULT CSwhMainLayout::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	PostQuitMessage( 0 );
	return 0;
}

void CSwhMainLayout::Init()
{
}

void CSwhMainLayout::Notify( TNotifyUI& msg )
{
	CStringW	strSenderName = msg.pSender->GetName();
	CStringW	strType = msg.sType;

	if( strType == L"click")
	{
		if( strSenderName == L"bn1" )
		{
			PostQuitMessage( 1 );
		}
		else if( strSenderName == L"bnLeftThumb" )
		{
			CVerticalLayoutUI *pVL = FindCtrl<CVerticalLayoutUI>( L"vlLeftBar" );
			pVL->SetVisible( !pVL->IsVisible() );
		}
	}
	else if( strType == L"selectchanged" )
	{
		CCheckBoxUI *pCk = FindCtrl<CCheckBoxUI>( L"bnL2" );
		CVerticalLayoutUI *pVL = FindCtrl<CVerticalLayoutUI>( L"vlL2Sub" );
		pVL->SetVisible( pCk->IsCheck() );
	}
}