#pragma once

//#include "../FrequentlyBasicLib/sstring.h"
//using namespace sstring;

class sstring::CStringW;

//namespace DuiLib
//{




class RDLIB_API CButtonUI : public CLabelUI
{
public:
	CButtonUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	bool Activate();
	void SetEnabled(bool bEnable = true);
	void DoEvent(TEventUI& event);

	LPCTSTR GetNormalImage()	{ return m_sNormalImage; }
	void SetNormalImage(LPCTSTR pStrImage);

	LPCTSTR GetHotImage()		{ return m_sHotImage; }
	void SetHotImage(LPCTSTR pStrImage);

	LPCTSTR GetPushedImage()	{ return m_sPushedImage; }
	void SetPushedImage(LPCTSTR pStrImage);

	LPCTSTR GetFocusedImage()	{ return m_sFocusedImage; }
	void SetFocusedImage(LPCTSTR pStrImage);

	LPCTSTR GetDisabledImage()	{ return m_sDisabledImage; }
	void SetDisabledImage(LPCTSTR pStrImage);

	void SetHotTextColor(DWORD dwColor);
	DWORD GetHotTextColor() const;

	void SetPushedTextColor(DWORD dwColor);
	DWORD GetPushedTextColor() const;

	void SetFocusedTextColor(DWORD dwColor);
	DWORD GetFocusedTextColor() const;

	SIZE EstimateSize(SIZE szAvailable);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void PaintText(HDC hDC);
	void PaintStatusImage(HDC hDC);

	//static Handle<v8::FunctionTemplate> getJSClass();

protected:
	bool	m_DbClicked;
	UINT m_uButtonState;

	DWORD m_dwHotTextColor;
	DWORD m_dwPushedTextColor;
	DWORD m_dwFocusedTextColor;

	CStringW m_sNormalImage;
	CStringW m_sHotImage;
	CStringW m_sPushedImage;
	CStringW m_sFocusedImage;
	CStringW m_sDisabledImage;
};



//JS_METHOD(_Button_Constructor);



//};
