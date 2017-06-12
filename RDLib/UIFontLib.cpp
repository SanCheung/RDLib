#include "StdAfx.h"
#include "UIFontLib.h"


CUIFontLib::CUIFontLib(void)
{
	// build default font; 
	LOGFONT lf = { 0 };
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	lf.lfCharSet = DEFAULT_CHARSET;
	HFONT hDefaultFont = ::CreateFontIndirect(&lf);
	m_DefaultFontInfo.hFont = hDefaultFont;
	//m_DefaultFontInfo.sFontName = lf.lfFaceName;
	wcscpy( m_DefaultFontInfo.sFontName, lf.lfFaceName );
	m_DefaultFontInfo.iSize = -lf.lfHeight;
	m_DefaultFontInfo.bBold = (lf.lfWeight >= FW_BOLD);
	m_DefaultFontInfo.bUnderline = (lf.lfUnderline == TRUE);
	m_DefaultFontInfo.bItalic = (lf.lfItalic == TRUE);
	::ZeroMemory(&m_DefaultFontInfo.tm, sizeof(m_DefaultFontInfo.tm));
}


CUIFontLib::~CUIFontLib(void)
{
	::DeleteObject(m_DefaultFontInfo.hFont);
}

TFontInfo* CUIFontLib::GetDefaultFontInfo()
{
	//if( m_pParentResourcePM ) return m_pParentResourcePM->GetDefaultFontInfo();

	if( m_DefaultFontInfo.tm.tmHeight == 0 )
	{
		HFONT hOldFont = (HFONT) ::SelectObject(m_hDcPaint, m_DefaultFontInfo.hFont);
		::GetTextMetrics(m_hDcPaint, &m_DefaultFontInfo.tm);
		::SelectObject(m_hDcPaint, hOldFont);
	}
	return &m_DefaultFontInfo;
}

void CUIFontLib::SetDefaultFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)
{
	LOGFONT lf = { 0 };
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	_tcscpy(lf.lfFaceName, pStrFontName);
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfHeight = -nSize;
	if( bBold ) lf.lfWeight += FW_BOLD;
	if( bUnderline ) lf.lfUnderline = TRUE;
	if( bItalic ) lf.lfItalic = TRUE;
	lf.lfQuality  = CLEARTYPE_QUALITY; //尽可能使用truetype
	HFONT hFont = ::CreateFontIndirect(&lf);
	if( hFont == NULL ) return;

	::DeleteObject(m_DefaultFontInfo.hFont);
	m_DefaultFontInfo.hFont = hFont;
	//m_DefaultFontInfo.sFontName = pStrFontName;
	wcscpy( m_DefaultFontInfo.sFontName, pStrFontName );
	m_DefaultFontInfo.iSize = nSize;
	m_DefaultFontInfo.bBold = bBold;
	m_DefaultFontInfo.bUnderline = bUnderline;
	m_DefaultFontInfo.bItalic = bItalic;
	::ZeroMemory(&m_DefaultFontInfo.tm, sizeof(m_DefaultFontInfo.tm));
	if( m_hDcPaint ) {
		HFONT hOldFont = (HFONT) ::SelectObject(m_hDcPaint, hFont);
		::GetTextMetrics(m_hDcPaint, &m_DefaultFontInfo.tm);
		::SelectObject(m_hDcPaint, hOldFont);
	}
}

DWORD CUIFontLib::GetCustomFontCount() const
{
	return (int)m_aCustomFonts.size();
}

HFONT CUIFontLib::AddFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)
{
	LOGFONT lf = { 0 };
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	_tcscpy(lf.lfFaceName, pStrFontName);
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfHeight = -nSize;
	if( bBold ) lf.lfWeight += FW_BOLD;
	if( bUnderline ) lf.lfUnderline = TRUE;
	if( bItalic ) lf.lfItalic = TRUE;
	lf.lfQuality  = CLEARTYPE_QUALITY; //尽可能使用truetype
	HFONT hFont = ::CreateFontIndirect(&lf);
	if( hFont == NULL ) return NULL;

	TFontInfo* pFontInfo = new TFontInfo;
	if( !pFontInfo ) return false;
	::ZeroMemory(pFontInfo, sizeof(TFontInfo));
	pFontInfo->hFont = hFont;
	//pFontInfo->sFontName = pStrFontName;
	wcscpy( pFontInfo->sFontName, pStrFontName );
	pFontInfo->iSize = nSize;
	pFontInfo->bBold = bBold;
	pFontInfo->bUnderline = bUnderline;
	pFontInfo->bItalic = bItalic;
	if( m_hDcPaint ) {
		HFONT hOldFont = (HFONT) ::SelectObject(m_hDcPaint, hFont);
		::GetTextMetrics(m_hDcPaint, &pFontInfo->tm);
		::SelectObject(m_hDcPaint, hOldFont);
	}


	m_aCustomFonts.push_back( pFontInfo );
	//if( !m_aCustomFonts.Add(pFontInfo) ) {
	//    ::DeleteObject(hFont);
	//    delete pFontInfo;
	//    return NULL;
	//}

	return hFont;
}

