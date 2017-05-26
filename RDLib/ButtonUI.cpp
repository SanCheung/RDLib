#include "StdAfx.h"
#include "LabelUI.h"
#include "ButtonUI.h"

//namespace DuiLib
//{



CButtonUI::CButtonUI()
	: m_uButtonState(0)
	, m_dwHotTextColor(0)
	, m_dwPushedTextColor(0)
	, m_dwFocusedTextColor(0)
	, m_DbClicked(false)

{
	m_uTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER;
}

LPCTSTR CButtonUI::GetClass() const
{
	return _T("ButtonUI");
}

LPVOID CButtonUI::GetInterface(LPCTSTR pstrName)
{
	if( EQUSTR(pstrName, _T("Button")) )
		return static_cast<CButtonUI*>(this);

	return CLabelUI::GetInterface(pstrName);
}

UINT CButtonUI::GetControlFlags() const
{
	return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
}

void CButtonUI::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CLabelUI::DoEvent(event);
		return;
	}

	if( event.Type == UIEVENT_SETFOCUS ) 
	{
		Invalidate();
	}
	if( event.Type == UIEVENT_KILLFOCUS ) 
	{
		Invalidate();
	}
	if( event.Type == UIEVENT_KEYDOWN )
	{
		if (IsKeyboardEnabled()) {
			if( event.chKey == VK_SPACE || event.chKey == VK_RETURN ) {
				Activate();
				return;
			}
		}
	}
	if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
	{
		if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() ) {
			m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
			Invalidate();			
			m_DbClicked = event.Type == UIEVENT_DBLCLICK;
		}
		return;
	}
	if( event.Type == UIEVENT_MOUSEMOVE )
	{
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
			if( ::PtInRect(&m_rcItem, event.ptMouse) ) m_uButtonState |= UISTATE_PUSHED;
			else m_uButtonState &= ~UISTATE_PUSHED;
			Invalidate();
		}
		return;
	}
	if( event.Type == UIEVENT_BUTTONUP )
	{
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
			if( ::PtInRect(&m_rcItem, event.ptMouse) ) Activate();
			m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
			Invalidate();
		}
		return;
	}
	if( event.Type == UIEVENT_CONTEXTMENU )
	{
		if( IsContextMenuUsed() ) {
			m_pManager->SendNotify(this, _T("menu"), event.wParam, event.lParam);
		}
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
	if( event.Type == UIEVENT_SETCURSOR ) {
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		return;
	}
	CLabelUI::DoEvent(event);
}

bool CButtonUI::Activate()
{
	if( !CControlUI::Activate() ) return false;
	if( m_pManager != NULL ) 
	{
		m_pManager->SendNotify(this, _T("click"));
		//if (m_DbClicked){
		//	m_pManager->SendNotify(this, _T("dbclick"));
		//}
		//else{
		//	m_pManager->SendNotify(this, _T("click"));
		//}
	}
	return true;
}

void CButtonUI::SetEnabled(bool bEnable)
{
	CControlUI::SetEnabled(bEnable);
	if( !IsEnabled() ) {
		m_uButtonState = 0;
	}
}

void CButtonUI::SetHotTextColor(DWORD dwColor)
{
	m_dwHotTextColor = dwColor;
}

DWORD CButtonUI::GetHotTextColor() const
{
	return m_dwHotTextColor;
}

void CButtonUI::SetPushedTextColor(DWORD dwColor)
{
	m_dwPushedTextColor = dwColor;
}

DWORD CButtonUI::GetPushedTextColor() const
{
	return m_dwPushedTextColor;
}

void CButtonUI::SetFocusedTextColor(DWORD dwColor)
{
	m_dwFocusedTextColor = dwColor;
}

DWORD CButtonUI::GetFocusedTextColor() const
{
	return m_dwFocusedTextColor;
}

void CButtonUI::SetNormalImage(LPCTSTR pStrImage)
{
	m_sNormalImage = pStrImage;
	Invalidate();
}

void CButtonUI::SetHotImage(LPCTSTR pStrImage)
{
	m_sHotImage = pStrImage;
	Invalidate();
}

void CButtonUI::SetPushedImage(LPCTSTR pStrImage)
{
	m_sPushedImage = pStrImage;
	Invalidate();
}

void CButtonUI::SetFocusedImage(LPCTSTR pStrImage)
{
	m_sFocusedImage = pStrImage;
	Invalidate();
}

void CButtonUI::SetDisabledImage(LPCTSTR pStrImage)
{
	m_sDisabledImage = pStrImage;
	Invalidate();
}

SIZE CButtonUI::EstimateSize(SIZE szAvailable)
{
	if( m_cxyFixed.cy == 0 ) 
		return CDUISize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 8);

	return CLabelUI::EstimateSize(szAvailable);
}

void CButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( EQUSTR( pstrName, _T("normalimage")) ) 
		SetNormalImage(pstrValue);
	else if( EQUSTR(pstrName, _T("hotimage")) ) 
		SetHotImage(pstrValue);
	else if( EQUSTR(pstrName, _T("pushedimage")) ) 
		SetPushedImage(pstrValue);
	else if( EQUSTR(pstrName, _T("focusedimage")) ) 
		SetFocusedImage(pstrValue);
	else if( EQUSTR(pstrName, _T("disabledimage")) ) 
		SetDisabledImage(pstrValue);
	else if( EQUSTR(pstrName, _T("hottextcolor")) ) 
		SetHotTextColor( FormatToColor( pstrValue ) );
	else if( EQUSTR(pstrName, _T("pushedtextcolor")) ) 
		SetPushedTextColor( FormatToColor( pstrValue ) );
	else if( EQUSTR(pstrName, _T("focusedtextcolor")) ) 
		SetFocusedTextColor( FormatToColor( pstrValue ) );
	else 
		CLabelUI::SetAttribute(pstrName, pstrValue);
}

void CButtonUI::PaintText(HDC hDC)
{
	if( IsFocused() ) 
		m_uButtonState |= UISTATE_FOCUSED;
	else
		m_uButtonState &= ~ UISTATE_FOCUSED;

	if( !IsEnabled() ) 
		m_uButtonState |= UISTATE_DISABLED;
	else 
		m_uButtonState &= ~ UISTATE_DISABLED;

	if( m_dwTextColor == 0 ) 
		m_dwTextColor = m_pManager->GetDefaultFontColor();
	if( m_dwDisabledTextColor == 0 ) 
		m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

	if( m_sText.IsEmpty() ) 
		return;

	int nLinks = 0;
	RECT rc = m_rcItem;
	rc.left += m_rcTextPadding.left;
	rc.right -= m_rcTextPadding.right;
	rc.top += m_rcTextPadding.top;
	rc.bottom -= m_rcTextPadding.bottom;

	DWORD clrColor = IsEnabled()?m_dwTextColor:m_dwDisabledTextColor;

	if( ((m_uButtonState & UISTATE_PUSHED) != 0) && (GetPushedTextColor() != 0) )
		clrColor = GetPushedTextColor();
	else if( ((m_uButtonState & UISTATE_HOT) != 0) && (GetHotTextColor() != 0) )
		clrColor = GetHotTextColor();
	else if( ((m_uButtonState & UISTATE_FOCUSED) != 0) && (GetFocusedTextColor() != 0) )
		clrColor = GetFocusedTextColor();

	if( m_bShowHtml )
		CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, clrColor, \
		m_iFont, NULL, NULL, nLinks, m_uTextStyle);
	else
		CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, clrColor, \
		m_iFont, m_uTextStyle);
}

void CButtonUI::PaintStatusImage(HDC hDC)
{
	if( IsFocused() ) 
		m_uButtonState |= UISTATE_FOCUSED;
	else 
		m_uButtonState &= ~ UISTATE_FOCUSED;

	if( !IsEnabled() ) 
		m_uButtonState |= UISTATE_DISABLED;
	else 
		m_uButtonState &= ~ UISTATE_DISABLED;

	if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
		if( !m_sDisabledImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) m_sDisabledImage.Empty();
			else return;
		}
	}
	else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
		if( !m_sPushedImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sPushedImage) ) m_sPushedImage.Empty();
			else return;
		}
	}
	else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
		if( !m_sHotImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ) m_sHotImage.Empty();
			else return;
		}
	}
	else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
		if( !m_sFocusedImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) m_sFocusedImage.Empty();
			else return;
		}
	}

	if( !m_sNormalImage.IsEmpty() ) {
		if( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) )
			m_sNormalImage.Empty();
		else 
			return;
	}
}




//JS_METHOD(_Button_Constructor)
//{
//	ASSERT_CONSTRUCTOR;
//	SAVE_PTR(0, new CButtonUI());
//	return args.This();
//}
//
//static Handle<Value> textGetter( Local<String> name, const AccessorInfo& info)
//{ 
//	CButtonUI *item = reinterpret_cast<CButtonUI*>(info.This()->GetPointerFromInternalField(0));
//	if( item!=NULL )
//	{
//		CStringA	strA = item->GetText();
//		char *str = (char *)strA;
//		return v8::String::New( str );
//	}
//	return v8::Undefined();
//} 
//
//static void textSetter( Local<String> name, Local<Value> value, const AccessorInfo& info)
//{ 
//	CButtonUI *item = reinterpret_cast<CButtonUI*>(info.This()->GetPointerFromInternalField(0));
//	if( item!=NULL ){
//		v8::String::AsciiValue xname(value);
//
//		CStringW	strText = (CStringA)*xname;
//		item->SetText( strText );
//	}
//}
//
//Handle<v8::FunctionTemplate> CButtonUI::getJSClass()
//{
//	v8::HandleScope handle_scope;
//
//	//指定"构造函数"
//	v8::Handle<v8::FunctionTemplate> ft = v8::FunctionTemplate::New(_Button_Constructor);
//
//	//在javascript的class内放一个C++实例
//	v8::Handle<v8::ObjectTemplate> it = ft->InstanceTemplate();
//	it->SetInternalFieldCount(1); 
//
//	//类的"成员函数"
//	v8::Handle<v8::ObjectTemplate> pt = ft->PrototypeTemplate();;
//	pt->SetAccessor( String::New("Text"), textGetter, textSetter);
//
//	return handle_scope.Close(ft);
//}





//};
