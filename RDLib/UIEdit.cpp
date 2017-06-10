#include "StdAfx.h"
#include "UIEdit.h"

//namespace DuiLib
//{


class CEditWnd : public CWindowWnd
{
public:
	CEditWnd();

	void Init(CEditUI* pOwner);
	RECT CalPos();

	LPCTSTR GetWindowClassName() const;
	LPCTSTR GetSuperClassName() const;
	void OnFinalMessage(HWND hWnd);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	CEditUI* m_pOwner;
	HBRUSH m_hBkBrush;
	bool m_bInit;
};


CEditWnd::CEditWnd() : m_pOwner(NULL), m_hBkBrush(NULL), m_bInit(false)
{
}

void CEditWnd::Init(CEditUI* pOwner)
{
	m_pOwner = pOwner;
	RECT rcPos = CalPos();
	//UINT uStyle = WS_CHILD | ES_AUTOHSCROLL;
	UINT uStyle = WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|ES_AUTOHSCROLL;
	if (m_pOwner->m_bMultiLine)
	{
		uStyle = WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VSCROLL|ES_AUTOVSCROLL|ES_WANTRETURN|ES_MULTILINE;
	}

	if( m_pOwner->IsPasswordMode() ) uStyle |= ES_PASSWORD;
	if (pOwner->IsNumberOnly()) uStyle |= ES_NUMBER;
	if (pOwner->IsUpper()) uStyle |= ES_UPPERCASE;	

	Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);
	SetWindowFont(m_hWnd, m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->hFont, TRUE);
	Edit_LimitText(m_hWnd, m_pOwner->GetMaxChar());
	if( m_pOwner->IsPasswordMode() ) Edit_SetPasswordChar(m_hWnd, m_pOwner->GetPasswordChar());
	Edit_SetText(m_hWnd, m_pOwner->GetText());
	Edit_SetModify(m_hWnd, FALSE);
	SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(0, 0));
	Edit_Enable(m_hWnd, m_pOwner->IsEnabled() == true);
	Edit_SetReadOnly(m_hWnd, m_pOwner->IsReadOnly() == true);
	::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
	::SetFocus(m_hWnd);
	m_bInit = true;    
}

RECT CEditWnd::CalPos()
{
	//CDUIRect rcPos = m_pOwner->GetPos();
	//RECT rcInset = m_pOwner->GetTextPadding();
	//rcPos.left += rcInset.left;
	//rcPos.top += rcInset.top;
	//rcPos.right -= rcInset.right;
	//rcPos.bottom -= rcInset.bottom;
	//LONG lEditHeight = m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->tm.tmHeight;
	//if( lEditHeight < rcPos.GetHeight() ) {
	//    rcPos.top += (rcPos.GetHeight() - lEditHeight) / 2;
	//    rcPos.bottom = rcPos.top + lEditHeight;
	//}

	CDUIRect rcPos = m_pOwner->GetPos();
	RECT rcInset = m_pOwner->GetTextPadding();
	rcPos.left += rcInset.left;
	rcPos.top += rcInset.top;
	rcPos.right -= rcInset.right;
	rcPos.bottom -= rcInset.bottom;

	if (!m_pOwner->m_bMultiLine)
	{
		//TFontInfo ti;
		//UIGetResourceManager()->GetFont(m_pOwner->GetFont(), ti);
		//LONG lEditHeight = ti.tm.tmHeight;
		LONG lEditHeight = m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->tm.tmHeight;
		if( lEditHeight < rcPos.GetHeight() ) {
			rcPos.top += (rcPos.GetHeight() - lEditHeight) / 2;
			rcPos.bottom = rcPos.top + lEditHeight;
		}
	}
	return rcPos;
}

LPCTSTR CEditWnd::GetWindowClassName() const
{
	return _T("EditWnd");
}

LPCTSTR CEditWnd::GetSuperClassName() const
{
	return WC_EDIT;
}

void CEditWnd::OnFinalMessage(HWND /*hWnd*/)
{
	// Clear reference and die
	if( m_hBkBrush != NULL ) ::DeleteObject(m_hBkBrush);
	m_pOwner->m_pWindow = NULL;
	delete this;
}

