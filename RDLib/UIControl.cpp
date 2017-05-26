#include "StdAfx.h"

//namespace DuiLib {

CControlUI::CControlUI() : 
m_pManager(NULL), 
m_pParent(NULL), 
m_bUpdateNeeded(true),
m_bMenuUsed(false),
m_bVisible(true), 
m_bInternVisible(true),
m_bFocused(false),
m_bEnabled(true),
m_bMouseEnabled(true),
m_bKeyboardEnabled(true),
m_bFloat(false),
m_bSetPos(false),
m_chShortcut('\0'),
m_pTag(NULL),
m_dwBackColor(0),
m_dwBackColor2(0),
m_dwBackColor3(0),
m_dwBorderColor(0),
m_dwFocusBorderColor(0),
m_bColorHSL(false),
m_nBorderSize(1)
{
    m_cXY.cx = m_cXY.cy = 0;
    m_cxyFixed.cx = m_cxyFixed.cy = 0;
    m_cxyMin.cx = m_cxyMin.cy = 0;
    m_cxyMax.cx = m_cxyMax.cy = 9999;
    m_cxyBorderRound.cx = m_cxyBorderRound.cy = 0;

    ::ZeroMemory(&m_rcPadding, sizeof(m_rcPadding));
    ::ZeroMemory(&m_rcItem, sizeof(RECT));
    ::ZeroMemory(&m_rcPaint, sizeof(RECT));
    ::ZeroMemory(&m_tRelativePos, sizeof(TRelativePosUI));

	m_nBorderState = 15;
}

CControlUI::~CControlUI()
{
    if( OnDestroy ) OnDestroy(this);
    if( m_pManager != NULL ) m_pManager->ReapObjects(this);
}

CStringW CControlUI::GetName() const
{
    return m_sName;
}

void CControlUI::SetName(LPCTSTR pstrName)
{
    m_sName = pstrName;
}

LPVOID CControlUI::GetInterface(LPCTSTR pstrName)
{
    if( EQUSTR(pstrName, _T("Control")) ) 
		return static_cast<CControlUI*>(this);

    return NULL;
}

//LPCTSTR CControlUI::GetClass() const
//{
//    return _T("ControlUI");
//}

UINT CControlUI::GetControlFlags() const
{
    return 0;
}

bool CControlUI::Activate()
{
    if( !IsVisible() ) return false;
    if( !IsEnabled() ) return false;
    return true;
}

//CPaintManagerUI* CControlUI::GetManager() const
//{
//    return m_pManager;
//}

void CControlUI::SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit)
{
    m_pManager = pManager;
    m_pParent = pParent;
    if( bInit && m_pParent ) 
		Init();
}

//CControlUI* CControlUI::GetParent() const
//{
//	return m_pParent;
//}

CStringW CControlUI::GetText() const
{
    return m_sText;
}

void CControlUI::SetText(LPCTSTR pstrText)
{
    if( m_sText == pstrText ) return;

    m_sText = pstrText;
    Invalidate();
}

DWORD CControlUI::GetBkColor() const
{
    return m_dwBackColor;
}

void CControlUI::SetBkColor(DWORD dwBackColor)
{
    if( m_dwBackColor == dwBackColor ) return;

    m_dwBackColor = dwBackColor;
    Invalidate();
}

DWORD CControlUI::GetBkColor2() const
{
    return m_dwBackColor2;
}

void CControlUI::SetBkColor2(DWORD dwBackColor)
{
    if( m_dwBackColor2 == dwBackColor ) return;

    m_dwBackColor2 = dwBackColor;
    Invalidate();
}

DWORD CControlUI::GetBkColor3() const
{
    return m_dwBackColor3;
}

void CControlUI::SetBkColor3(DWORD dwBackColor)
{
    if( m_dwBackColor3 == dwBackColor ) return;

    m_dwBackColor3 = dwBackColor;
    Invalidate();
}

