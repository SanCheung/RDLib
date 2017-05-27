#include "StdAfx.h"
#include "ComboUIEx.h"
#include "ListLabelElementUIEx.h"
#include "SXUtil.h"
#include "SXPicture.h"

CComboUIEx::CComboUIEx(void) : m_iCurShow(-1)
{
#ifdef ZOOM_LOAD
	m_bLoading = false;
	m_sequence = NULL;
#endif

	m_bItemSelect = true;
}

CComboUIEx::~CComboUIEx(void)
{
	int a = 0;
#ifdef ZOOM_LOAD
	DestroySequence();
#endif
}

int CComboUIEx::expand()
{
	if (m_iCurSel >= 0)
	{
		m_iCurShow = m_iCurSel;
		HWND hWnd = ::FindWindow(_T("ComboWnd"), NULL);
		SendNotifyMessage(hWnd, WM_PAINT, 0, 0);		// 在Paint中确定各个项目坐标
		SendNotifyMessage(hWnd, WM_KEYDOWN, 0x53, 0);	// 按下键盘，自动跳到选中位置
	}
	return m_iCurSel;
}

Image* CComboUIEx::LoadImage(int iIndex, LPCWSTR strImage)
{
	CListLabelElementUIEx* pLLE = (CListLabelElementUIEx*)m_items[iIndex];
	if (pLLE != NULL)
		return pLLE->LoadImage(strImage);

	return NULL;
}

void CComboUIEx::AddItem(int iIndex, CListLabelElementUIEx* pItem)
{
	CStringW str;
	pItem->SetPadding(CDUIRect(0, m_cbProperty.btLLEGap, 0, 0));
	
	str.Format( _T("%d"), m_cbProperty.wLLEHeight);
	pItem->SetAttribute(_T("height"), str);
	
	str.Format( _T("%d"), iIndex + 1);
	pItem->SetText(str);
	Add(pItem);
}

void CComboUIEx::PaintText(HDC hDC)
{
	CDUIRect rcText(m_rcItem);
 	rcText.DeflateRect(&m_rcTextPadding);

	if (m_iCurShow >= 0)
	{
		TCHAR szText[4];
		_itot(m_iCurShow + 1, szText, 10);
		CRenderEngine::DrawText(hDC, m_pManager, rcText, szText, 0, 1, DT_LEFT | DT_VCENTER);
	}
}

void CComboUIEx::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_KEYDOWN)
	{
		m_iCurSel = m_iCurShow;
	}
	/*else if (event.Type == UIEVENT_SCROLLWHEEL)
	{
	m_bItemSelect = false;
	}*/
	//2016-5-18修改去掉鼠标滚动选择
	//CComboUI::DoEvent(event);
	if (event.Type == UIEVENT_KEYDOWN)
	{
		m_iCurShow = m_iCurSel;
	}
	else if (event.Type == UIEVENT_SCROLLWHEEL)
	{
		m_bItemSelect = true;
	}
}

void CComboUIEx::DoPaint(HDC hDC, const RECT& rcPaint)
{
	Graphics g(m_pManager->GetPaintDC());
	if (0 == m_rcDB.Width)
		InitComboShadow(2, 0, 6, 10);
	if (IsExpand())
	{
		CSXPicture::DrawShadow(g, m_shadowDBPath, m_shadowDBRgn, 0xff675f5f);
	}
	else
	{
		DWORD clrShadow = m_pManager->GetRoot()->GetBkColor();
		SolidBrush brush(clrShadow);
		g.FillRegion(&brush, &m_shadowDBRgn);
	}
	CComboUI::DoPaint(hDC, rcPaint);	
}

bool CComboUIEx::SelectItem(int iIndex, bool bTakeFocus /* = false */)
{
//	if( m_pWindow != NULL ) m_pWindow->Close();
	if( iIndex == m_iCurSel ) return true;
	int iOldSel = m_iCurSel;
	if( m_iCurSel >= 0 ) {
		CControlUI* pControl = static_cast<CControlUI*>(m_items[m_iCurSel]);
		if( !pControl ) return false;
		IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
		if( pListItem != NULL ) pListItem->Select(false);
		m_iCurSel = -1;
	}
	if( iIndex < 0 ) return false;

	int n = (int)m_items.size(); //m_items.GetSize()
	if( n == 0 ) return false;
	if( iIndex >= n ) iIndex = n - 1;
	CControlUI* pControl = static_cast<CControlUI*>(m_items[iIndex]);
	if( !pControl /*|| !pControl->IsVisible() || !pControl->IsEnabled() */) return false;
	IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
	if( pListItem == NULL ) return false;
	m_iCurSel = iIndex;
	if( m_pWindow != NULL || bTakeFocus ) pControl->SetFocus();
	pListItem->Select(true);
	
	UpdateItem(iOldSel);
	
	Invalidate();

	return true;
}

