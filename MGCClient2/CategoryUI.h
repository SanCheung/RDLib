#pragma once
class CCategoryUI : public CControlUI
{
public:
	CCategoryUI(void);
	virtual ~CCategoryUI(void);

	void	Event( TEventUI& evt );
	void	DoPaint(HDC hDC, const RECT& rcPaint);
	void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void	SetPos(RECT rc);

public:
	void	addText( CStringW strText, string strID )
	{
		m_asText.push_back( strText );
		m_asID.push_back( strID );
	}

	CStringW getItemText( int nIndex )
	{
		if( nIndex < 0 || nIndex >= (int)m_asText.size() )
			return L"";

		return m_asText[nIndex];
	}

	string	getItemID( int nIndex )
	{
		if( nIndex < 0 || nIndex >= (int)m_asID.size() )
			return "";

		return m_asID[nIndex];
	}

	void	clearAll()
	{	
		m_asText.clear();
		m_asID.clear();
	}

	void	setData( const vector<mgcGameFamilyInfo>  &arFamilyInfo );


private:
	void	draw( Bitmap *pBmp );
	void	drawItem( Graphics &g, RectF rt, int index );

	CDUIRect	getItemRect( int index );
	int			hitTest( CDUIPoint pt );
	void		reCalcLimit();


private:
	vector< CStringW >		m_asText;
	vector< string >		m_asID;

	int		m_nIndexHover;
	int		m_nIndexSelected;

	int		m_offsetY;
	int		m_offsetMax;
};

