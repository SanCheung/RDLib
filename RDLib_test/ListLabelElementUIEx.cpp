#include "StdAfx.h"
#include "ListLabelElementUIEx.h"
#include "ComboUIEx.h"
#include "SXPicture.h"

CListLabelElementUIEx::CListLabelElementUIEx(void)
{
	ZeroMemory(&m_rcInner, sizeof(m_rcInner));
	m_image = NULL;
}

CListLabelElementUIEx::~CListLabelElementUIEx(void)
{
	FreeImage();
}

void CListLabelElementUIEx::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_BUTTONDOWN)  return;
	else if(event.Type == UIEVENT_BUTTONUP)
	{
		CComboUIEx* pCombo = (CComboUIEx*)m_pOwner;
		pCombo->SelectItem(pCombo->m_iCurShow);		// 先恢复当前选中项目
		pCombo->m_iCurShow = m_iIndex;
		event.Type = UIEVENT_BUTTONDOWN;			// UP事件按DOWN事件处理
	}

	CListLabelElementUI::DoEvent(event);
}

void CListLabelElementUIEx::DoPaint(HDC hDC, const RECT& rcPaint)
{
	if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;
//  使用GDI+进行绘图
	// 初始化绘图范围
	Rect rcItem(m_rcItem.left, m_rcItem.top,							// LLE范围
		m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top);
	if (m_rcInner.left == 0)
		SetInnerRect();							// 确定内距
	Rect rcDraw(rcItem.X + m_rcInner.left, rcItem.Y + m_rcInner.top,	// 图片范围
		rcItem.Width - m_rcInner.left - m_rcInner.right, rcItem.Height - m_rcInner.top - m_rcInner.bottom);

	// 绘图相关变量
	CComboUIEx* pCombo = (CComboUIEx*)m_pOwner;
	ComboProperty& cbProperty = pCombo->m_cbProperty;
	Graphics g(hDC);
	GraphicsPath shadowPath;
	Region shadowRgn;

	// 图片文本绘制(因为所使用的GDI+不支持多字节字符，方便起见采用开源库中的方法进行绘制)
	CDUIRect rcText = m_rcItem;
	rcText.right = rcText.left + 25;
	rcText.bottom = rcText.top + 25;
	DrawItemText(hDC, rcText);

	// 图片及阴影绘制
	CSXPicture::SetShadowRP(rcDraw, 4, 4, 6, 7, shadowRgn, shadowPath, true);
	CSXPicture::DrawShadow(g, shadowPath, shadowRgn, 0x92404040);

	if (m_image != NULL)
	{
		g.DrawImage(m_image, rcDraw.X, rcDraw.Y, rcDraw.Width, rcDraw.Height);
	}
	else
	{
		rcText = m_rcItem;
		rcText.DeflateRect(&m_rcInner);
		CRenderEngine::DrawText(hDC, m_pManager, rcText, _T("Loading..."), 0xff000000, 2, DT_CENTER | DT_VCENTER);
	}

	ItemState state = GetItemState();
	// 根据状态绘制决定是否绘制背景
	if (state == ITEM_STATE_HOT)		// HOT背景
	{
		CSXPicture::SetShadowRP(rcDraw, m_rcInner.left,
			m_rcInner.top, m_rcInner.right, m_rcInner.bottom, shadowRgn, shadowPath);
		LinearGradientBrush lBrush(rcItem, cbProperty.dwClrHot1,
			cbProperty.dwClrHot2, LinearGradientModeVertical);
		g.FillRegion(&lBrush, &shadowRgn);
	}
	else if (state == ITEM_STATE_SEL)	// SEL背景
	{
		CSXPicture::SetShadowRP(rcDraw, m_rcInner.left,
			m_rcInner.top, m_rcInner.right, m_rcInner.bottom, shadowRgn, shadowPath);
		LinearGradientBrush lBrush(rcItem, cbProperty.dwClrSelected1,
			cbProperty.dwClrSelected2, LinearGradientModeVertical);
		g.FillRegion(&lBrush, &shadowRgn);
	}
}

void CListLabelElementUIEx::SetInnerRect()
{
	CComboUIEx* pCombo = (CComboUIEx*)m_pOwner;
	int width = GetWidth();
	int height = GetHeight();
	m_rcInner.right = (width - pCombo->GetImageWidth()) / 5;	// 左右内距4：1
	m_rcInner.left = m_rcInner.right * 4;
	m_rcInner.top = (height - pCombo->GetImageHeight()) / 2;
	m_rcInner.bottom = m_rcInner.top;
}

bool CListLabelElementUIEx::DrawImage(CComboUIEx* pCombo, HDC hDC, RECT& rcDraw, LPCTSTR pStrModify /* = NULL */)
{
	if (m_image != NULL)
	{// 图片已设置
		Graphics graphcs(hDC);

		CSXPicture::DrawShadow(graphcs, rcDraw, 3, 3, 5, 5);
		graphcs.DrawImage(m_image, rcDraw.left, rcDraw.top, m_image->GetWidth(), m_image->GetHeight());
		return true;
	}
	else
	{// 图片未设置，绘制文本..................
		CRenderEngine::DrawColor(hDC, rcDraw, 0xffffffff);
		CRenderEngine::DrawText(hDC, m_pManager, rcDraw, _T("Loading..."), 0xff000000, 2, DT_CENTER | DT_VCENTER);
		return false;
	}
}

bool CListLabelElementUIEx::FreeImage()
{
	if (NULL != m_image)
	{
		delete m_image;
		m_image = NULL;
		return true;
	}
	return false;
}

Image* CListLabelElementUIEx::LoadImage(LPCWSTR strImage)
{
	if (NULL != m_image)
	{// 已有GDI+对象，需要重新加载
		delete m_image;
		m_image = NULL;
	}
	m_image = m_image->FromFile(strImage, FALSE);
	if (m_image == NULL)
	{
		DWORD dwErr = GetLastError();
		int a = 0;
	}
	return m_image;
}

ItemState CListLabelElementUIEx::GetItemState()
{
	// 确定鼠标是否在LLE上
	POINT point;
	GetCursorPos(&point);
	HWND hWnd = FindWindow(_T("ComboWnd"), NULL);
	ScreenToClient(hWnd, &point);
	if (PtInRect(&m_rcItem, point))
		return ITEM_STATE_HOT;
	// 判断当前页是否为选中项
	CComboUIEx* pCombo = (CComboUIEx*)m_pOwner;
	if (m_iIndex == pCombo->m_iCurShow)
		return ITEM_STATE_SEL;
	// 默认Normal状态：既不Hot，又不Sel
	return ITEM_STATE_NORMAL;
}
