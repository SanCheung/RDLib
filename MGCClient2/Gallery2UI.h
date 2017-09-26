#pragma once
#include "D2HostableWnd.h"

class CGallery2UI	: public CD2HostableWnd
{
public:
	CGallery2UI(void);
	virtual ~CGallery2UI(void);

	virtual void	DoDraw();
	virtual void	dh_BuildAllBitmap();

	virtual LRESULT HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );



	void	SetVSRatio( float f );


public:
	void	setImageFolder( CStringW strFolder )
	{
		m_strImageFolder = strFolder;
	}
	void	addTitleAndImageName( CStringW strTitle, CStringW strImageName );

	// clearAllItem + addTitleAndImageName.... + resetAllItems;
	void	clearAllItems();
	bool	resetAllItems();

	void	setData( const vector<mgcGameInfo>  &arGameInfo, CStringW strType );

private:
	//void	draw( Bitmap *pBmp );
	//void	drawItem( Graphics &g, RectF rt, int index );
	
	void	drawItem( CDUIRect rt, int index );

	CDUIRect	getItemRect( int index );
	void		getItemButtonsRect( int index, CDUIRect &rtLeft, CDUIRect &rtRight );

	void		eventPointToLogic( CDUIPoint pt, int &x, int &y );
	CDUIPoint	eventPointToLogic( CDUIPoint pt );

	int			hitTest( CDUIPoint pt );
	bool		reCalcLimit();

	//Image*		addImageToLib( CStringW strImage );

private:
	CDUIRect				m_rt;

	vector< CStringW >		m_asTitle;
	vector< ID2D1Bitmap* >	m_arImage;

	ID2D1Bitmap*			m_pBg;
	vector< ID2D1Bitmap* >	m_arFSImage;
	vector< ID2D1Bitmap* >	m_arSGImage;

	//vector< CStringW >	m_asTitle;
	//vector< Image* >		m_arImage;

	//map< CStringW, Image* >		m_mImageLib;

	CStringW	m_strImageFolder;

	int		m_nIndexHover;

	// -1 noraml  1 hover 2 pushed
	int		m_nLButtonState;
	int		m_nRButtonState;

	int		m_offsetY;
	int		m_offsetMax;

	bool	m_bMouseTracking;
};

