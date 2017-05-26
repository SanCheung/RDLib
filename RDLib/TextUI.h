#pragma once

//namespace DuiLib
//{


class RDLIB_API CTextUI : public CLabelUI
{
public:
	CTextUI();
	~CTextUI();

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	CStringW* GetLinkContent(int iIndex);

	void DoEvent(TEventUI& event);
	SIZE EstimateSize(SIZE szAvailable);

	void PaintText(HDC hDC);

	void SetText(LPCTSTR pstrText);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	enum { MAX_LINK = 8 };
	int			m_nLinks;
	RECT		m_rcLinks[MAX_LINK];
	CStringW	m_sLinks[MAX_LINK];
	int			m_nHoverLink;
};


//};
