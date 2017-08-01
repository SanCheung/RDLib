#include "StdAfx.h"
#include "SwhMainLayout.h"
#include "DlgTestList.h"
#include "BarCodeOcxUI.h"
#include "IconPathViewD2UI.h"
#include "D2ViewUI.h"
#include "ExitTPWindow.h"

CSwhMainLayout::CSwhMainLayout(void)
{
}


CSwhMainLayout::~CSwhMainLayout(void)
{
}

CControlUI* CSwhMainLayout::CreateControl( LPCTSTR pstrClass )
{
	if( EQUSTR( pstrClass, L"ocx" ) )
	{
		m_ocx = CBarCodeOcxUI::CreateUI( m_hWnd );
		return m_ocx->GetHostUI();
	}
	else if( EQUSTR( pstrClass, L"IconPathView") )
	{
		m_pIPV = CIconPathViewD2UI::CreateUI( m_hWnd );
		return m_pIPV->GetHostUI();
	}
	else if( EQUSTR( pstrClass, L"D2View") )
	{
		m_pView = CD2ViewUI::CreateUI( m_hWnd );
		return m_pView->GetHostUI();
	}
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

	paint_manager_.SetDefaultLinkFontColor( 0xFF000000 );
	paint_manager_.SetDefaultLinkHoverFontColor( 0xFF000000 );

	FindCtrl<CTextUI>( L"tx" )->SetText( L"{c #FF0000}{f 2}  {c #00FF00}xyz{/c}  {a app:notepad}hello{/a}  {f 1}abc{/f}  {a 2}world{/a}{/f}{/c}" );

	m_ocx->SetValue( L"" );
	m_pIPV->InitAllImage();

	m_pView->InitAllImage();
}

void CSwhMainLayout::Notify( TNotifyUI& msg )
{
	CStringW	strSenderName = msg.pSender->GetName();
	CStringW	strType = msg.sType;

	if( strType == L"click")
	{
		if( strSenderName == L"bnClose" )
		{
			PostQuitMessage( 1 );
		}
		if( strSenderName == L"bnMax" )
		{
			BOOL bZoomed = ::IsZoomed(m_hWnd);
			SendMessage(WM_SYSCOMMAND, bZoomed?SC_RESTORE:SC_MAXIMIZE, 0);
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
		else if( strSenderName == L"bn3" )
		{
			CExitTPWindow::Show( GetHWND() );
		}
	}
	else if( L"link" == strType )
	{
		LPCTSTR str = (LPCTSTR)msg.lParam;
		MsgBox( str );

		//if( 0 == msg.wParam )
		//	MsgBox( L"hello" );
		//else if( 1 == msg.wParam )
		//	MsgBox( L"world" );

	}
	else if( strType == L"selectchanged" )
	{
		CCheckBoxUI *pCk = FindCtrl<CCheckBoxUI>( L"bnL2" );
		CVerticalLayoutUI *pVL = FindCtrl<CVerticalLayoutUI>( L"vlL2Sub" );
		pVL->SetVisible( pCk->IsCheck() );


		CTabLayoutUI *pTabMain = FindCtrl<CTabLayoutUI>(L"tabMain");
		if( L"bnL3" == strSenderName )
			pTabMain->SelectItem( 1 );
		else if( L"bnL4" == strSenderName )
			pTabMain->SelectItem( 2 );
		else
			pTabMain->SelectItem( 0 );
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