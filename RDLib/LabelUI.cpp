#include "StdAfx.h"
#include "LabelUI.h"

//namespace DuiLib
//{


CLabelUI::CLabelUI() 
	: m_uTextStyle(DT_VCENTER)
	, m_dwTextColor(0)
	, m_dwDisabledTextColor(0)
	, m_iFont(-1)
	, m_bShowHtml(false)
{
    ::ZeroMemory(&m_rcTextPadding, sizeof(m_rcTextPadding));
}

LPCTSTR CLabelUI::GetClass() const
{
    return _T("LabelUI");
}

LPVOID CLabelUI::GetInterface(LPCTSTR pstrName)
{
    if( EQUSTR(pstrName, _T("Label")) ) 
		return static_cast<CLabelUI*>(this);

    return CControlUI::GetInterface(pstrName);
}

void CLabelUI::SetTextStyle(UINT uStyle)
{
    m_uTextStyle = uStyle;
    Invalidate();
}

UINT CLabelUI::GetTextStyle() const
{
	return m_uTextStyle;
}

void CLabelUI::SetTextColor(DWORD dwTextColor)
{
    m_dwTextColor = dwTextColor;
}

DWORD CLabelUI::GetTextColor() const
{
	return m_dwTextColor;
}

void CLabelUI::SetDisabledTextColor(DWORD dwTextColor)
{
    m_dwDisabledTextColor = dwTextColor;
}

DWORD CLabelUI::GetDisabledTextColor() const
{
	return m_dwDisabledTextColor;
}

void CLabelUI::SetFont(int index)
{
    m_iFont = index;
}

int CLabelUI::GetFont() const
{
	return m_iFont;
}

RECT CLabelUI::GetTextPadding() const
{
    return m_rcTextPadding;
}

void CLabelUI::SetTextPadding(RECT rc)
{
    m_rcTextPadding = rc;
    Invalidate();
}

bool CLabelUI::IsShowHtml()
{
    return m_bShowHtml;
}

void CLabelUI::SetShowHtml(bool bShowHtml)
{
    if( m_bShowHtml == bShowHtml ) return;

    m_bShowHtml = bShowHtml;
    Invalidate();
}

SIZE CLabelUI::EstimateSize(SIZE szAvailable)
{
	if( m_cxyFixed.cy == 0 ){
		CDUISize size;
		
		//需要寻找一种方法来精确计算字符串占用的界限
	/*	CDUIRect rect;
		
		TFontInfo* pFontInfo = m_pManager->GetFontInfo(GetFont());
		HDC paintDC = m_pManager->GetPaintDC();
		if (paintDC && pFontInfo && pFontInfo->hFont)
		{
			HFONT hOldFont = (HFONT)SelectObject(paintDC, pFontInfo->hFont);
			if(::GetTextExtentPoint32 (paintDC, m_sText, m_sText.GetLength(), &size))
			{

			}
			SelectObject(paintDC, hOldFont);
		}
		else{*/
			size = CDUISize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + m_rcTextPadding.top + m_rcTextPadding.bottom + 4);
		//}
		return size;
	}
    return CControlUI::EstimateSize(szAvailable);
}

void CLabelUI::DoEvent(TEventUI& event)
{
    if( event.Type == UIEVENT_SETFOCUS ) 
    {
        m_bFocused = true;
        return;
    }
    if( event.Type == UIEVENT_KILLFOCUS ) 
    {
        m_bFocused = false;
        return;
    }
    if( event.Type == UIEVENT_MOUSEENTER )
    {
        return;
    }
    if( event.Type == UIEVENT_MOUSELEAVE )
    {
        return;
    }
    CControlUI::DoEvent(event);
}

void CLabelUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if(  EQUSTR(pstrName, _T("align")) ) 
	{
        if( _tcsstr(pstrValue, _T("left")) != NULL ) {
            m_uTextStyle &= ~(DT_CENTER | DT_RIGHT | DT_TOP | DT_BOTTOM);
            m_uTextStyle |= DT_LEFT;
        }
        if( _tcsstr(pstrValue, _T("center")) != NULL ) {
            m_uTextStyle &= ~(DT_LEFT | DT_RIGHT | DT_TOP | DT_BOTTOM);
            m_uTextStyle |= DT_CENTER;
        }
        if( _tcsstr(pstrValue, _T("right")) != NULL ) {
            m_uTextStyle &= ~(DT_LEFT | DT_CENTER | DT_TOP | DT_BOTTOM);
            m_uTextStyle |= DT_RIGHT;
        }
		if( _tcsstr(pstrValue, _T("top")) != NULL ) {
			m_uTextStyle &= ~(DT_BOTTOM | DT_VCENTER | DT_LEFT | DT_RIGHT);
			m_uTextStyle |= DT_TOP;
		}
		if( _tcsstr(pstrValue, _T("bottom")) != NULL ) {
			m_uTextStyle &= ~(DT_TOP | DT_VCENTER | DT_LEFT | DT_RIGHT);
			m_uTextStyle |= DT_BOTTOM;
		}
    }
    else if( EQUSTR(pstrName, _T("endellipsis")) ) {
        if( EQUSTR(pstrValue, _T("true")) ) 
			m_uTextStyle |= DT_END_ELLIPSIS;
        else 
			m_uTextStyle &= ~DT_END_ELLIPSIS;
    }    
    else if( EQUSTR(pstrName, _T("font")) ) 
		SetFont(_ttoi(pstrValue));
    else if( EQUSTR(pstrName, _T("textcolor")) ) 
	{
        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        SetTextColor(clrColor);
    }
    else if( EQUSTR(pstrName, _T("disabledtextcolor")) )
	{
        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        SetDisabledTextColor(clrColor);
    }
    else if( EQUSTR(pstrName, _T("textpadding")) ) 
	{
        RECT rcTextPadding = { 0 };
        LPTSTR pstr = NULL;
        rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
        rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
        rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
        rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
        SetTextPadding(rcTextPadding);
    }
    else if( EQUSTR(pstrName, _T("showhtml")) ) 
		SetShowHtml( EQUSTR(pstrValue, _T("true")) );
    else
		CControlUI::SetAttribute(pstrName, pstrValue);
}

void CLabelUI::PaintText(HDC hDC)
{
    if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
    if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

    if( m_sText.IsEmpty() ) return;
    int nLinks = 0;
    RECT rc = m_rcItem;
    rc.left += m_rcTextPadding.left;
    rc.right -= m_rcTextPadding.right;
    rc.top += m_rcTextPadding.top;
    rc.bottom -= m_rcTextPadding.bottom;
    if( IsEnabled() ) {
        if( m_bShowHtml )
            CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, m_dwTextColor, \
				m_iFont, NULL, NULL, nLinks, DT_SINGLELINE | m_uTextStyle);
				//m_iFont, NULL, NULL, nLinks, m_uTextStyle);
        else
            CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwTextColor, \
				m_iFont, DT_SINGLELINE | m_uTextStyle);
    }
    else {
        if( m_bShowHtml )
            CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, m_dwDisabledTextColor, \
				m_iFont, NULL, NULL, nLinks, DT_SINGLELINE | m_uTextStyle);
        else
            CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwDisabledTextColor, \
				m_iFont, DT_SINGLELINE | m_uTextStyle);
    }
}


//};