LPCTSTR CControlUI::GetBkImage()
{
    return m_sBkImage;
}

void CControlUI::SetBkImage(LPCTSTR pStrImage)
{
    if( m_sBkImage == pStrImage ) return;

    m_sBkImage = pStrImage;
    Invalidate();
}

DWORD CControlUI::GetBorderColor() const
{
    return m_dwBorderColor;
}

void CControlUI::SetBorderColor(DWORD dwBorderColor)
{
    if( m_dwBorderColor == dwBorderColor ) return;

    m_dwBorderColor = dwBorderColor;
    Invalidate();
}

DWORD CControlUI::GetFocusBorderColor() const
{
    return m_dwFocusBorderColor;
}

void CControlUI::SetFocusBorderColor(DWORD dwBorderColor)
{
    if( m_dwFocusBorderColor == dwBorderColor ) return;

    m_dwFocusBorderColor = dwBorderColor;
    Invalidate();
}

bool CControlUI::IsColorHSL() const
{
    return m_bColorHSL;
}

void CControlUI::SetColorHSL(bool bColorHSL)
{
    if( m_bColorHSL == bColorHSL ) return;

    m_bColorHSL = bColorHSL;
    Invalidate();
}

int CControlUI::GetBorderSize() const
{
    return m_nBorderSize;
}

void CControlUI::SetBorderSize(int nSize)
{
    if( m_nBorderSize == nSize ) return;

    m_nBorderSize = nSize;
    Invalidate();
}

SIZE CControlUI::GetBorderRound() const
{
    return m_cxyBorderRound;
}

void CControlUI::SetBorderRound(SIZE cxyRound)
{
    m_cxyBorderRound = cxyRound;
    Invalidate();
}

bool CControlUI::DrawImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify)
{
    return CRenderEngine::DrawImageString(hDC, m_pManager, m_rcItem, m_rcPaint, pStrImage, pStrModify);
}

const RECT& CControlUI::GetPos() const
{
    return m_rcItem;
}

void CControlUI::SetPos(RECT rc)
{
    if( rc.right < rc.left ) rc.right = rc.left;
    if( rc.bottom < rc.top ) rc.bottom = rc.top;

    CDUIRect invalidateRc = m_rcItem;
    if( ::IsRectEmpty(&invalidateRc) ) invalidateRc = rc;

    m_rcItem = rc;
    if( m_pManager == NULL ) return;

    if( !m_bSetPos ) {
        m_bSetPos = true;
        if( OnSize ) OnSize(this);
        m_bSetPos = false;
    }
    
    if( m_bFloat ) {
        CControlUI* pParent = GetParent();
        if( pParent != NULL ) {
            RECT rcParentPos = pParent->GetPos();
            if( m_cXY.cx >= 0 ) m_cXY.cx = m_rcItem.left - rcParentPos.left;
            else m_cXY.cx = m_rcItem.right - rcParentPos.right;
            if( m_cXY.cy >= 0 ) m_cXY.cy = m_rcItem.top - rcParentPos.top;
            else m_cXY.cy = m_rcItem.bottom - rcParentPos.bottom;
            m_cxyFixed.cx = m_rcItem.right - m_rcItem.left;
            m_cxyFixed.cy = m_rcItem.bottom - m_rcItem.top;
        }
    }

    m_bUpdateNeeded = false;
    invalidateRc.Join(m_rcItem);

    CControlUI* pParent = this;
    RECT rcTemp;
    RECT rcParent;
    while( pParent = pParent->GetParent() )
    {
        rcTemp = invalidateRc;
        rcParent = pParent->GetPos();
        if( !::IntersectRect(&invalidateRc, &rcTemp, &rcParent) ) 
        {
            return;
        }
    }
    m_pManager->Invalidate(invalidateRc);
}

int CControlUI::GetWidth() const
{
    return m_rcItem.right - m_rcItem.left;
}

