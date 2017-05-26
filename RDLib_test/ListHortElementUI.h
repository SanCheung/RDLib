#pragma once
class CListHortElementUI : public CHorizontalLayoutUI, public IListItemUI
{
public:
	CListHortElementUI(void);
	virtual ~CListHortElementUI(void);

	void SetPos(RECT rc);

	CListHeaderUI *m_pHeader;


	LPCTSTR GetClass() const	{ return _T("ListHortElementUI"); }
	UINT GetControlFlags() const{ return UIFLAG_WANTRETURN; }
	LPVOID GetInterface(LPCTSTR pstrName);

	int GetIndex() const;
	void SetIndex(int iIndex);

	IListOwnerUI* GetOwner();
	void SetOwner(CControlUI* pOwner);
	void SetVisible(bool bVisible = true);
	void SetEnabled(bool bEnable = true);

	bool IsSelected() const;
	bool Select(bool bSelect = true);
	bool IsExpanded() const;
	bool Expand(bool bExpand = true);

	void Invalidate(); // 直接CControl::Invalidate会导致滚动条刷新，重写减少刷新区域
	bool Activate();

	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void DoPaint(HDC hDC, const RECT& rcPaint);

	void DrawItemText(HDC hDC, const RECT& rcItem);    
	void DrawItemBk(HDC hDC, const RECT& rcItem);

protected:
	int m_iIndex;
	bool m_bSelected;
	UINT m_uButtonState;
	IListOwnerUI* m_pOwner;
};