LRESULT CEditWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	if( uMsg == WM_KILLFOCUS )
		lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
	else if( uMsg == OCM_COMMAND )
	{
		if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE ) 
			lRes = OnEditChanged(uMsg, wParam, lParam, bHandled);
		else if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_UPDATE )
		{
			RECT rcClient;
			::GetClientRect(m_hWnd, &rcClient);
			::InvalidateRect(m_hWnd, &rcClient, FALSE);
		}
	}
	else if( uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_RETURN ) 
	{
		//m_pOwner->GetManager()->SendNotify(m_pOwner, _T("return"));
		if(m_pOwner->m_bMultiLine )
		{
			bHandled = FALSE;
		}
		else
		{
			m_pOwner->GetManager()->SendNotify(m_pOwner, _T("return"));	//UIE_KEY_RETURN);
		}
	}
	else if( uMsg == OCM__BASE + WM_CTLCOLOREDIT  || uMsg == OCM__BASE + WM_CTLCOLORSTATIC )
	{
		if( m_pOwner->GetNativeEditBkColor() == 0xFFFFFFFF ) return NULL;
		::SetBkMode((HDC)wParam, TRANSPARENT);
		DWORD dwTextColor = m_pOwner->GetTextColor();
		::SetTextColor((HDC)wParam, RGB(GetBValue(dwTextColor),GetGValue(dwTextColor),GetRValue(dwTextColor)));
		if( m_hBkBrush == NULL ) 
		{
			DWORD clrColor = m_pOwner->GetNativeEditBkColor();
			m_hBkBrush = ::CreateSolidBrush(RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
		}
		return (LRESULT)m_hBkBrush;
	}
	else 
		bHandled = FALSE;

	if( !bHandled ) 
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CEditWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	//PostMessage(WM_CLOSE);
	//return lRes;
	LRESULT lRes = 0;
	if (!m_pOwner->m_bMultiLine)
	{
		lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		PostMessage(WM_CLOSE);
	}
	else{
		bHandled = FALSE;
	}
	return lRes;
}

LRESULT CEditWnd::OnEditChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if( !m_bInit ) return 0;
	if( m_pOwner == NULL ) return 0;
	// Copy text back
	int cchLen = ::GetWindowTextLength(m_hWnd) + 1;
	LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
	ASSERT(pstr);
	if( pstr == NULL ) return 0;
	::GetWindowText(m_hWnd, pstr, cchLen);
	m_pOwner->m_sText = pstr;
	m_pOwner->GetManager()->SendNotify(m_pOwner, _T("textchanged"));

	m_pOwner->Invalidate();
	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////
//
//

CEditUI::CEditUI() 
	: m_pWindow(NULL)
	, m_uMaxChar(2048)
	, m_bReadOnly(false)
	, m_bPasswordMode(false)
	, m_cPasswordChar(_T('*'))
	, m_uButtonState(0)
	, m_dwEditbkColor(0xFFFFFFFF)
	, m_bNumberOnly(false)
	, m_bUpper(false)
	, m_BannerColor(0xffb1c5d7)
	, m_bMultiLine( false )
{
	SetTextPadding(CDUIRect(4, 3, 4, 3));
	SetBkColor(0xFFFFFFFF);
}

LPCTSTR CEditUI::GetClass() const
{
	return _T("EditUI");
}

LPVOID CEditUI::GetInterface(LPCTSTR pstrName)
{
	if( EQUSTR(pstrName, _T("Edit"))  ) 
		return static_cast<CEditUI*>(this);

	return CLabelUI::GetInterface(pstrName);
}

UINT CEditUI::GetControlFlags() const
{
	if( !IsEnabled() ) return CControlUI::GetControlFlags();

	return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
}

