#pragma once

//#define		WM_REFRESHSUBWINDOW			WM_USER+10000


class RDLIB_API CWndUI : public CControlUI
{
public:
	CWndUI( CHostableWnd* pWnd );
	~CWndUI(void);

	virtual void SetPos(RECT rc);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void SetInternVisible(bool bVisible = true);
	void SetVisible(bool bVisible = true);
	CHostableWnd* GetWindow(LPCWSTR pstrWClassName);
	void DoInit();
	void DoEvent(TEventUI& event);
	void DestroyInternalWindow();

	CHostableWnd*		m_pWnd;
	CDUIRect			m_rcOld;

	//virtual void DoPaint(HDC hDC, const RECT& rcPaint);
};

