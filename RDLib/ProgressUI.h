#pragma once

//namespace DuiLib
//{

class RDLIB_API CProgressUI : public CLabelUI
{
public:
	CProgressUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	bool IsHorizontal();
	void SetHorizontal(bool bHorizontal = true);
	int GetMinValue() const;
	void SetMinValue(int nMin);
	int GetMaxValue() const;
	void SetMaxValue(int nMax);
	int GetValue() const;
	void SetValue(int nValue);
	LPCTSTR GetForeImage() const;
	void SetForeImage(LPCTSTR pStrImage);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void PaintStatusImage(HDC hDC);

	SIZE EstimateSize(SIZE szAvailable);
protected:
	bool m_bHorizontal;
	int m_nMax;
	int m_nMin;
	int m_nValue;

	CStringW m_sForeImage;
	CStringW m_sForeImageModify;

	DWORD m_ProgressForeColor;
	DWORD m_ProgressForeColor2;
};


//};
