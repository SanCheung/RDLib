#include "StdAfx.h"

#include "LabelUI.h"
#include "ProgressUI.h"

//namespace DuiLib
//{

CProgressUI::CProgressUI() : m_bHorizontal(true), m_nMin(0), m_nMax(100), m_nValue(0),m_ProgressForeColor(0),m_ProgressForeColor2(0)
{
	m_uTextStyle = DT_SINGLELINE | DT_CENTER;
}

LPCTSTR CProgressUI::GetClass() const
{
	return _T("ProgressUI");
}

LPVOID CProgressUI::GetInterface(LPCTSTR pstrName)
{
	if( EQUSTR(pstrName, _T("Progress")) ) 
		return static_cast<CProgressUI*>(this);
	
	return CLabelUI::GetInterface(pstrName);
}

bool CProgressUI::IsHorizontal()
{
	return m_bHorizontal;
}

void CProgressUI::SetHorizontal(bool bHorizontal)
{
	if( m_bHorizontal == bHorizontal ) return;

	m_bHorizontal = bHorizontal;
	Invalidate();
}

int CProgressUI::GetMinValue() const
{
	return m_nMin;
}

void CProgressUI::SetMinValue(int nMin)
{
	m_nMin = nMin;
	Invalidate();
}

int CProgressUI::GetMaxValue() const
{
	return m_nMax;
}

void CProgressUI::SetMaxValue(int nMax)
{
	m_nMax = nMax;
	Invalidate();
}

int CProgressUI::GetValue() const
{
	return m_nValue;
}

void CProgressUI::SetValue(int nValue)
{
	m_nValue = nValue;
	Invalidate();
}

LPCTSTR CProgressUI::GetForeImage() const
{
	return m_sForeImage;
}

void CProgressUI::SetForeImage(LPCTSTR pStrImage)
{
	if( m_sForeImage == pStrImage ) return;

	m_sForeImage = pStrImage;
	Invalidate();
}

void CProgressUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( EQUSTR(pstrName, _T("foreimage")) ) 
		SetForeImage(pstrValue);
	else if( EQUSTR(pstrName, _T("hor")) ) 
		SetHorizontal( EQUSTR(pstrValue, _T("true")) );
	else if( EQUSTR(pstrName, _T("min")) ) 
		SetMinValue(_ttoi(pstrValue));
	else if( EQUSTR(pstrName, _T("max")) ) 
		SetMaxValue(_ttoi(pstrValue));
	else if( EQUSTR(pstrName, _T("value")) ) 
		SetValue(_ttoi(pstrValue));
	else if( EQUSTR(pstrName, _T("progressforecolor")) )
	{
		if (pstrValue && pstrValue[0] == _T('#'))
		{		
			pstrValue = ::CharNext(pstrValue);
		}
		LPTSTR pstr = NULL;
		m_ProgressForeColor = _tcstoul(pstrValue, &pstr, 16);
	}
	else if( EQUSTR(pstrName, _T("progressforecolor2")) )
	{	
		if (pstrValue && pstrValue[0] == _T('#'))
		{		
			pstrValue = ::CharNext(pstrValue);
		}
		LPTSTR pstr = NULL;
		m_ProgressForeColor2 = _tcstoul(pstrValue, &pstr, 16);
	}
	else
		CLabelUI::SetAttribute(pstrName, pstrValue);
}

void CProgressUI::PaintStatusImage(HDC hDC)
{
	if( m_nMax <= m_nMin ) m_nMax = m_nMin + 1;
	if( m_nValue > m_nMax ) m_nValue = m_nMax;
	if( m_nValue < m_nMin ) m_nValue = m_nMin;

	RECT rc = {0};
	if( m_bHorizontal ) {
		rc.right = (m_nValue - m_nMin) * (m_rcItem.right - m_rcItem.left) / (m_nMax - m_nMin);
		rc.bottom = m_rcItem.bottom - m_rcItem.top;
	}
	else {
		rc.top = (m_rcItem.bottom - m_rcItem.top) * (m_nMax - m_nValue) / (m_nMax - m_nMin);
		rc.right = m_rcItem.right - m_rcItem.left;
		rc.bottom = m_rcItem.bottom - m_rcItem.top;
	}

	if( !m_sForeImage.IsEmpty() ) {
		m_sForeImageModify.Empty();
		m_sForeImageModify.Format(_T("dest='%d,%d,%d,%d'"), rc.left, rc.top, rc.right, rc.bottom);

		if( !DrawImage(hDC, (LPCTSTR)m_sForeImage, (LPCTSTR)m_sForeImageModify) ) m_sForeImage.Empty();
		else return;
	}
	else
	{
		CDUIRect rectPos = m_rcItem;				
		if (m_bHorizontal){
			rectPos.right = rectPos.left + rc.right;
		}
		else{
			rectPos.top =(rectPos.bottom - (rc.bottom - rc.top));
		}

		if (m_ProgressForeColor2 != 0){
			CRenderEngine::DrawGradient(hDC, rectPos, m_ProgressForeColor, m_ProgressForeColor2, !m_bHorizontal, 0);
		}
		else if(m_ProgressForeColor != 0){
			CRenderEngine::DrawColor(hDC, rectPos, m_ProgressForeColor);
		}
	}
}

SIZE CProgressUI::EstimateSize( SIZE szAvailable )
{
	return CControlUI::EstimateSize(szAvailable);
}


//};
