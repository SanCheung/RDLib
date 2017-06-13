#include "StdAfx.h"
#include "WndUI.h"

CWndUI::CWndUI( CHostableWnd* pWnd )
	: m_pWnd( pWnd )
{
}

CWndUI::~CWndUI()
{
	DestroyInternalWindow();
}

void CWndUI::SetPos( RECT rc )
{
	CControlUI::SetPos(rc);
	if (m_pWnd)
	{
		CDUIRect rect = rc;
		//m_pWnd->MoveWindow(rect.left + m_nBorderSize, rect.top + m_nBorderSize, rect.GetWidth() - m_nBorderSize * 2, rect.GetHeight() - m_nBorderSize * 2);
		m_pWnd->MoveWindow( rect.left, rect.top, rect.Width(), rect.Height() );
	}

	m_rcOld = rc;
}

void CWndUI::SetInternVisible( bool bVisible /* = true */ )
{
	CControlUI::SetInternVisible(bVisible);
	if (m_pWnd){
		m_pWnd->ShowWindow(bVisible, false);
	}
}

void CWndUI::SetVisible(bool bVisible)
{
	CControlUI::SetVisible(bVisible);
	if (m_pWnd)
	{
		m_pWnd->ShowWindow(bVisible, bVisible?true:false);
		if (!IsVisible())
		{
			m_pManager->SetFocus(NULL);
		}
	}
}

//CHostableWnd* CWndUI::GetWindow(LPCWSTR pstrWClassName)
//{
//	CHostableWnd* pWindow = NULL;
//	if (m_ClassName == pstrWClassName){
//		pWindow = m_pWnd;
//	}
//	return pWindow;
//}

void CWndUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	//if (_tcscmp(pstrName, L"wclass") == 0)
	//{
	//	m_ClassName = pstrValue;
	//}
	//else{
		CControlUI::SetAttribute(pstrName, pstrValue);
	//}
}

void CWndUI::DoInit()
{
	//if (m_ClassName == ClassName_TestWnd)
	//{
		//m_pWnd = new CTestWnd;
		//m_pWnd->Create(GetManager()->GetPaintWindow(), L"", UI_WNDSTYLE_CHILD, 0);
	//}
	//else if (m_ClassName == WhiteBoardWClass)
	//{
	//	m_pWnd = new CWhiteBoardWnd;
	//	m_pWnd->Create(GetManager()->GetPaintWindow(), L"", WS_CHILD|WS_VISIBLE/*|WS_VSCROLL|WS_HSCROLL*/, 0);
	//}

	if (m_pWnd)
	{
		//m_pWnd->SetHostUI(this);
		//m_pWnd->m_pHost = this;
		NeedParentUpdate();
	}
}

void CWndUI::DoEvent( TEventUI& ev )
{
	//TRACE( L"%d", ev.Type );

	if( ev.Type == UIEVENT_SETCURSOR )
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		return;
	}

	if( ev.Type == UIEVENT_SETFOCUS ) 
	{
		m_bFocused = true;
		return;
	}

	if( ev.Type == UIEVENT_KILLFOCUS ) 
	{
		m_bFocused = false;
		return;
	}

	if( ev.Type == UIEVENT_TIMER )
	{
		m_pManager->SendNotify(this, _T("timer"), ev.wParam, ev.lParam);
		return;
	}

	if( ev.Type == UIEVENT_CONTEXTMENU )
	{
		if( IsContextMenuUsed() ) {
			m_pManager->SendNotify(this, _T("menu"), ev.wParam, ev.lParam);
			return;
		}
	}

	if( ev.Type > UIEVENT__MOUSEBEGIN && ev.Type < UIEVENT__MOUSEEND )
		return;

	if( m_pParent != NULL ) m_pParent->DoEvent( ev );
}

void CWndUI::DestroyInternalWindow()
{
	if (m_pWnd && ::IsWindow(m_pWnd->GetHWND()))
	{
		::DestroyWindow(*m_pWnd);
		delete m_pWnd;
		m_pWnd = NULL;
	}
}

//void CWndUI::DoPaint( HDC hDC, const RECT& rcPaint )
//{
//	if( m_pWnd != NULL && ::IsWindow( m_pWnd->GetHWND() ) )
//	{
//		PostMessage( m_pWnd->GetHWND(), WM_REFRESHSUBWINDOW, 0, 0 );
//		//SetPos( m_rcOld );
//	}
//		
//	//{
//	//	HWND	hWnd = m_pWnd->GetHWND();
//	//	CDUIRect	rt;
//	//	GetClientRect( hWnd, rt );
//
//	//	HDC		hWndDC = ::GetDC( hWnd );
//	//	BitBlt( hDC, rcPaint.left, rcPaint.top, rcPaint.right-rcPaint.left, 
//	//		rcPaint.bottom - rcPaint.top, hWndDC, 0, 0, SRCCOPY );
//
//	//	::ReleaseDC( hWnd, hWndDC );
//	//}
//}