int CControlUI::GetHeight() const
{
    return m_rcItem.bottom - m_rcItem.top;
}

int CControlUI::GetX() const
{
    return m_rcItem.left;
}

int CControlUI::GetY() const
{
    return m_rcItem.top;
}

RECT CControlUI::GetPadding() const
{
    return m_rcPadding;
}

void CControlUI::SetPadding(RECT rcPadding)
{
    m_rcPadding = rcPadding;
    NeedParentUpdate();
}

SIZE CControlUI::GetFixedXY() const
{
    return m_cXY;
}

void CControlUI::SetFixedXY(SIZE szXY)
{
    m_cXY.cx = szXY.cx;
    m_cXY.cy = szXY.cy;
    if( !m_bFloat ) NeedParentUpdate();
    else NeedUpdate();
}

int CControlUI::GetFixedWidth() const
{
    return m_cxyFixed.cx;
}

void CControlUI::SetFixedWidth(int cx)
{
    if( cx < 0 ) return; 
    m_cxyFixed.cx = cx;
    if( !m_bFloat ) NeedParentUpdate();
    else NeedUpdate();
}

int CControlUI::GetFixedHeight() const
{
    return m_cxyFixed.cy;
}

void CControlUI::SetFixedHeight(int cy)
{
    if( cy < 0 ) return; 
    m_cxyFixed.cy = cy;
    if( !m_bFloat ) NeedParentUpdate();
    else NeedUpdate();
}

int CControlUI::GetMinWidth() const
{
    return m_cxyMin.cx;
}

void CControlUI::SetMinWidth(int cx)
{
    if( m_cxyMin.cx == cx ) return;

    if( cx < 0 ) return; 
    m_cxyMin.cx = cx;
    if( !m_bFloat ) NeedParentUpdate();
    else NeedUpdate();
}

int CControlUI::GetMaxWidth() const
{
    return m_cxyMax.cx;
}

void CControlUI::SetMaxWidth(int cx)
{
    if( m_cxyMax.cx == cx ) return;

    if( cx < 0 ) return; 
    m_cxyMax.cx = cx;
    if( !m_bFloat ) NeedParentUpdate();
    else NeedUpdate();
}

int CControlUI::GetMinHeight() const
{
    return m_cxyMin.cy;
}

void CControlUI::SetMinHeight(int cy)
{
    if( m_cxyMin.cy == cy ) return;

    if( cy < 0 ) return; 
    m_cxyMin.cy = cy;
    if( !m_bFloat ) NeedParentUpdate();
    else NeedUpdate();
}

int CControlUI::GetMaxHeight() const
{
    return m_cxyMax.cy;
}

void CControlUI::SetMaxHeight(int cy)
{
    if( m_cxyMax.cy == cy ) return;

    if( cy < 0 ) return; 
    m_cxyMax.cy = cy;
    if( !m_bFloat ) NeedParentUpdate();
    else NeedUpdate();
}

void CControlUI::SetRelativePos(SIZE szMove,SIZE szZoom)
{
    m_tRelativePos.bRelative = TRUE;
    m_tRelativePos.nMoveXPercent = szMove.cx;
    m_tRelativePos.nMoveYPercent = szMove.cy;
    m_tRelativePos.nZoomXPercent = szZoom.cx;
    m_tRelativePos.nZoomYPercent = szZoom.cy;
}

void CControlUI::SetRelativeParentSize(SIZE sz)
{
    m_tRelativePos.szParent = sz;
}

TRelativePosUI CControlUI::GetRelativePos() const
{
    return m_tRelativePos;
}

bool CControlUI::IsRelativePos() const
{
    return m_tRelativePos.bRelative;
}

CStringW CControlUI::GetToolTip() const
{
    return m_sToolTip;
}

void CControlUI::SetToolTip(LPCTSTR pstrText)
{
    m_sToolTip = pstrText;
}


TCHAR CControlUI::GetShortcut() const
{
    return m_chShortcut;
}