//HFONT CUIFontLib::AddFontAt(int index, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)
//{
//    LOGFONT lf = { 0 };
//    ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
//    _tcscpy(lf.lfFaceName, pStrFontName);
//    lf.lfCharSet = DEFAULT_CHARSET;
//    lf.lfHeight = -nSize;
//    if( bBold ) lf.lfWeight += FW_BOLD;
//    if( bUnderline ) lf.lfUnderline = TRUE;
//    if( bItalic ) lf.lfItalic = TRUE;
//	lf.lfQuality  = CLEARTYPE_QUALITY; //尽可能使用truetype
//    HFONT hFont = ::CreateFontIndirect(&lf);
//    if( hFont == NULL ) return NULL;
//
//    TFontInfo* pFontInfo = new TFontInfo;
//    if( !pFontInfo ) return false;
//    ::ZeroMemory(pFontInfo, sizeof(TFontInfo));
//    pFontInfo->hFont = hFont;
//    //pFontInfo->sFontName = pStrFontName;
//	wcscpy( pFontInfo->sFontName, pStrFontName );
//    pFontInfo->iSize = nSize;
//    pFontInfo->bBold = bBold;
//    pFontInfo->bUnderline = bUnderline;
//    pFontInfo->bItalic = bItalic;
//    if( m_hDcPaint ) {
//        HFONT hOldFont = (HFONT) ::SelectObject(m_hDcPaint, hFont);
//        ::GetTextMetrics(m_hDcPaint, &pFontInfo->tm);
//        ::SelectObject(m_hDcPaint, hOldFont);
//    }
//
//	m_aCustomFonts[index] = pFontInfo;
//    //if( !m_aCustomFonts.InsertAt(index, pFontInfo) ) {
//    //    ::DeleteObject(hFont);
//    //    delete pFontInfo;
//    //    return NULL;
//    //}
//
//    return hFont;
//}

HFONT CUIFontLib::CloneFont( HFONT hFont )
{
	HFONT hFontClone = NULL;
	TFontInfo* pFontInfo = NULL;

	if(hFont == m_DefaultFontInfo.hFont)
	{
		pFontInfo = &m_DefaultFontInfo;
	}
	else
	{
		for( int it = 0; it < (int)m_aCustomFonts.size(); it++ ) {
			pFontInfo = static_cast<TFontInfo*>(m_aCustomFonts[it]);
			if( pFontInfo->hFont == hFont )
			{
				break;
			}
		}
	}

	if (pFontInfo != NULL)
	{
		LOGFONT lf = { 0 };
		::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		_tcscpy(lf.lfFaceName, pFontInfo->sFontName);
		lf.lfCharSet = DEFAULT_CHARSET;
		lf.lfHeight = -pFontInfo->iSize;
		if( pFontInfo->bBold ) lf.lfWeight += FW_BOLD;
		if( pFontInfo->bUnderline ) lf.lfUnderline = TRUE;
		if( pFontInfo->bUnderline ) lf.lfItalic = TRUE;
		lf.lfQuality  = CLEARTYPE_QUALITY; //尽可能使用truetype
		hFontClone = ::CreateFontIndirect(&lf);		
	}

	//else
	//{
	//	if( m_pParentResourcePM ) 
	//		hFontClone = m_pParentResourcePM->CloneFont(hFont);
	//}

	return hFontClone;
}

HFONT CUIFontLib::GetFont(int index)
{
	if( index < 0 || index >= (int)m_aCustomFonts.size() )
	{
		//if( m_pParentResourcePM )
		//	return m_pParentResourcePM->GetFont( index );

		return GetDefaultFontInfo()->hFont;
	}

	TFontInfo* pFontInfo = static_cast<TFontInfo*>(m_aCustomFonts[index]);
	return pFontInfo->hFont;
}