void CEditUI::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CLabelUI::DoEvent(event);
		return;
	}

	if( event.Type == UIEVENT_SETCURSOR && IsEnabled() )
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
		return;
	}
	if( event.Type == UIEVENT_WINDOWSIZE )
	{
		if( m_pWindow != NULL ) m_pManager->SetFocusNeeded(this);
	}
	if( event.Type == UIEVENT_SCROLLWHEEL )
	{
		if( m_pWindow != NULL ) return;
	}
	if( event.Type == UIEVENT_SETFOCUS && IsEnabled() ) 
	{
		//if( !m_pWindow ) {		
		//	m_pWindow = new CEditWnd();
		//	ASSERT(m_pWindow);
		//	m_pWindow->Init(this);
		//}
		//if (IsVisible())
		//{
		//	m_pWindow->ShowWindow(true, true);
		//}
		//Invalidate();
		GetManager()->ReleaseCapture();
		if( !m_pWindow ) 
		{		
			m_pWindow = new CEditWnd();
			ASSERT(m_pWindow);
			m_pWindow->Init(this);
		}
		else if (m_bMultiLine)
		{
			::SetFocus(*m_pWindow);
		}
		Invalidate();
	}
	if( event.Type == UIEVENT_KILLFOCUS && IsEnabled() ) 
	{
		Invalidate();
	}
	if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_RBUTTONDOWN) 
	{
		if( IsEnabled() ) {
			GetManager()->ReleaseCapture();
			if( IsFocused() && m_pWindow == NULL )
			{
				m_pWindow = new CEditWnd();
				ASSERT(m_pWindow);
				m_pWindow->Init(this);

				if( PtInRect(&m_rcItem, event.ptMouse) )
				{
					int nSize = GetWindowTextLength(*m_pWindow);
					if( nSize == 0 )
						nSize = 1;

					Edit_SetSel(*m_pWindow, 0, nSize);
				}
			}
			else if( m_pWindow != NULL )
			{
#if 1
				int nSize = GetWindowTextLength(*m_pWindow);
				if( nSize == 0 )
					nSize = 1;

				Edit_SetSel(*m_pWindow, 0, nSize);
#else
				POINT pt = event.ptMouse;
				pt.x -= m_rcItem.left + m_rcTextPadding.left;
				pt.y -= m_rcItem.top + m_rcTextPadding.top;
				::SendMessage(*m_pWindow, WM_LBUTTONDOWN, event.wParam, MAKELPARAM(pt.x, pt.y));
#endif
			}
		}
		return;
	}
	if( event.Type == UIEVENT_MOUSEMOVE ) 
	{
		return;
	}
	if( event.Type == UIEVENT_BUTTONUP ) 
	{
		return;
	}
	if( event.Type == UIEVENT_CONTEXTMENU )
	{
		return;
	}
	if( event.Type == UIEVENT_MOUSEENTER )
	{
		if( IsEnabled() ) {
			m_uButtonState |= UISTATE_HOT;
			Invalidate();
		}
		return;
	}
	if( event.Type == UIEVENT_MOUSELEAVE )
	{
		if( IsEnabled() ) {
			m_uButtonState &= ~UISTATE_HOT;
			Invalidate();
		}
		return;
	}
	CLabelUI::DoEvent(event);
}

void CEditUI::SetEnabled(bool bEnable)
{
	CControlUI::SetEnabled(bEnable);
	if( !IsEnabled() ) {
		m_uButtonState = 0;
	}
}

void CEditUI::SetText(LPCTSTR pstrText)
{
	m_sText = pstrText;
	if( m_pWindow != NULL ) Edit_SetText(*m_pWindow, m_sText);
	Invalidate();
}

void CEditUI::SetMaxChar(UINT uMax)
{
	m_uMaxChar = uMax;
	if( m_pWindow != NULL ) Edit_LimitText(*m_pWindow, m_uMaxChar);
}

UINT CEditUI::GetMaxChar()
{
	return m_uMaxChar;
}

void CEditUI::SetReadOnly(bool bReadOnly)
{
	if( m_bReadOnly == bReadOnly ) return;

	m_bReadOnly = bReadOnly;
	if( m_pWindow != NULL ) Edit_SetReadOnly(*m_pWindow, m_bReadOnly);
	Invalidate();
}

bool CEditUI::IsReadOnly() const
{
	return m_bReadOnly;
}

void CEditUI::SetPasswordMode(bool bPasswordMode)
{
	if( m_bPasswordMode == bPasswordMode ) return;
	m_bPasswordMode = bPasswordMode;
	Invalidate();
}

bool CEditUI::IsPasswordMode() const
{
	return m_bPasswordMode;
}