void CControlUI::SetShortcut(TCHAR ch)
{
    m_chShortcut = ch;
}

bool CControlUI::IsContextMenuUsed() const
{
    return m_bMenuUsed;
}

void CControlUI::SetContextMenuUsed(bool bMenuUsed)
{
    m_bMenuUsed = bMenuUsed;
}

const CStringW& CControlUI::GetUserData()
{
    return m_sUserData;
}

void CControlUI::SetUserData(LPCTSTR pstrText)
{
    m_sUserData = pstrText;
}

UINT_PTR CControlUI::GetTag() const
{
    return m_pTag;
}

void CControlUI::SetTag(UINT_PTR pTag)
{
    m_pTag = pTag;
}

bool CControlUI::IsVisible() const
{
    return m_bVisible && m_bInternVisible;
}

void CControlUI::SetVisible(bool bVisible)
{
    if( m_bVisible == bVisible ) return;

    bool v = IsVisible();
    m_bVisible = bVisible;
    if( m_bFocused ) m_bFocused = false;
	if (!bVisible && m_pManager && m_pManager->GetFocus() == this) {
		m_pManager->SetFocus(NULL) ;
	}
    if( IsVisible() != v ) {
        NeedParentUpdate();
    }
}

void CControlUI::SetInternVisible(bool bVisible)
{
    m_bInternVisible = bVisible;
	if (!bVisible && m_pManager && m_pManager->GetFocus() == this) {
		m_pManager->SetFocus(NULL) ;
	}
}

bool CControlUI::IsEnabled() const
{
    return m_bEnabled;
}

void CControlUI::SetEnabled(bool bEnabled)
{
    if( m_bEnabled == bEnabled ) return;

    m_bEnabled = bEnabled;
    Invalidate();
}

bool CControlUI::IsMouseEnabled() const
{
    return m_bMouseEnabled;
}

void CControlUI::SetMouseEnabled(bool bEnabled)
{
    m_bMouseEnabled = bEnabled;
}

bool CControlUI::IsKeyboardEnabled() const
{
	return m_bKeyboardEnabled ;
}
void CControlUI::SetKeyboardEnabled(bool bEnabled)
{
	m_bKeyboardEnabled = bEnabled ; 
}

bool CControlUI::IsFocused() const
{
    return m_bFocused;
}

void CControlUI::SetFocus()
{
    if( m_pManager != NULL ) m_pManager->SetFocus(this);
}

bool CControlUI::IsFloat() const
{
    return m_bFloat;
}

void CControlUI::SetFloat(bool bFloat)
{
    if( m_bFloat == bFloat ) return;

    m_bFloat = bFloat;
    NeedParentUpdate();
}

CControlUI* CControlUI::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags)
{
    if( (uFlags & UIFIND_VISIBLE) != 0 && !IsVisible() ) return NULL;
    if( (uFlags & UIFIND_ENABLED) != 0 && !IsEnabled() ) return NULL;
    if( (uFlags & UIFIND_HITTEST) != 0 && (!m_bMouseEnabled || !::PtInRect(&m_rcItem, * static_cast<LPPOINT>(pData))) ) return NULL;
    return Proc(this, pData);
}

void CControlUI::Invalidate()
{
    if( !IsVisible() ) return;

    RECT invalidateRc = m_rcItem;

    CControlUI* pParent = this;
    RECT rcTemp;
    RECT rcParent;
    while( pParent = pParent->GetParent() )
    {
        rcTemp = invalidateRc;
        rcParent = pParent->GetPos();
        if( !::IntersectRect(&invalidateRc, &rcTemp, &rcParent) ) 
        {
            return;
        }
    }

    if( m_pManager != NULL ) m_pManager->Invalidate(invalidateRc);
}

bool CControlUI::IsUpdateNeeded() const
{
    return m_bUpdateNeeded;
}

