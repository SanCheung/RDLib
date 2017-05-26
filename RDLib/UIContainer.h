#ifndef __UICONTAINER_H__
#define __UICONTAINER_H__

#pragma once

//namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//

class IContainerUI
{
public:
    virtual CControlUI* GetItemAt(int iIndex) const = 0;
    virtual int GetItemIndex(CControlUI* pControl) const  = 0;
    virtual bool SetItemIndex(CControlUI* pControl, int iIndex)  = 0;
    virtual int GetCount() const = 0;
    virtual bool Add(CControlUI* pControl) = 0;
    virtual bool AddAt(CControlUI* pControl, int iIndex)  = 0;
    virtual bool Remove(CControlUI* pControl) = 0;
    virtual bool RemoveAt(int iIndex)  = 0;

	//20170110 add bNeedUpdate
    virtual void RemoveAll( bool bNeedUpdate = true ) = 0;
};


/////////////////////////////////////////////////////////////////////////////////////
//
class CScrollBarUI;

class RDLIB_API CContainerUI : public CControlUI, public IContainerUI
{
public:
    CContainerUI();
    virtual ~CContainerUI();

public:
    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    CControlUI* GetItemAt(int iIndex) const;
    int GetItemIndex(CControlUI* pControl) const;
    bool SetItemIndex(CControlUI* pControl, int iIndex);
    int GetCount() const;
    bool Add(CControlUI* pControl);
    bool AddAt(CControlUI* pControl, int iIndex);
    bool Remove(CControlUI* pControl);
    bool RemoveAt(int iIndex);
	
	//20170110 add bNeedUpdate
    void RemoveAll( bool bNeedUpdate = true );

    void DoEvent(TEventUI& event);
    void SetVisible(bool bVisible = true);
    void SetInternVisible(bool bVisible = true);
    void SetMouseEnabled(bool bEnable = true);

    virtual RECT GetInset() const;
    virtual void SetInset(RECT rcInset); // 设置内边距，相当于设置客户区
    virtual int GetChildPadding() const;
    virtual void SetChildPadding(int iPadding);
    virtual bool IsAutoDestroy() const;
    virtual void SetAutoDestroy(bool bAuto);
    virtual bool IsDelayedDestroy() const;
    virtual void SetDelayedDestroy(bool bDelayed);
    virtual bool IsMouseChildEnabled() const;
    virtual void SetMouseChildEnabled(bool bEnable = true);

    virtual int FindSelectable(int iIndex, bool bForward = true) const;

    void SetPos(RECT rc);
    void DoPaint(HDC hDC, const RECT& rcPaint);

    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);
    CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

    virtual SIZE GetScrollPos() const;
    virtual SIZE GetScrollRange() const;
    virtual void SetScrollPos(SIZE szPos);
    virtual void LineUp();
    virtual void LineDown();
    virtual void PageUp();
    virtual void PageDown();
    virtual void HomeUp();
    virtual void EndDown();
    virtual void LineLeft();
    virtual void LineRight();
    virtual void PageLeft();
    virtual void PageRight();
    virtual void HomeLeft();
    virtual void EndRight();
    virtual void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);
    virtual CScrollBarUI* GetVerticalScrollBar() const;
    virtual CScrollBarUI* GetHorizontalScrollBar() const;

protected:
    virtual void SetFloatPos(int iIndex);
    virtual void ProcessScrollBar(RECT rc, int cxRequired, int cyRequired);
	virtual void ProcessHScrollbar(RECT rc, int cxRequired);
	virtual void ProcessVScrollbar(RECT rc, int cyRequired);