void CEditUI::SetPasswordChar(TCHAR cPasswordChar)
{
	if( m_cPasswordChar == cPasswordChar ) return;
	m_cPasswordChar = cPasswordChar;
	if( m_pWindow != NULL ) Edit_SetPasswordChar(*m_pWindow, m_cPasswordChar);
	Invalidate();
}

TCHAR CEditUI::GetPasswordChar() const
{
	return m_cPasswordChar;
}

LPCTSTR CEditUI::GetNormalImage()
{
	return m_sNormalImage;
}

void CEditUI::SetNormalImage(LPCTSTR pStrImage)
{
	m_sNormalImage = pStrImage;
	Invalidate();
}

LPCTSTR CEditUI::GetHotImage()
{
	return m_sHotImage;
}

void CEditUI::SetHotImage(LPCTSTR pStrImage)
{
	m_sHotImage = pStrImage;
	Invalidate();
}

LPCTSTR CEditUI::GetFocusedImage()
{
	return m_sFocusedImage;
}

void CEditUI::SetFocusedImage(LPCTSTR pStrImage)
{
	m_sFocusedImage = pStrImage;
	Invalidate();
}

LPCTSTR CEditUI::GetDisabledImage()
{
	return m_sDisabledImage;
}

void CEditUI::SetDisabledImage(LPCTSTR pStrImage)
{
	m_sDisabledImage = pStrImage;
	Invalidate();
}

void CEditUI::SetNativeEditBkColor(DWORD dwBkColor)
{
	m_dwEditbkColor = dwBkColor;
}

DWORD CEditUI::GetNativeEditBkColor() const
{
	return m_dwEditbkColor;
}

void CEditUI::SetPos(RECT rc)
{
	CControlUI::SetPos(rc);
	if( m_pWindow != NULL )
	{
		RECT rcPos = m_pWindow->CalPos();
		::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
			rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);        
	}
}

void CEditUI::SetVisible(bool bVisible)
{
	//   CControlUI::SetVisible(bVisible);
	//if( !IsVisible() && m_pWindow != NULL ){
	//	m_pManager->SetFocus(NULL);
	//}
	CControlUI::SetVisible(bVisible);
	if (m_pWindow)
	{
		m_pWindow->ShowWindow(bVisible, bVisible?true:false);
		if (!IsVisible())
		{
			m_pManager->SetFocus(NULL);
		}
	}
}

void CEditUI::SetInternVisible(bool bVisible)
{
	/*  if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);*/
	SetVisible(bVisible);
}

SIZE CEditUI::EstimateSize(SIZE szAvailable)
{
	if( m_cxyFixed.cy == 0 ) return CDUISize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 6);
	return CControlUI::EstimateSize(szAvailable);
}

void CEditUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( EQUSTR(pstrName, _T("readonly")) )			SetReadOnly(EQUSTR_TRUE(pstrValue) );
	else if( EQUSTR(pstrName, _T("password")) )		SetPasswordMode(EQUSTR_TRUE(pstrValue) );
	else if( EQUSTR(pstrName, _T("maxchar")) )		SetMaxChar(_ttoi(pstrValue));
	else if( EQUSTR(pstrName, _T("number")) )		SetNumberOnly( EQUSTR_TRUE(pstrValue) );
	else if( EQUSTR(pstrName, _T("upper")) )		SetUpper( EQUSTR_TRUE(pstrValue) );
	else if( EQUSTR(pstrName, _T("normalimage")) )	SetNormalImage(pstrValue);
	else if( EQUSTR(pstrName, _T("hotimage"))  )	SetHotImage(pstrValue);
	else if( EQUSTR(pstrName, _T("focusedimage")) )		SetFocusedImage(pstrValue);
	else if( EQUSTR(pstrName, _T("disabledimage")) )	SetDisabledImage(pstrValue);
	else if( EQUSTR(pstrName, _T("banner")) )			SetBanner(pstrValue);
	else if( EQUSTR(pstrName, _T("bannercolor")) )
	{
		DWORD clrColor = FormatToColor(pstrValue);
		SetBannerColor(clrColor);
	}
	else if( EQUSTR(pstrName, _T("nativebkcolor")) )
	{
		DWORD clrColor = FormatToColor(pstrValue);
		SetNativeEditBkColor(clrColor);
	}
	else if( EQUSTR(pstrName, _T("multiline")) )
	{
		m_bMultiLine = EQUSTR_TRUE(pstrValue);
		m_uTextStyle = 0;
	}
	else CLabelUI::SetAttribute(pstrName, pstrValue);
}

