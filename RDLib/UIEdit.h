#pragma once


//namespace DuiLib
//{


class CEditWnd;

class RDLIB_API CEditUI : public CLabelUI
{
	friend CEditWnd;
public:
	CEditUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	virtual void DoInit();

	void SetEnabled(bool bEnable = true);
	void SetText(LPCTSTR pstrText);
	void SetMaxChar(UINT uMax);
	UINT GetMaxChar();
	void SetReadOnly(bool bReadOnly);
	bool IsReadOnly() const;
	void SetPasswordMode(bool bPasswordMode);
	bool IsPasswordMode() const;
	void SetPasswordChar(TCHAR cPasswordChar);
	TCHAR GetPasswordChar() const;

	LPCTSTR GetNormalImage();
	void SetNormalImage(LPCTSTR pStrImage);
	LPCTSTR GetHotImage();
	void SetHotImage(LPCTSTR pStrImage);
	LPCTSTR GetFocusedImage();
	void SetFocusedImage(LPCTSTR pStrImage);
	LPCTSTR GetDisabledImage();
	void SetDisabledImage(LPCTSTR pStrImage);
	void SetNativeEditBkColor(DWORD dwBkColor);
	DWORD GetNativeEditBkColor() const;

	void SetPos(RECT rc);
	void SetVisible(bool bVisible = true);
	void SetInternVisible(bool bVisible = true);
	SIZE EstimateSize(SIZE szAvailable);
	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void PaintStatusImage(HDC hDC);
	void PaintText(HDC hDC);
	bool IsNumberOnly();
	void SetNumberOnly(bool b);
	bool IsUpper();
	void SetUpper(bool b);
	void SetBanner( LPCTSTR pstrValue );
	void SetBannerColor( DWORD clrColor );

	void SetSel( int nStart, int nEnd );

protected:
	CEditWnd*	m_pWindow;
	bool		m_bMultiLine;

	CStringW	m_Banner;
	DWORD		m_BannerColor;
	UINT m_uMaxChar;
	bool m_bReadOnly;
	bool m_bPasswordMode;
	bool m_bNumberOnly;
	bool m_bUpper;
	TCHAR m_cPasswordChar;
	UINT m_uButtonState;
	CStringW m_sNormalImage;
	CStringW m_sHotImage;
	CStringW m_sFocusedImage;

	CStringW m_sDisabledImage;
	DWORD m_dwEditbkColor;
};



//};