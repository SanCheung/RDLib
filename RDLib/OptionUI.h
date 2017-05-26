#pragma once

//namespace DuiLib
//{

class RDLIB_API COptionUI : public CButtonUI
{
public:
	COptionUI();
	~COptionUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);

	bool Activate();
	void SetEnabled(bool bEnable = true);

	LPCTSTR GetSelectedImage();
	void SetSelectedImage(LPCTSTR pStrImage);

	void SetSelectedTextColor(DWORD dwTextColor);
	DWORD GetSelectedTextColor();

	LPCTSTR GetForeImage();
	void SetForeImage(LPCTSTR pStrImage);

	LPCTSTR GetGroup() const;
	void SetGroup(LPCTSTR pStrGroupName = NULL);
	bool IsSelected() const;
	void Selected(bool bSelected);

	SIZE EstimateSize(SIZE szAvailable);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void PaintStatusImage(HDC hDC);
	void PaintText(HDC hDC);

protected:
	bool m_bSelected;
	CStringW m_sGroupName;

	DWORD m_dwSelectedTextColor;

	CStringW m_sSelectedImage;
	CStringW m_sForeImage;
};



//};