void CControlUI::NeedUpdate()
{
    if( !IsVisible() ) return;
    m_bUpdateNeeded = true;
    Invalidate();

    if( m_pManager != NULL ) m_pManager->NeedUpdate();
}

void CControlUI::NeedParentUpdate()
{
    if( GetParent() ) {
        GetParent()->NeedUpdate();
        GetParent()->Invalidate();
    }
    else {
        NeedUpdate();
    }

    if( m_pManager != NULL ) m_pManager->NeedUpdate();
}

DWORD CControlUI::GetAdjustColor(DWORD dwColor)
{
    if( !m_bColorHSL ) return dwColor;
    short H, S, L;
    CAppData::GetHSL(&H, &S, &L);
    return CRenderEngine::AdjustColor(dwColor, H, S, L);
}

void CControlUI::Init()
{
	DoInit();
	if( OnInit ) OnInit(this);
}

void CControlUI::DoInit()
{

}

void CControlUI::Event(TEventUI& event)
{
    if( OnEvent(&event) ) DoEvent(event);
}

void CControlUI::DoEvent(TEventUI& event)
{
    if( event.Type == UIEVENT_SETCURSOR )
    {
        ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
        return;
    }
    if( event.Type == UIEVENT_SETFOCUS ) 
    {
        m_bFocused = true;
        Invalidate();
        return;
    }
    if( event.Type == UIEVENT_KILLFOCUS ) 
    {
        m_bFocused = false;
        Invalidate();
        return;
    }
    if( event.Type == UIEVENT_TIMER )
    {
        m_pManager->SendNotify(this, _T("timer"), event.wParam, event.lParam);
        return;
    }
    if( event.Type == UIEVENT_CONTEXTMENU )
    {
        if( IsContextMenuUsed() ) {
            m_pManager->SendNotify(this, _T("menu"), event.wParam, event.lParam);
            return;
        }
    }
    if( m_pParent != NULL ) m_pParent->DoEvent(event);
}

void CControlUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( EQUSTR(pstrName, _T("pos")) ) 
	{
        RECT rcPos = { 0 };
        //LPTSTR pstr = NULL;
        //rcPos.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
        //rcPos.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
        //rcPos.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
        //rcPos.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
		rcPos = FormatToRect( pstrValue );

        SIZE szXY = {rcPos.left >= 0 ? rcPos.left : rcPos.right, rcPos.top >= 0 ? rcPos.top : rcPos.bottom};
        SetFixedXY(szXY);
        SetFixedWidth(rcPos.right - rcPos.left);
        SetFixedHeight(rcPos.bottom - rcPos.top);
    }
    else if( EQUSTR(pstrName, _T("relativepos"))) {
        SIZE szMove,szZoom;
        LPTSTR pstr = NULL;
        szMove.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
        szMove.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
        szZoom.cx = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
        szZoom.cy = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr); 
        SetRelativePos(szMove,szZoom);
    }
    else if( EQUSTR(pstrName, _T("padding"))) {
        RECT rcPadding = FormatToRect( pstrValue );
        SetPadding(rcPadding);
    }
    else if( EQUSTR(pstrName, _T("bkcolor")) || EQUSTR(pstrName, _T("bkcolor1")) ) {
		DWORD cr = FormatToColor( pstrValue );
        SetBkColor( cr );
    }
    else if( EQUSTR(pstrName, _T("bkcolor2")) )
	{
		DWORD cr = FormatToColor( pstrValue );
		SetBkColor2( cr );
    }
    else if( EQUSTR(pstrName, _T("bkcolor3")) )
	{
		DWORD cr = FormatToColor( pstrValue );
        SetBkColor3( cr );
    }
    else if( EQUSTR(pstrName, _T("bordercolor")) )
	{
		DWORD cr = FormatToColor( pstrValue );
        SetBorderColor( cr );
    }
    else if( EQUSTR(pstrName, _T("focusbordercolor")) )
	{
		DWORD cr = FormatToColor( pstrValue );
        SetFocusBorderColor( cr );
    }
    else if( EQUSTR(pstrName, _T("colorhsl")) ) 
		SetColorHSL( EQUSTR_TRUE(pstrValue) );
    else if( EQUSTR(pstrName, _T("bordersize")) ) 
		SetBorderSize(_ttoi(pstrValue));
    else if( EQUSTR(pstrName, _T("borderround")) )
	{
        SIZE cxyRound = { 0 };
        LPTSTR pstr = NULL;
        cxyRound.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
        cxyRound.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);     
        SetBorderRound(cxyRound);
    }
    else if( EQUSTR(pstrName, _T("bkimage")) )		SetBkImage(pstrValue);
    else if( EQUSTR(pstrName, _T("width")) )		SetFixedWidth(_ttoi(pstrValue));
    else if( EQUSTR(pstrName, _T("height")) )		SetFixedHeight(_ttoi(pstrValue));
    else if( EQUSTR(pstrName, _T("minwidth")) )		SetMinWidth(_ttoi(pstrValue));
    else if( EQUSTR(pstrName, _T("minheight")) )	SetMinHeight(_ttoi(pstrValue));
    else if( EQUSTR(pstrName, _T("maxwidth")) )		SetMaxWidth(_ttoi(pstrValue));
    else if( EQUSTR(pstrName, _T("maxheight")) )	SetMaxHeight(_ttoi(pstrValue));
    else if( EQUSTR(pstrName, _T("name")) )			SetName(pstrValue);
    else if( EQUSTR(pstrName, _T("text")) )			SetText(pstrValue);
    else if( EQUSTR(pstrName, _T("tooltip")) )		SetToolTip(pstrValue);
    else if( EQUSTR(pstrName, _T("userdata")) )		SetUserData(pstrValue);
    else if( EQUSTR(pstrName, _T("enabled")) )		SetEnabled(EQUSTR_TRUE(pstrValue));
    else if( EQUSTR(pstrName, _T("mouse")) )		SetMouseEnabled(EQUSTR_TRUE(pstrValue));
	else if( EQUSTR(pstrName, _T("keyboard")) )		SetKeyboardEnabled(EQUSTR_TRUE(pstrValue));
    else if( EQUSTR(pstrName, _T("visible")) )		SetVisible(EQUSTR_TRUE(pstrValue));
    else if( EQUSTR(pstrName, _T("float")) )		SetFloat(EQUSTR_TRUE(pstrValue));
    else if( EQUSTR(pstrName, _T("shortcut")) )		SetShortcut(pstrValue[0]);
    else if( EQUSTR(pstrName, _T("menu")) )			SetContextMenuUsed(EQUSTR_TRUE(pstrValue));
	//else if( EQUSTR(pstrName, _T("allowdrop")) )	m_AllowDrop = EQUSTR_TRUE(pstrValue);
	else if( EQUSTR(pstrName, _T("borderstate")) )	SetBorderState(_ttoi(pstrValue));
	else if( EQUSTR(pstrName, _T("tabstop")) )		SetKeyboardEnabled(EQUSTR_TRUE(pstrValue));
}

CControlUI* CControlUI::ApplyAttributeList(LPCTSTR pstrList)
{
    CStringW sItem;
    CStringW sValue;
    while( *pstrList != _T('\0') )
	{
        sItem.Empty();
        sValue.Empty();
        while( *pstrList != _T('\0') && *pstrList != _T('=') )
		{
            LPTSTR pstrTemp = ::CharNext(pstrList);
            while( pstrList < pstrTemp)
			{
                sItem += *pstrList++;
            }
        }
        ASSERT( *pstrList == _T('=') );
        if( *pstrList++ != _T('=') ) return this;
        ASSERT( *pstrList == _T('\"') );
        if( *pstrList++ != _T('\"') ) return this;
        while( *pstrList != _T('\0') && *pstrList != _T('\"') )
		{
            LPTSTR pstrTemp = ::CharNext(pstrList);
            while( pstrList < pstrTemp) 
			{
                sValue += *pstrList++;
            }
        }
        ASSERT( *pstrList == _T('\"') );
        if( *pstrList++ != _T('\"') )
			return this;

        SetAttribute(sItem, sValue);
        if( *pstrList++ != _T(' ') ) 
			return this;
    }
    return this;
}