bool CComboUIEx::SetCBProperty(LPCVOID data, int flag)
{
	bool bSuccess = true;
	switch (flag)
	{
	case FLAG_LLE_IMAGE_WIDTH:
		m_cbProperty.wLLEImageWidth = (UINT16)data;
		break;
	case FLAG_LLE_IMAGE_HEIGHT:
		m_cbProperty.wLLEImageHeight = (UINT16)data;
		break;
	case FLAG_LLE_HEIGHT:
		m_cbProperty.wLLEHeight = (UINT16)data;
		break;
	case FLAG_LLE_GAP:
		m_cbProperty.btLLEGap = (UINT8)data;
		break;
	case FLAG_LLE_COUNT:
		m_cbProperty.btLLECount = (UINT8)data;
		break;
	case FLAG_DB_MIN_WIDTH:
		m_cbProperty.wDBMinWidth = (UINT16)data;
		break;
	default:
		bSuccess = false;
		break;
	}
	return bSuccess;
}

bool CComboUIEx::SetColorProperty(DWORD dwClr1, DWORD dwClr2, int flag)
{
	bool bSuccess = true;
	switch (flag)
	{
	case FLAG_CLR_SELECT:
		m_cbProperty.dwClrSelected1 = dwClr1;
		m_cbProperty.dwClrSelected2 = dwClr2;
		break;
	case FLAG_CLR_HOT:
		m_cbProperty.dwClrHot1 = dwClr1;
		m_cbProperty.dwClrHot2 = dwClr2;
		break;
	default:
		bSuccess = false;
		break;
	}
	return bSuccess;
}

void CComboUIEx::SetDBSize(UINT8 btPageCount /* = 4 */, UINT16 wDBMinWidth /* = 215 */)
{
	m_cbProperty.wDBMinWidth = wDBMinWidth;				// 设置DropBox最小宽度
	int height = m_cbProperty.GetDBHeight(btPageCount);
	int width = GetWidth();
	if (width < m_cbProperty.wDBMinWidth)  width = m_cbProperty.wDBMinWidth;
	SetDropBoxSize(CDUISize(width, height));
}

void CComboUIEx::UpdateItem(int iOldSel)
{
	HWND hWnd = FindWindow(_T("ComboWnd"), NULL);
	if (hWnd == NULL)
	{// Combo下拉框未展开
		m_iCurShow = m_iCurSel;
		if( m_pManager != NULL )
			m_pManager->SendNotify(this, _T("itemselect"), m_iCurSel, iOldSel);
	}
	else
	{// Combo下拉框展开
		if (m_bItemSelect)
		{
			if( m_pManager != NULL)
				m_pManager->SendNotify(this, _T("itemselect"), m_iCurShow, iOldSel);
		}
	}
}

bool CComboUIEx::IsExpand()
{
	HWND hWndCombo = FindWindow(_T("ComboWnd"), NULL);
	return (hWndCombo != NULL) ? true : false;
}

void CComboUIEx::InitComboShadow(BYTE left, BYTE top, BYTE right, BYTE bottom)
{
	RECT rect = GetPos();
	SIZE sz = GetDropBoxSize();
	m_rcDB.X = rect.left;
	m_rcDB.Y = rect.bottom;
	m_rcDB.Width = sz.cx;
	m_rcDB.Height = sz.cy;
	CSXPicture::SetShadowRP(m_rcDB, left, top, right, bottom, m_shadowDBRgn, m_shadowDBPath);
}

#ifdef ZOOM_LOAD
bool CComboUIEx::InitSequence()
{
	bool bRealloc = false;
	int count = GetCount();
	if (m_sequence == NULL)
	{
		m_sequence = new int[count];
		bRealloc = true;
	}
	CSXUtil::RandSeqence(0, count - 1, m_sequence, count);
	return bRealloc;
}

bool CComboUIEx::DestroySequence()
{
	if (m_sequence != NULL)
	{
		delete[] m_sequence;
		m_sequence = NULL;
	}
	return false;
}
#endif