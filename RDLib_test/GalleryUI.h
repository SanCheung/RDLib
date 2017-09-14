#pragma once
class CGalleryUI : public CControlUI
{
public:
	CGalleryUI(void);
	virtual ~CGalleryUI(void);

	void	Event( TEventUI& evt );
	void	DoPaint(HDC hDC, const RECT& rcPaint);
	void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void	SetPos(RECT rc);

private:
	void	draw( Bitmap *pBmp );
	void	drawItem( Graphics &g, RectF rt, int index );

	CDUIRect	getItemRect( int index );
	int			hitTest( CDUIPoint pt );
	void		reCalcLimit();


private:
	vector< CStringW >		m_asText;
	vector< CStringW >		m_asImage;

	int		m_nIndexHover;
	int		m_nIndexSelected;

	int		m_offsetY;
	int		m_offsetMax;
};

