#include "StdAfx.h"
#include "SwhMainLayout.h"
#include "DlgTestList.h"


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
	m_pVLHandle = FindCtrl<CVerticalLayoutUI>( L"vlHandle" );
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
		else if( strSenderName == L"bnLeftHandle" )
		{
			CVerticalLayoutUI *pVL = FindCtrl<CVerticalLayoutUI>( L"vlLeftBar" );
			
			bool	bShow = !pVL->IsVisible();
			pVL->SetVisible( bShow );

			int x = bShow?pVL->GetWidth():0;
			int y = pVL->GetY();
			int w = 32;
			int h = pVL->GetHeight();
			RECT	rt = { x, y, x+w, y+h };
			m_pVLHandle->SetPos( rt );
		}
		else if( strSenderName == L"bn2" ) 
		{
			CDlgTestList dlg;
			dlg.DoModal( GetHWND(), L"hello" );
		}
	}
	else if( strType == L"selectchanged" )
	{
		CCheckBoxUI *pCk = FindCtrl<CCheckBoxUI>( L"bnL2" );
		CVerticalLayoutUI *pVL = FindCtrl<CVerticalLayoutUI>( L"vlL2Sub" );
		pVL->SetVisible( pCk->IsCheck() );
	}
	else if( strType == L"itemselect" )
	{
		CComboUI		*pCb = (CComboUI *)msg.pSender;
		
		CStringW		str = L"....";

		int nIndex = pCb->GetCurSel();
		if( nIndex >= 0 && nIndex < pCb->GetCount() )
			str = pCb->GetItemAt(nIndex)->GetText();
		
		MsgBox( str );
		OutputDebugString( str+L"-----------\n" );
	}
}