protected:
    CStdPtrArray m_items;
    RECT m_rcInset;
    int m_iChildPadding;
    bool m_bAutoDestroy;
    bool m_bDelayedDestroy;
    bool m_bMouseChildEnabled;
    bool m_bScrollProcess; // 防止SetPos循环调用

    CScrollBarUI* m_pVerticalScrollBar;
    CScrollBarUI* m_pHorizontalScrollBar;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class RDLIB_API CVerticalLayoutUI : public CContainerUI
{
public:
    CVerticalLayoutUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;

    void SetSepHeight(int iHeight);
    int GetSepHeight() const;
    void SetSepImmMode(bool bImmediately);
    bool IsSepImmMode() const;
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    void DoEvent(TEventUI& event);

    void SetPos(RECT rc);
    void DoPostPaint(HDC hDC, const RECT& rcPaint);

    RECT GetThumbRect(bool bUseNew = false) const;

protected:
    int m_iSepHeight;
    UINT m_uButtonState;
    POINT ptLastMouse;
    RECT m_rcNewPos;
    bool m_bImmMode;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class RDLIB_API CHorizontalLayoutUI : public CContainerUI
{
public:
    CHorizontalLayoutUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;
    
    void SetSepWidth(int iWidth);
	int GetSepWidth() const;
    void SetSepImmMode(bool bImmediately);
	bool IsSepImmMode() const;
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    void DoEvent(TEventUI& event);

    void SetPos(RECT rc);
    void DoPostPaint(HDC hDC, const RECT& rcPaint);

    RECT GetThumbRect(bool bUseNew = false) const;

protected:
    int m_iSepWidth;
    UINT m_uButtonState;
    POINT ptLastMouse;
    RECT m_rcNewPos;
    bool m_bImmMode;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class RDLIB_API CTileLayoutUI : public CContainerUI
{
public:
    CTileLayoutUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void SetPos(RECT rc);

    SIZE GetItemSize() const;
    void SetItemSize(SIZE szItem);
    int GetColumns() const;
    void SetColumns(int nCols);

    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
    SIZE m_szItem;
    int m_nColumns;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class RDLIB_API CTabLayoutUI : public CContainerUI
{
public:
    CTabLayoutUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    bool Add(CControlUI* pControl);
    bool AddAt(CControlUI* pControl, int iIndex);
    bool Remove(CControlUI* pControl);
    void RemoveAll();
    int GetCurSel() const;
    bool SelectItem(int iIndex);

    void SetPos(RECT rc);

    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
    int m_iCurSel;
};



class RDLIB_API CCenterUI : public CContainerUI
{

public:
	LPCTSTR GetClass() const
	{
		return _T("CenterUI");
	}
	LPVOID GetInterface(LPCTSTR pstrName)
	{
		if( EQUSTR( pstrName, _T("Center")) )
		{
			return static_cast<LPVOID>(this);
		}
		return CContainerUI::GetInterface(pstrName);
	}
	bool Add(CControlUI* pControl)
	{
		return CContainerUI::Add(pControl);
	}
	void SetPos(RECT rc)
	{
		CControlUI::SetPos(rc);
		if( m_items.IsEmpty() ) return;
		rc.left += m_rcInset.left;
		rc.top += m_rcInset.top;
		rc.right -= m_rcInset.right;
		rc.bottom -= m_rcInset.bottom;

		for( int it = 0; it < m_items.GetSize(); it++ ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
			if( !pControl->IsVisible() ) continue;
			if( pControl->IsFloat() ) {
				SetFloatPos(it);
			}
			else 
			{	
				CDUIRect rectCenter = rc;
				CDUISize size = pControl->EstimateSize(CDUISize(rc.right-rc.left, rc.bottom-rc.top));
				if (size.cx == 0){
					size.cx = rectCenter.GetWidth();
				}
				if (size.cy == 0){
					size.cy = rectCenter.GetHeight();
				}

				if (size.cx < rectCenter.GetWidth() )
				{
					rectCenter.left = rc.left + (rectCenter.GetWidth() - size.cx) / 2;	
					rectCenter.right = rectCenter.left + size.cx;
				}
				if (size.cy < rectCenter.GetHeight())
				{
					rectCenter.top = rc.top + (rectCenter.GetHeight() - size.cy) / 2;					
					rectCenter.bottom = rectCenter.top + size.cy;
				}
				pControl->SetPos(rectCenter); //
			}
		}
	}
};



//} // namespace DuiLib

#endif // __UICONTAINER_H__