SIZE CControlUI::EstimateSize(SIZE szAvailable)
{
    return m_cxyFixed;
}

void CControlUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
    if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;

    // »æÖÆÑ­Ðò£º±³¾°ÑÕÉ«->±³¾°Í¼->×´Ì¬Í¼->ÎÄ±¾->±ß¿ò
    if( m_cxyBorderRound.cx > 0 || m_cxyBorderRound.cy > 0 ) {
        CRenderClip roundClip;
        CRenderClip::GenerateRoundClip(hDC, m_rcPaint,  m_rcItem, m_cxyBorderRound.cx, m_cxyBorderRound.cy, roundClip);
        //PaintBkColor(hDC);
        //PaintBkImage(hDC);
        //PaintStatusImage(hDC);
        //PaintText(hDC);
        //PaintBorder(hDC);
    }
    //else {
    //    PaintBkColor(hDC);
    //    PaintBkImage(hDC);
    //    PaintStatusImage(hDC);
    //    PaintText(hDC);
    //    PaintBorder(hDC);
    //}

	PaintBkColor(hDC);
	PaintBkImage(hDC);
	PaintStatusImage(hDC);
	PaintText(hDC);
	PaintBorder(hDC);
}

void CControlUI::PaintBkColor(HDC hDC)
{
    if( m_dwBackColor != 0 ) {
        if( m_dwBackColor2 != 0 ) {
            if( m_dwBackColor3 != 0 ) {
                RECT rc = m_rcItem;
                rc.bottom = (rc.bottom + rc.top) / 2;
                CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwBackColor), GetAdjustColor(m_dwBackColor2), true, 8);
                rc.top = rc.bottom;
                rc.bottom = m_rcItem.bottom;
                CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwBackColor2), GetAdjustColor(m_dwBackColor3), true, 8);
            }
            else 
                CRenderEngine::DrawGradient(hDC, m_rcItem, GetAdjustColor(m_dwBackColor), GetAdjustColor(m_dwBackColor2), true, 16);
        }
        else if( m_dwBackColor >= 0xFF000000 ) CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwBackColor));
        else CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwBackColor));
    }
}

void CControlUI::PaintBkImage(HDC hDC)
{
	if( m_sBkImage.IsEmpty() ) return;
	if( !DrawImage(hDC, (LPCTSTR)m_sBkImage) ) m_sBkImage.Empty();
}

void CControlUI::PaintStatusImage(HDC hDC)
{
    return;
}

void CControlUI::PaintText(HDC hDC)
{
    return;
}

