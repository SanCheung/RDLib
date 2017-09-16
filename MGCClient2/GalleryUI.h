#pragma once

// 画廊 自己滚动后，需要发送此消息 告诉 滚动条
#define		WM_GLY_SETVS_RATIO		WM_USER+100
#define		WM_GLY_CLICK_LEFT		WM_USER+101
#define		WM_GLY_CLICK_RIGHT		WM_USER+102

class CGalleryUI : public CControlUI
{
public:
	CGalleryUI(void);
	virtual ~CGalleryUI(void);

	void	Event( TEventUI& evt );
	void	DoPaint(HDC hDC, const RECT& rcPaint);
	void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void	SetPos(RECT rc);

	void	SetVSRatio( float f );


public:
	void	setImageFolder( CStringW strFolder )
	{
		m_strImageFolder = strFolder;
	}
	void	addTitleAndImageName( CStringW strTitle, CStringW strImageName );

private:
	void	draw( Bitmap *pBmp );
	void	drawItem( Graphics &g, RectF rt, int index );

	CDUIRect	getItemRect( int index );
	void		getItemButtonsRect( int index, CDUIRect &rtLeft, CDUIRect &rtRight );

	void		eventPointToLogic( CDUIPoint pt, int &x, int &y );
	CDUIPoint	eventPointToLogic( CDUIPoint pt );

	int			hitTest( CDUIPoint pt );
	void		reCalcLimit();

	Image*		addImageToLib( CStringW strImage );

private:
	vector< CStringW >		m_asTitle;
	vector< Image* >		m_arImage;

	map< CStringW, Image* >		m_mImageLib;

	CStringW	m_strImageFolder;

	int		m_nIndexHover;

	// -1 noraml  1 hover 2 pushed
	int		m_nLButtonState;
	int		m_nRButtonState;

	int		m_offsetY;
	int		m_offsetMax;
};