void CEditUI::PaintStatusImage(HDC hDC)
{
	if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
	else m_uButtonState &= ~ UISTATE_FOCUSED;
	if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
	else m_uButtonState &= ~ UISTATE_DISABLED;

	if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
		if( !m_sDisabledImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) m_sDisabledImage.Empty();
			else return;
		}
	}
	else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
		if( !m_sFocusedImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) m_sFocusedImage.Empty();
			else return;
		}
	}
	else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
		if( !m_sHotImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ) m_sHotImage.Empty();
			else return;
		}
	}

	if( !m_sNormalImage.IsEmpty() ) {
		if( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) ) m_sNormalImage.Empty();
		else return;
	}
}

void CEditUI::PaintText(HDC hDC)
{
	if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
	if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

	DWORD textColor = m_dwTextColor;
	CStringW sText = m_sText;
	if( m_sText.IsEmpty() && IsEnabled()) {
		sText = m_Banner;
		textColor = m_BannerColor;
	}

	if (sText.IsEmpty()){
		return;
	}

	if( m_bPasswordMode ) {
		sText.Empty();
		//LPCTSTR p = m_sText.GetData();
		LPCTSTR p = m_sText;
		while( *p != _T('\0') ) {
			sText += m_cPasswordChar;
			p = ::CharNext(p);
		}
	}

	//RECT rc = m_rcItem;
	//rc.left += m_rcTextPadding.left;
	//rc.right -= m_rcTextPadding.right;
	//rc.top += m_rcTextPadding.top;
	//rc.bottom -= m_rcTextPadding.bottom;
	//if( IsEnabled() ) {
	//    CRenderEngine::DrawText(hDC, m_pManager, rc, sText, textColor, \
	//        m_iFont, DT_SINGLELINE | m_uTextStyle);
	//}
	//else {
	//    CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwDisabledTextColor, \
	//        m_iFont, DT_SINGLELINE | m_uTextStyle);
	//}

	RECT rc = m_rcItem;
	rc.left += m_rcTextPadding.left;
	rc.right -= m_rcTextPadding.right;
	rc.top += m_rcTextPadding.top;
	rc.bottom -= m_rcTextPadding.bottom;
	if( !IsEnabled() ) 
	{
		textColor = m_dwDisabledTextColor;
	}

	UINT	uStyle = DT_SINGLELINE | DT_EXPANDTABS| m_uTextStyle;
	if (m_bMultiLine)
		uStyle = DT_EDITCONTROL|DT_EXPANDTABS | DT_WORD_ELLIPSIS;

	CRenderEngine::DrawText(hDC, m_pManager, rc, sText, textColor, m_iFont, uStyle );
}

bool CEditUI::IsNumberOnly()
{
	return m_bNumberOnly;
}

bool CEditUI::IsUpper()
{
	return m_bUpper;
}

void CEditUI::SetNumberOnly( bool b )
{
	m_bNumberOnly = b;
}

void CEditUI::SetUpper( bool b )
{
	m_bUpper = b;
}

void CEditUI::SetBanner( LPCTSTR pstrValue )
{
	m_Banner = pstrValue;
}

void CEditUI::SetBannerColor( DWORD clrColor )
{
	m_BannerColor = clrColor;
}

void CEditUI::DoInit()
{		
	if (m_bMultiLine && m_pWindow == NULL )
	{
		m_pWindow = new CEditWnd();
		ASSERT(m_pWindow);
		m_pWindow->Init(this);
	}
}

void CEditUI::SetSel( int nStart, int nEnd )
{
	if( m_pWindow != NULL && ::IsWindow( m_pWindow->GetHWND() ) )
	{
		Edit_SetSel(*m_pWindow, nStart, nEnd);
	}
}



//};
