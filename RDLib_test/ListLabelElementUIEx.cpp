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
		pCombo->SelectItem(pCombo->m_iCurShow);		// �Ȼָ���ǰѡ����Ŀ
		pCombo->m_iCurShow = m_iIndex;
		event.Type = UIEVENT_BUTTONDOWN;			// UP�¼���DOWN�¼�����
	}

	CListLabelElementUI::DoEvent(event);
}

void CListLabelElementUIEx::DoPaint(HDC hDC, const RECT& rcPaint)
{
	if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;
//  ʹ��GDI+���л�ͼ
	// ��ʼ����ͼ��Χ
	Rect rcItem(m_rcItem.left, m_rcItem.top,							// LLE��Χ
		m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top);
	if (m_rcInner.left == 0)
		SetInnerRect();							// ȷ���ھ�
	Rect rcDraw(rcItem.X + m_rcInner.left, rcItem.Y + m_rcInner.top,	// ͼƬ��Χ
		rcItem.Width - m_rcInner.left - m_rcInner.right, rcItem.Height - m_rcInner.top - m_rcInner.bottom);

	// ��ͼ��ر���
	CComboUIEx* pCombo = (CComboUIEx*)m_pOwner;
	ComboProperty& cbProperty = pCombo->m_cbProperty;
	Graphics g(hDC);
	GraphicsPath shadowPath;
	Region shadowRgn;

	// ͼƬ�ı�����(��Ϊ��ʹ�õ�GDI+��֧�ֶ��ֽ��ַ�������������ÿ�Դ���еķ������л���)
	CDUIRect rcText = m_rcItem;
	rcText.right = rcText.left + 25;
	rcText.bottom = rcText.top + 25;
	DrawItemText(hDC, rcText);

	// ͼƬ����Ӱ����
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
	// ����״̬���ƾ����Ƿ���Ʊ���
	if (state == ITEM_STATE_HOT)		// HOT����
	{
		CSXPicture::SetShadowRP(rcDraw, m_rcInner.left,
			m_rcInner.top, m_rcInner.right, m_rcInner.bottom, shadowRgn, shadowPath);
		LinearGradientBrush lBrush(rcItem, cbProperty.dwClrHot1,
			cbProperty.dwClrHot2, LinearGradientModeVertical);
		g.FillRegion(&lBrush, &shadowRgn);
	}
	else if (state == ITEM_STATE_SEL)	// SEL����
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
	m_rcInner.right = (width - pCombo->GetImageWidth()) / 5;	// �����ھ�4��1
	m_rcInner.left = m_rcInner.right * 4;
	m_rcInner.top = (height - pCombo->GetImageHeight()) / 2;
	m_rcInner.bottom = m_rcInner.top;
}

bool CListLabelElementUIEx::DrawImage(CComboUIEx* pCombo, HDC hDC, RECT& rcDraw, LPCTSTR pStrModify /* = NULL */)
{
	if (m_image != NULL)
	{// ͼƬ������
		Graphics graphcs(hDC);

		CSXPicture::DrawShadow(graphcs, rcDraw, 3, 3, 5, 5);
		graphcs.DrawImage(m_image, rcDraw.left, rcDraw.top, m_image->GetWidth(), m_image->GetHeight());
		return true;
	}
	else
	{// ͼƬδ���ã������ı�..................
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
	{// ����GDI+������Ҫ���¼���
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
	// ȷ������Ƿ���LLE��
	POINT point;
	GetCursorPos(&point);
	HWND hWnd = FindWindow(_T("ComboWnd"), NULL);
	ScreenToClient(hWnd, &point);
	if (PtInRect(&m_rcItem, point))
		return ITEM_STATE_HOT;
	// �жϵ�ǰҳ�Ƿ�Ϊѡ����
	CComboUIEx* pCombo = (CComboUIEx*)m_pOwner;
	if (m_iIndex == pCombo->m_iCurShow)
		return ITEM_STATE_SEL;
	// Ĭ��Normal״̬���Ȳ�Hot���ֲ�Sel
	return ITEM_STATE_NORMAL;
}
