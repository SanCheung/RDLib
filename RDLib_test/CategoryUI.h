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
	void	addText( CStringW strText )
	{
		m_asText.push_back( strText );
	}

private:
	void	draw( Bitmap *pBmp );
	void	drawItem( Graphics &g, RectF rt, int index );

	CDUIRect	getItemRect( int index );
	int			hitTest( CDUIPoint pt );
	void		reCalcLimit();


private:
	vector< CStringW >		m_asText;
	int		m_nIndexHover;
	int		m_nIndexSelected;

	int		m_offsetY;
	int		m_offsetMax;
};

