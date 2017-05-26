#include "StdAfx.h"
#include "TextUI2.h"


CTextUI2::CTextUI2(void)
{
}


CTextUI2::~CTextUI2(void)
{
}

SIZE CTextUI2::EstimateSize(SIZE szAvailable)
{
	HDC		hdc = m_pManager->GetPaintDC();

	RECT rcText = { 0, 0, 
		MAX(szAvailable.cx, m_cxyFixed.cx), 
		MAX(szAvailable.cy, m_cxyFixed.cy) };
	rcText.left += m_rcTextPadding.left;
	rcText.right -= m_rcTextPadding.right;
	if( m_bShowHtml ) {   
		int nLinks = 0;
		CRenderEngine::DrawHtmlText( hdc, m_pManager, rcText, m_sText, m_dwTextColor, \
			m_iFont, NULL, NULL, nLinks, DT_CALCRECT | m_uTextStyle);
	}
	else {
		CRenderEngine::DrawText( hdc, m_pManager, rcText, m_sText, m_dwTextColor, \
			m_iFont, DT_CALCRECT | m_uTextStyle);
	}
	SIZE cXY = {rcText.right - rcText.left + m_rcTextPadding.left + m_rcTextPadding.right,
		rcText.bottom - rcText.top + m_rcTextPadding.top + m_rcTextPadding.bottom};

	if( m_cxyFixed.cy != 0 ) cXY.cy = m_cxyFixed.cy;
	return cXY;
}
