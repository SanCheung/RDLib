#pragma once

//namespace DuiLib {



class RDLIB_API CCheckBoxUI : public CButtonUI
{
public:
	CCheckBoxUI(void);

	LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
	bool		m_bChecked;


protected:
	CStringW	m_sCheckedImage;
    CStringW	m_sUnCheckedImage;
	CStringW	m_sCheckDisableImage;

	CDUISize	m_CheckImageSize;

public:
	void Event(TEventUI& event);
	void DoPaint(HDC hDC, const RECT& rcPaint);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void SetCheckImageSize( SIZE sizeCheckImage );

	bool IsCheck();
	void SetCheck(bool check);
	
};

//}