HFONT CUIFontLib::GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)
{
	TFontInfo* pFontInfo = NULL;
	for( int it = 0; it < (int)m_aCustomFonts.size(); it++ )
	{
		pFontInfo = static_cast<TFontInfo*>(m_aCustomFonts[it]);
		if( pFontInfo->sFontName == pStrFontName && pFontInfo->iSize == nSize && 
			pFontInfo->bBold == bBold && pFontInfo->bUnderline == bUnderline && pFontInfo->bItalic == bItalic) 
			return pFontInfo->hFont;
	}

	//if( m_pParentResourcePM ) 
	//	return m_pParentResourcePM->GetFont(pStrFontName, nSize, bBold, bUnderline, bItalic);

	return NULL;
}

bool CUIFontLib::FindFont(HFONT hFont)
{
	TFontInfo* pFontInfo = NULL;
	for( int it = 0; it < (int)m_aCustomFonts.size(); it++ )
	{
		pFontInfo = static_cast<TFontInfo*>(m_aCustomFonts[it]);
		if( pFontInfo->hFont == hFont ) return true;
	}

	//if( m_pParentResourcePM ) 
	//	return m_pParentResourcePM->FindFont(hFont);

	return false;
}

bool CUIFontLib::FindFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)
{
	TFontInfo* pFontInfo = NULL;
	for( int it = 0; it < (int)m_aCustomFonts.size(); it++ )
	{
		pFontInfo = static_cast<TFontInfo*>(m_aCustomFonts[it]);
		if( pFontInfo->sFontName == pStrFontName && pFontInfo->iSize == nSize && 
			pFontInfo->bBold == bBold && pFontInfo->bUnderline == bUnderline && pFontInfo->bItalic == bItalic) 
			return true;
	}

	//if( m_pParentResourcePM ) 
	//	return m_pParentResourcePM->FindFont(pStrFontName, nSize, bBold, bUnderline, bItalic);
	return false;
}

int CUIFontLib::GetFontIndex(HFONT hFont)
{
	TFontInfo* pFontInfo = NULL;
	for( int it = 0; it < (int)m_aCustomFonts.size(); it++ ) {
		pFontInfo = static_cast<TFontInfo*>(m_aCustomFonts[it]);
		if( pFontInfo->hFont == hFont ) 
			return it;
	}
	return -1;
}

int CUIFontLib::GetFontIndex(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)
{
	TFontInfo* pFontInfo = NULL;
	for( int it = 0; it < (int)m_aCustomFonts.size(); it++ )
	{
		pFontInfo = static_cast<TFontInfo*>(m_aCustomFonts[it]);
		if( pFontInfo->sFontName == pStrFontName &&
			pFontInfo->iSize == nSize && 
			pFontInfo->bBold == bBold && 
			pFontInfo->bUnderline == bUnderline && 
			pFontInfo->bItalic == bItalic ) 
			return it;
	}
	return -1;
}

int CUIFontLib::GetFontIndexEx( LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic )
{
	TFontInfo* pFontInfo = NULL;
	for( int it = 0; it < (int)m_aCustomFonts.size(); it++ )
	{
		pFontInfo = static_cast<TFontInfo*>(m_aCustomFonts[it]);
		if( pFontInfo->sFontName == pStrFontName && 
			pFontInfo->iSize == nSize && 
			pFontInfo->bBold == bBold && 
			pFontInfo->bUnderline == bUnderline && 
			pFontInfo->bItalic == bItalic) 
			return it;
	}

	LOGFONT lf = { 0 };
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	_tcscpy(lf.lfFaceName, pStrFontName);
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfHeight = -nSize;
	if( bBold ) lf.lfWeight += FW_BOLD;
	if( bUnderline ) lf.lfUnderline = TRUE;
	if( bItalic ) lf.lfItalic = TRUE;
	lf.lfQuality  = CLEARTYPE_QUALITY; //尽可能使用truetype
	HFONT hFont = ::CreateFontIndirect(&lf);
	if( hFont == NULL ) return -1;

	pFontInfo = new TFontInfo;
	if( !pFontInfo ) return -1;
	::ZeroMemory(pFontInfo, sizeof(TFontInfo));
	pFontInfo->hFont = hFont;
	//pFontInfo->sFontName = pStrFontName;
	wcscpy( pFontInfo->sFontName, pStrFontName );
	pFontInfo->iSize = nSize;
	pFontInfo->bBold = bBold;
	pFontInfo->bUnderline = bUnderline;
	pFontInfo->bItalic = bItalic;

	if( m_hDcPaint ) {
		HFONT hOldFont = (HFONT) ::SelectObject(m_hDcPaint, hFont);
		::GetTextMetrics(m_hDcPaint, &pFontInfo->tm);
		::SelectObject(m_hDcPaint, hOldFont);
	}

	m_aCustomFonts.push_back( pFontInfo );
	//if( !m_aCustomFonts.Add(pFontInfo) )
	//{
	//	::DeleteObject(hFont);
	//	delete pFontInfo;
	//	return -1;
	//}

	return (int)m_aCustomFonts.size()-1;
}


