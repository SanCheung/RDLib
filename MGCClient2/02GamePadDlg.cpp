#include "StdAfx.h"
#include "02GamePadDlg.h"
#include "CategoryUI.h"
#include "GalleryUI.h"

#include "02FloatWnd.h"


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
	//EndModal(1);
	ShowWindow( false );
	return 0;
}

void C02GamePadDlg::Init()
{
	m_category	= FindCtrl<CCategoryUI>( L"category" );
	m_gallery	= FindCtrl<CGalleryUI>( L"gallery" );

	m_vs = FindCtrl<CVScrollUI>( L"vs" );
	m_vs->SetHost( this );

	m_pBnLogin	= FindCtrl<CButtonUI>( L"bnLogin" );
	m_pBnLogin->OnEvent += MakeDelegate(this, &C02GamePadDlg::OnButtonLoginEvent );

	::ShowWindow( m_hWnd, SW_MAXIMIZE );
}

void C02GamePadDlg::Notify( TNotifyUI& msg )
{
	CStringW	strSenderName = msg.pSender->GetName();
	CStringW	strType = msg.sType;

	if( strType == L"click")
	{
		if( strSenderName == L"bnLogin" )
		{
			m_nLoginState = !m_nLoginState;

			if( m_nLoginState == 1 )
			{
				m_pBnLogin->SetNormalImage( L"endlogin-normal.png" );
				m_pBnLogin->SetHotImage( L"endlogin-hover.png" );
				m_pBnLogin->SetPushedImage( L"endlogin-pushed.png" );
			}
			else
			{
				m_pBnLogin->SetNormalImage( L"startlogin-normal.png" );
				m_pBnLogin->SetHotImage( L"startlogin-hover.png" );
				m_pBnLogin->SetPushedImage( L"startlogin-pushed.png" );
			}

			C02FloatWnd::Hide();
		}
	}
	else if( strType == L"hover-loginbutton" )
	{
		if( !m_nLoginState )
			return;

		CDUIRect	rtItem = m_pBnLogin->GetPos();
		CDUIPoint	ptLT = rtItem.TopLeft();
		ptLT.Offset( 0, -190 );
		MapWindowPoints( m_hWnd, GetDesktopWindow(), &ptLT, 1 );
		C02FloatWnd::Show( m_hWnd, ptLT );
	}
	else if( strType == L"leave-loginbutton" )
	{
		if( !m_nLoginState )
			return;

		C02FloatWnd::Hide();
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

bool C02GamePadDlg::OnButtonLoginEvent( void* paramters )
{
	TEventUI* pEvent = (TEventUI*)paramters;	
	if( pEvent->Type == UIEVENT_MOUSEENTER || pEvent->Type == UIEVENT_MOUSEHOVER )
	{
		m_pBnLogin->GetManager()->SendNotify( m_pBnLogin, L"hover-loginbutton");
	}
	else if( pEvent->Type == UIEVENT_MOUSELEAVE )
	{
		m_pBnLogin->GetManager()->SendNotify( m_pBnLogin, L"leave-loginbutton");
	}

	return true;
}
