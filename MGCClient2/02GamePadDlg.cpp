#include "StdAfx.h"
#include "02GamePadDlg.h"
#include "CategoryUI.h"
#include "GalleryUI.h"


C02GamePadDlg::C02GamePadDlg(void)
	: m_nLoginState( 0 )
{
}


C02GamePadDlg::~C02GamePadDlg(void)
{
}

CControlUI* C02GamePadDlg::CreateControl( LPCTSTR pstrClass )
{
	if( EQUSTR( pstrClass, L"Category" ) )
		return new CCategoryUI;
	else if( EQUSTR( pstrClass, L"Gallery" ) )
		return new CGalleryUI;
	else if( EQUSTR(pstrClass, L"VScroll") )
		return new CVScrollUI;

	return NULL;
}

LRESULT C02GamePadDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	EndModal(1);
	return 0;
}

void C02GamePadDlg::Init()
{
	m_category	= FindCtrl<CCategoryUI>( L"category" );
	m_gallery	= FindCtrl<CGalleryUI>( L"gallery" );

	m_vs = FindCtrl<CVScrollUI>( L"vs" );
	m_vs->SetHost( this );
}

void C02GamePadDlg::Notify( TNotifyUI& msg )
{
	CStringW	strSenderName = msg.pSender->GetName();
	CStringW	strType = msg.sType;

	if( strType == L"click")
	{
		if( strSenderName == L"bn" )
		{
			CButtonUI	*pBn = FindCtrl<CButtonUI>( L"bn" );

			m_nLoginState = !m_nLoginState;

			if( m_nLoginState == 1 )
			{
				pBn->SetNormalImage( L"endlogin-normal.png" );
				pBn->SetHotImage( L"endlogin-hover.png" );
				pBn->SetPushedImage( L"endlogin-pushed.png" );
			}
			else
			{
				pBn->SetNormalImage( L"startlogin-normal.png" );
				pBn->SetHotImage( L"startlogin-hover.png" );
				pBn->SetPushedImage( L"startlogin-pushed.png" );
			}

		}

	}
}

LRESULT C02GamePadDlg::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( WM_GLY_SETVS_RATIO == uMsg )
	{
		float f = wParam/100.f;
		m_vs->SetRatio( f );
	}
	else if( WM_GLY_CLICK_LEFT == uMsg )
	{
		ATLTRACE( L"WM_GLY_CLICK_LEFT %d\n", wParam );
	}
	else if( WM_GLY_CLICK_RIGHT == uMsg )
	{
		ATLTRACE( L"WM_GLY_CLICK_RIGHT %d\n", wParam );
	}

	return CDialogBase::HandleMessage(uMsg, wParam, lParam);
}

void C02GamePadDlg::Data2UI()
{
}

void C02GamePadDlg::OnSetRatio( PointF pt )
{
	//ATLTRACE( L"%.2f %.2f\n", pt.X, pt.Y );
	m_gallery->SetVSRatio( pt.Y );
}