bool CUIFontLib::RemoveFont(HFONT hFont)
{
	TFontInfo* pFontInfo = NULL;
	//   for( int it = 0; it < (int)m_aCustomFonts.size(); it++ ) 
	//	  {
	//       pFontInfo = static_cast<TFontInfo*>(m_aCustomFonts[it]);
	//       if( pFontInfo->hFont == hFont )
	//	      {
	//           ::DeleteObject(pFontInfo->hFont);
	//           delete pFontInfo;
	//           return m_aCustomFonts.Remove(it);
	//       }
	//   }

	for( auto it = m_aCustomFonts.begin(); it != m_aCustomFonts.end(); ++it )
	{
		pFontInfo = static_cast<TFontInfo*>(*it);
		if( pFontInfo->hFont == hFont )
		{
			::DeleteObject(pFontInfo->hFont);
			delete pFontInfo;
			m_aCustomFonts.erase( it );
			return true;
		}
	}

	return false;
}

bool CUIFontLib::RemoveFontAt(int index)
{
	if( index < 0 || index >= (int)m_aCustomFonts.size() ) 
		return false;

	TFontInfo* pFontInfo = static_cast<TFontInfo*>(m_aCustomFonts[index]);
	::DeleteObject(pFontInfo->hFont);
	delete pFontInfo;

	//return m_aCustomFonts.Remove(index);
	m_aCustomFonts.erase( m_aCustomFonts.begin() + index );
	return true;
}

void CUIFontLib::RemoveAllFonts()
{
	TFontInfo* pFontInfo;
	for( int it = 0; it < (int)m_aCustomFonts.size(); it++ )
	{
		pFontInfo = static_cast<TFontInfo*>(m_aCustomFonts[it]);
		::DeleteObject(pFontInfo->hFont);
		delete pFontInfo;
	}
	m_aCustomFonts.clear();
}

TFontInfo* CUIFontLib::GetFontInfo(int index)
{
	if( index < 0 || index >= (int)m_aCustomFonts.size() ) 
		return GetDefaultFontInfo();

	TFontInfo* pFontInfo = static_cast<TFontInfo*>(m_aCustomFonts[index]);
	if( pFontInfo->tm.tmHeight == 0 )
	{
		HFONT hOldFont = (HFONT) ::SelectObject(m_hDcPaint, pFontInfo->hFont);
		::GetTextMetrics(m_hDcPaint, &pFontInfo->tm);
		::SelectObject(m_hDcPaint, hOldFont);
	}
	return pFontInfo;
}

TFontInfo* CUIFontLib::GetFontInfo(HFONT hFont)
{
	TFontInfo* pFontInfo = NULL;
	for( int it = 0; it < (int)m_aCustomFonts.size(); it++ )
	{
		pFontInfo = static_cast<TFontInfo*>(m_aCustomFonts[it]);
		if( pFontInfo->hFont == hFont )
		{
			if( pFontInfo->tm.tmHeight == 0 )
			{
				HFONT hOldFont = (HFONT) ::SelectObject(m_hDcPaint, pFontInfo->hFont);
				::GetTextMetrics(m_hDcPaint, &pFontInfo->tm);
				::SelectObject(m_hDcPaint, hOldFont);
			}
			return pFontInfo;
		}
	}

	//if( m_pParentResourcePM ) 
	//	return m_pParentResourcePM->GetFontInfo(hFont);
	return GetDefaultFontInfo();
}