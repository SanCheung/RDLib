#pragma once
class CUIFontLib
{
public:
	CUIFontLib(void);
	virtual ~CUIFontLib(void);

public:
	void		SetDC( HDC hDC )		{ m_hDcPaint = hDC; }

public:
	HDC			m_hDcPaint;

	TFontInfo	m_DefaultFontInfo;
	vectorv		m_aCustomFonts;


	TFontInfo*	GetDefaultFontInfo();
	void		SetDefaultFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);

	DWORD	GetCustomFontCount() const;
	HFONT	AddFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	//HFONT AddFontAt(int index, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);

	HFONT	CloneFont(HFONT hFont);

	HFONT	GetFont(int index);
	HFONT	GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	bool	FindFont(HFONT hFont);
	bool	FindFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	int		GetFontIndex(HFONT hFont);
	int		GetFontIndex(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	int		GetFontIndexEx(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	
	bool	RemoveFont(HFONT hFont);
	bool	RemoveFontAt(int index);
	void	RemoveAllFonts();

	TFontInfo* GetFontInfo(int index);
	TFontInfo* GetFontInfo(HFONT hFont);

};