void CControlUI::PaintBorder(HDC hDC)
{
	if( m_nBorderSize > 0 )
	{
        if( m_cxyBorderRound.cx > 0 || m_cxyBorderRound.cy > 0 )
		{
			if (IsFocused() && m_dwFocusBorderColor != 0)
				CRenderEngine::DrawRoundRect(hDC, m_rcItem, m_nBorderSize, m_cxyBorderRound.cx, m_cxyBorderRound.cy, GetAdjustColor(m_dwFocusBorderColor));
			else
				CRenderEngine::DrawRoundRect(hDC, m_rcItem, m_nBorderSize, m_cxyBorderRound.cx, m_cxyBorderRound.cy, GetAdjustColor(m_dwBorderColor));
		}
		else
		{
			if (IsFocused() && m_dwFocusBorderColor != 0)
				CRenderEngine::DrawRect(hDC, m_rcItem, m_nBorderSize, GetAdjustColor(m_dwFocusBorderColor));
			else if (m_dwBorderColor != 0)
			{
				//CRenderEngine::DrawRect(hDC, m_rcItem, m_nBorderSize, GetAdjustColor(m_dwBorderColor));
				if (m_nBorderState == 15)
				{
					CRenderEngine::DrawRect(hDC, m_rcItem, m_nBorderSize, m_dwBorderColor);
				}
				else
				{
					if (m_nBorderState & 1)
					{
						RECT rectLine;

						rectLine.bottom = rectLine.top = m_rcItem.top;
						rectLine.left = m_rcItem.left;
						rectLine.right = m_rcItem.right;
						CRenderEngine::DrawLine(hDC,rectLine, m_nBorderSize, m_dwBorderColor);
					}
					
					if (m_nBorderState & 2)
					{
						RECT rectLine;

						rectLine.bottom =  m_rcItem.bottom;
						rectLine.top = m_rcItem.top;
						rectLine.left = rectLine.right = m_rcItem.right-1;
						CRenderEngine::DrawLine(hDC,rectLine, m_nBorderSize, m_dwBorderColor);
					}
					
					if (m_nBorderState & 4)
					{
						RECT rectLine;

						rectLine.bottom = rectLine.top = m_rcItem.bottom-1;
						rectLine.left = m_rcItem.left;
						rectLine.right = m_rcItem.right;
						CRenderEngine::DrawLine(hDC,rectLine, m_nBorderSize, m_dwBorderColor);
					}
					
					if (m_nBorderState & 8)
					{
						RECT rectLine;

						rectLine.bottom =  m_rcItem.bottom;
						rectLine.top = m_rcItem.top;
						rectLine.left = rectLine.right = m_rcItem.left;
						CRenderEngine::DrawLine(hDC,rectLine, m_nBorderSize, m_dwBorderColor);
					}
				}
			}
		}
	}
}

void CControlUI::DoPostPaint(HDC hDC, const RECT& rcPaint)
{
    return;
}

//////////////////////////////////////////////////////////////////////////extends

//DROPEFFECT CControlUI::DoDragDrop(CDataObject* data)
//{
//	DROPEFFECT effect = DropEffectDeny;
//	CPaintManagerUI* pPM = GetManager();
//	if (pPM && data)
//	{			
//		CDragEventArgs e;
//		e.Data = data;
//		e.pt.x = GET_X_LPARAM(GetMessagePos());
//		e.pt.y = GET_Y_LPARAM(GetMessagePos());
//		pPM->m_IsDoDragDroping = true;
//		pPM->m_DragEventArgs = &e;
//		pPM->m_DragSource = this;
//		MSG msg = { 0 };
//		while(pPM->m_IsDoDragDroping) 
//		{
//			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
//				::TranslateMessage(&msg);
//				::DispatchMessage(&msg);
//			}
//		}
//		pPM->m_DragEventArgs = NULL;
//		pPM->m_DropTarget = NULL;
//		pPM->m_DragSource = NULL;
//		effect = e.Effect;
//	}
//	return effect;
//}
//
//HCURSOR CControlUI::GetFeedback( DRAGDROPSTEP step )
//{
//	return NULL;
//}

void CControlUI::SetBorderState( int state )
{
	m_nBorderState = state;
}

// 20160319
DWORD CControlUI::FormatToColor( LPCTSTR pstrValue )
{
	while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) 
		pstrValue = ::CharNext(pstrValue);

	if( *pstrValue == _T('#')) 
		pstrValue = ::CharNext(pstrValue);

	return _tcstoul(pstrValue, 0, 16);
}

RECT CControlUI::FormatToRect( LPCTSTR pstrValue )
{
	RECT rc = { 0 };
	LPTSTR pstr = NULL;
	rc.left		= _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
	rc.top		= _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
	rc.right	= _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
	rc.bottom	= _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr); 
	return rc;
}

//} // namespace DuiLib
