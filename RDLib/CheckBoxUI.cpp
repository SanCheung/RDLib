#include "stdafx.h"
#include "CheckBoxUI.h"

//namespace DuiLib {

CCheckBoxUI::CCheckBoxUI(void)
{
	m_bChecked = false;	
	m_CheckImageSize.cx = m_CheckImageSize.cy = 0;
	m_uTextStyle = DT_SINGLELINE | DT_VCENTER | DT_LEFT;
}

LPCTSTR CCheckBoxUI::GetClass() const
{
    return _T("CheckBoxUI");
}

LPVOID CCheckBoxUI::GetInterface(LPCTSTR pstrName)
{
    if( EQUSTR(pstrName, _T("CheckBox")) ) 
		return static_cast<CCheckBoxUI*>(this);

    return CButtonUI::GetInterface(pstrName);
}

void CCheckBoxUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( EQUSTR(pstrName, _T("checkedimage")) )
		m_sCheckedImage = pstrValue;
    else if( EQUSTR(pstrName, _T("uncheckedimage")) ) 
		m_sUnCheckedImage = pstrValue;
    else if( EQUSTR(pstrName, _T("checkdisableimage")) ) 
		m_sCheckDisableImage = pstrValue;
	else if( EQUSTR(pstrName, _T("checkimagesize")) )
	{
		SIZE sizeCheckImage = {0};
		LPTSTR pstr = NULL;
		sizeCheckImage.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		sizeCheckImage.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
		SetCheckImageSize(sizeCheckImage);
	}
    else 
		CButtonUI::SetAttribute(pstrName, pstrValue);
}

void CCheckBoxUI::Event(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pParent != NULL ) m_pParent->Event(event);
        else CLabelUI::Event(event);
        return;
    } 	
	
	if( event.Type == UIEVENT_SETCURSOR )
	{
		 ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		return;
	}
    else if( event.Type == UIEVENT_BUTTONUP )
	{
		if (IsEnabled())
		{
			if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() )
			{
				m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
	            
				m_bChecked = !m_bChecked;
				if (m_pManager){
					m_pManager->SendNotify(this, _T("checkedchanged"), event.wParam, event.lParam);
				}
				Invalidate();
			}
		}		
        return;
    }	
	else
	{
		CButtonUI::Event(event);
	}
}

void CCheckBoxUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) 
		return;

	RECT	rcStateBox,rcText;
	int stateBoxBottomOffset =  (GetHeight() - m_CheckImageSize.cy) / 2;
	if (m_CheckImageSize.cy >= GetHeight()){
		stateBoxBottomOffset = 0;
	}
	
	rcStateBox.left = m_rcItem.left;
	rcStateBox.top = m_rcItem.top + stateBoxBottomOffset;
	rcStateBox.right = rcStateBox.left +  m_CheckImageSize.cx;
	rcStateBox.bottom = rcStateBox.top + m_CheckImageSize.cy;

	rcText.left = rcStateBox.right + 4;
	rcText.top = m_rcItem.top ;
	rcText.right = m_rcItem.right;
	rcText.bottom = m_rcItem.bottom;

    // 绘制循序：背景颜色->背景图->状态图->文本
    // 选中状态的图片 长 宽 是 按钮高度 
    PaintBkColor(hDC);
    PaintBkImage(hDC);
    PaintStatusImage(hDC);
    
	DWORD textColor = m_dwTextColor;
	if (!IsEnabled())
	{
		CRenderEngine::DrawImageString(hDC, m_pManager, rcStateBox, m_rcPaint, m_sCheckDisableImage, NULL);
		textColor = 0xffcccccc;
	}
	else
	{
		if (m_bChecked)
		{
			CRenderEngine::DrawImageString(hDC, m_pManager, rcStateBox, m_rcPaint, m_sCheckedImage, NULL);
		}
		else
		{
			CRenderEngine::DrawImageString(hDC, m_pManager, rcStateBox, m_rcPaint, m_sUnCheckedImage, NULL);
		}
	}
	
	CRenderEngine::DrawText(hDC, m_pManager, rcText, m_sText, textColor, m_iFont, DT_SINGLELINE | m_uTextStyle);
    PaintBorder(hDC);
}

bool CCheckBoxUI::IsCheck()
{
	return m_bChecked;
}

void CCheckBoxUI::SetCheck( bool check )
{
	if (m_bChecked == check)
	{
		return;
	}
	m_bChecked = check;
	Invalidate();
}

void CCheckBoxUI::SetCheckImageSize( SIZE sizeCheckImage )
{
	m_CheckImageSize = sizeCheckImage;
}

//}