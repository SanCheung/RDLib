#include "StdAfx.h"
#include "OptionUI.h"

//
//namespace DuiLib
//{


COptionUI::COptionUI() : m_bSelected(false), m_dwSelectedTextColor(0)
{
}

COptionUI::~COptionUI()
{
	if( !m_sGroupName.IsEmpty() && m_pManager ) m_pManager->RemoveOptionGroup(m_sGroupName, this);
}

LPCTSTR COptionUI::GetClass() const
{
	return _T("OptionUI");
}

LPVOID COptionUI::GetInterface(LPCTSTR pstrName)
{
	if( EQUSTR(pstrName, _T("Option")) )
		return static_cast<COptionUI*>(this);

	return CButtonUI::GetInterface(pstrName);
}

void COptionUI::SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit)
{
	CControlUI::SetManager(pManager, pParent, bInit);
	if( bInit && !m_sGroupName.IsEmpty() ) {
		if (m_pManager) m_pManager->AddOptionGroup(m_sGroupName, this);
	}
}

LPCTSTR COptionUI::GetGroup() const
{
	return m_sGroupName;
}

void COptionUI::SetGroup(LPCTSTR pStrGroupName)
{
	if( pStrGroupName == NULL ) {
		if( m_sGroupName.IsEmpty() ) return;
		m_sGroupName.Empty();
	}
	else {
		if( m_sGroupName == pStrGroupName ) return;
		if (!m_sGroupName.IsEmpty() && m_pManager) m_pManager->RemoveOptionGroup(m_sGroupName, this);
		m_sGroupName = pStrGroupName;
	}

	if( !m_sGroupName.IsEmpty() ) {
		if (m_pManager) m_pManager->AddOptionGroup(m_sGroupName, this);
	}
	else {
		if (m_pManager) m_pManager->RemoveOptionGroup(m_sGroupName, this);
	}

	Selected(m_bSelected);
}

bool COptionUI::IsSelected() const
{
	return m_bSelected;
}

void COptionUI::Selected(bool bSelected)
{
	if( m_bSelected == bSelected ) return;
	m_bSelected = bSelected;
	if( m_bSelected ) m_uButtonState |= UISTATE_SELECTED;
	else m_uButtonState &= ~UISTATE_SELECTED;

	if( m_pManager != NULL ) {
		if( !m_sGroupName.IsEmpty() ) {
			if( m_bSelected ) {
				//CStdPtrArray* aOptionGroup = m_pManager->GetOptionGroup(m_sGroupName);
				//for( int i = 0; i < aOptionGroup->GetSize(); i++ ) {
				//	COptionUI* pControl = static_cast<COptionUI*>(aOptionGroup->GetAt(i));
				//	if( pControl != this ) {
				//		pControl->Selected(false);
				//	}
				//}
				vectorv *v = m_pManager->GetOptionGroup( m_sGroupName );
				for( auto it = v->begin(); it != v->end(); ++it )
				{
					COptionUI *pControl = static_cast<COptionUI*>( *it );
					if( pControl != this )
						pControl->Selected(false);
				}

				m_pManager->SendNotify(this, _T("selectchanged"));
			}
		}
		else {
			m_pManager->SendNotify(this, _T("selectchanged"));
		}
	}

	Invalidate();
}

bool COptionUI::Activate()
{
	if( !CButtonUI::Activate() ) return false;
	if( !m_sGroupName.IsEmpty() ) Selected(true);
	else Selected(!m_bSelected);

	return true;
}

void COptionUI::SetEnabled(bool bEnable)
{
	CControlUI::SetEnabled(bEnable);
	if( !IsEnabled() ) {
		if( m_bSelected ) m_uButtonState = UISTATE_SELECTED;
		else m_uButtonState = 0;
	}
}

LPCTSTR COptionUI::GetSelectedImage()
{
	return m_sSelectedImage;
}

void COptionUI::SetSelectedImage(LPCTSTR pStrImage)
{
	m_sSelectedImage = pStrImage;
	Invalidate();
}

void COptionUI::SetSelectedTextColor(DWORD dwTextColor)
{
	m_dwSelectedTextColor = dwTextColor;
}

DWORD COptionUI::GetSelectedTextColor()
{
	if (m_dwSelectedTextColor == 0) m_dwSelectedTextColor = m_pManager->GetDefaultFontColor();
	return m_dwSelectedTextColor;
}

LPCTSTR COptionUI::GetForeImage()
{
	return m_sForeImage;
}

void COptionUI::SetForeImage(LPCTSTR pStrImage)
{
	m_sForeImage = pStrImage;
	Invalidate();
}

SIZE COptionUI::EstimateSize(SIZE szAvailable)
{
	if( m_cxyFixed.cy == 0 ) return CDUISize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 8);
	return CControlUI::EstimateSize(szAvailable);
}

void COptionUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( EQUSTR(pstrName, _T("group")) ) 
		SetGroup(pstrValue);
	else if( EQUSTR(pstrName, _T("selected")) ) 
		Selected( EQUSTR(pstrValue, _T("true")) );
	else if( EQUSTR(pstrName, _T("selectedimage")) ) 
		SetSelectedImage(pstrValue);
	else if( EQUSTR(pstrName, _T("foreimage")) ) 
		SetForeImage(pstrValue);
	else if( EQUSTR(pstrName, _T("selectedtextcolor")) ) 
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetSelectedTextColor(clrColor);
	}
	else 
		CButtonUI::SetAttribute(pstrName, pstrValue);
}

void COptionUI::PaintStatusImage(HDC hDC)
{
	m_uButtonState &= ~UISTATE_PUSHED;

	if( (m_uButtonState & UISTATE_SELECTED) != 0 ) {
		if( !m_sSelectedImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sSelectedImage) ) 
				m_sSelectedImage.Empty();
			else 
				goto Label_ForeImage;
		}
	}

	CButtonUI::PaintStatusImage(hDC);

Label_ForeImage:
	if( !m_sForeImage.IsEmpty() ) {
		if( !DrawImage(hDC, (LPCTSTR)m_sForeImage) ) m_sForeImage.Empty();
	}
}

void COptionUI::PaintText(HDC hDC)
{
	if( (m_uButtonState & UISTATE_SELECTED) != 0 )
	{
		DWORD oldTextColor = m_dwTextColor;
		if( m_dwSelectedTextColor != 0 ) m_dwTextColor = m_dwSelectedTextColor;

		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

		if( m_sText.IsEmpty() ) return;
		int nLinks = 0;
		RECT rc = m_rcItem;
		rc.left += m_rcTextPadding.left;
		rc.right -= m_rcTextPadding.right;
		rc.top += m_rcTextPadding.top;
		rc.bottom -= m_rcTextPadding.bottom;

		if( m_bShowHtml )
			CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, IsEnabled()?m_dwTextColor:m_dwDisabledTextColor, \
			m_iFont, NULL, NULL, nLinks, m_uTextStyle);
		else
			CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, IsEnabled()?m_dwTextColor:m_dwDisabledTextColor, \
			m_iFont, m_uTextStyle);

		m_dwTextColor = oldTextColor;
	}
	else
		CButtonUI::PaintText(hDC);
}


//};
