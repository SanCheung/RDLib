#include "StdAfx.h"
#include "GalleryUI.h"

#define		ITEM_WIDTH		496

#define		ITEM_IMAGE_HEIGHT	279
#define		ITEM_TEXT_FONTSIZE	24
#define		ITEM_CTRLBAR_HEIGHT 62
#define		ITEM_HEIGHT		(ITEM_IMAGE_HEIGHT+ITEM_CTRLBAR_HEIGHT)

#define		H_PADDING		30
#define		V_PADDING		10

#define		TFontFace		L"Microsoft Yahei"


CGalleryUI::CGalleryUI(void)
	: m_offsetY( 0 )
	, m_offsetMax( 0 )

	, m_nIndexHover( -1 )
	, m_nLButtonState( -1 )
	, m_nRButtonState( -1 )
{
	setImageFolder( CAppData::GetResourcePath() );

	addTitleAndImageName( L"NBA2018",	L"game1.png" );
	addTitleAndImageName( L"��¶������", L"game2.png" );
	addTitleAndImageName( L"���ٿ��",	L"game3.png" );
	addTitleAndImageName( L"�Ǽ�����II", L"game2.png" );
	addTitleAndImageName( L"��;",		L"game1.png" );
	addTitleAndImageName( L"��ŭ��С��", L"game3.png" );
	addTitleAndImageName( L"��˫����",	L"game2.png" );
	addTitleAndImageName( L"�������",	L"game3.png" );
	addTitleAndImageName( L"�Ϲž���7",	L"game1.png" );
	addTitleAndImageName( L"����6",		L"game3.png" );
}


CGalleryUI::~CGalleryUI(void)
{
	m_arImage.clear();
	
	for( auto it = m_mImageLib.begin(); it != m_mImageLib.end(); ++it )
	{
		Image *image = it->second;
		DEL( image );
	}
	m_mImageLib.clear();
}

void CGalleryUI::Event( TEventUI& evt )
{
	if( UIEVENT_MOUSELEAVE == evt.Type )
	{
		m_nIndexHover = -1;
		m_nLButtonState = -1;
		m_nRButtonState = -1;
		Invalidate();
	}
	else if( UIEVENT_SCROLLWHEEL == evt.Type )
	{
		if( m_offsetMax > 0 )
		{
			int nDir = LOWORD( evt.wParam );
			if( SB_LINEUP == nDir )
				m_offsetY -= 50;
			else
				m_offsetY += 50;

			if( m_offsetY < 0 )
				m_offsetY = 0;

			if( m_offsetY > m_offsetMax )
				m_offsetY = m_offsetMax;

			Invalidate();

			int pc = int( m_offsetY*100.f/m_offsetMax);
			SendMessage( GetManager()->GetPaintWindow(), WM_GLY_SETVS_RATIO, pc, 0 );
			
		}
	}
	else if( UIEVENT_MOUSEMOVE == evt.Type )
	{
		int nNewHover = hitTest( evt.ptMouse );
		if( nNewHover != m_nIndexHover )
		{
			m_nIndexHover = nNewHover;
			//Invalidate();
			////ATLTRACE( L"%d\n", nNewHover );
		}

		if( nNewHover != -1 )
		{
			CDUIPoint	ptLogic = eventPointToLogic( evt.ptMouse );

			CDUIRect	rtL, rtR;
			getItemButtonsRect( nNewHover, rtL, rtR );

			if( PtInRect( rtL, ptLogic ) )
				m_nLButtonState = 1;
			else
				m_nLButtonState = -1;

			if( PtInRect( rtR, ptLogic ) )
				m_nRButtonState = 1;
			else
				m_nRButtonState = -1;
		}

		Invalidate();
	}
	else if( UIEVENT_BUTTONDOWN == evt.Type )
	{
		int nNewHover = hitTest( evt.ptMouse );
		if( nNewHover != -1 )
		{
			CDUIPoint	ptLogic = eventPointToLogic( evt.ptMouse );

			CDUIRect	rtL, rtR;
			getItemButtonsRect( nNewHover, rtL, rtR );

			if( PtInRect( rtL, ptLogic ) )
				m_nLButtonState = 2;
			else
				m_nLButtonState = -1;

			if( PtInRect( rtR, ptLogic ) )
				m_nRButtonState = 2;
			else
				m_nRButtonState = -1;

			Invalidate();
		}
	}
	else if( UIEVENT_BUTTONUP == evt.Type )
	{
		int nNewHover = hitTest( evt.ptMouse );
		if( nNewHover != -1 )
		{
			CDUIPoint	ptLogic = eventPointToLogic( evt.ptMouse );

			CDUIRect	rtL, rtR;
			getItemButtonsRect( nNewHover, rtL, rtR );

			if( PtInRect( rtL, ptLogic ) )
				SendMessage( GetManager()->GetPaintWindow(), WM_GLY_CLICK_LEFT, nNewHover, 0 );

			if( PtInRect( rtR, ptLogic ) )
				SendMessage( GetManager()->GetPaintWindow(), WM_GLY_CLICK_RIGHT, nNewHover, 0 );
		}
	}

	CControlUI::Event( evt );
}

void CGalleryUI::DoPaint( HDC hDC, const RECT& rcPaint )
{
	if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) 
		return;

	CDUIRect		rt( m_rcItem );
	int w = rt.Width();
	int h = rt.Height();

	Bitmap		bmp( w, h, PixelFormat32bppARGB );
	draw( &bmp );


	Graphics	g( hDC );
	g.DrawImage( &bmp, rt.left, rt.top );
}

void CGalleryUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	CControlUI::SetAttribute( pstrName, pstrValue );
}

void CGalleryUI::draw( Bitmap *pBmp )
{
	int w = pBmp->GetWidth();
	int h = pBmp->GetHeight();

	Graphics	g( pBmp );
	g.SetCompositingQuality( CompositingQualityHighQuality );
	g.SetTextRenderingHint( TextRenderingHintAntiAlias );

	int n = (int)m_asTitle.size();
	int i;
	for( i = 0; i < n; i++ )
	{
		CDUIRect	rt = getItemRect( i );
		//if( rt.top < -LINEHEIGHT || rt.top > h )
		//	continue;

		RectF		rtItem( rt.left, rt.top, rt.Width(), rt.Height() );
		drawItem( g, rtItem, i );
	}
}



void CGalleryUI::drawItem( Graphics &g, RectF rt, int index )
{
	Image	*image = m_arImage[index];
	g.DrawImage( image, Rect( rt.X, rt.Y, ITEM_WIDTH, ITEM_IMAGE_HEIGHT), 
		0, 0, image->GetWidth(), image->GetHeight(), UnitPixel );
	
	if( m_nIndexHover == index )
	{
		CStringW	strImageFV = CAppData::GetResourcePath() + L"fullvideo-normal.png";
		if( m_nLButtonState == 1 )
			strImageFV = CAppData::GetResourcePath() + L"fullvideo-hover.png";
		else if( m_nLButtonState == 2 )
			strImageFV = CAppData::GetResourcePath() + L"fullvideo-pushed.png";

		CDUIRect	rtL, rtR;
		getItemButtonsRect( index, rtL, rtR );
		
		Image	imgFV( strImageFV );
		int		iw = imgFV.GetWidth();
		int		ih = imgFV.GetHeight();

		//int		dx = rt.GetLeft();
		//int		dy = rt.GetBottom()-ITEM_CTRLBAR_HEIGHT;
		//int		dw = ITEM_WIDTH/2;
		//int		dh = ITEM_CTRLBAR_HEIGHT;

		g.DrawImage( &imgFV, 
			//Rect( dx + (dw-iw)/2, dy + (dh-ih)/2, iw, ih ),
			Rect( rtL.left, rtL.top, iw, ih ),
				0, 0, iw, ih, UnitPixel );



		CStringW	strImageSG = CAppData::GetResourcePath() + L"startgame1-normal.png";
		if( m_nRButtonState == 1 )
			strImageSG = CAppData::GetResourcePath() + L"startgame1-hover.png";
		else if( m_nRButtonState == 2 )
			strImageSG = CAppData::GetResourcePath() + L"startgame1-pushed.png";

		Image	imgSG( strImageSG );

		g.DrawImage( &imgSG,
			//Rect( dx + (dw-iw)/2 + dw, dy + (dh-ih)/2, iw, ih ),
			Rect( rtR.left, rtR.top, iw, ih ),
			0, 0, iw, ih, UnitPixel );

		g.DrawLine( &Pen(Color(51,255,255,255), 2), 
			rtR.left-1, rtR.top + (ITEM_CTRLBAR_HEIGHT-24)/2, rtR.left-1, rtR.top + (ITEM_CTRLBAR_HEIGHT+24)/2 );
	}
	else
	{
		Font ft( TFontFace, 24, FontStyleBold, UnitPixel );

		StringFormat	sf;
		sf.SetLineAlignment( StringAlignmentCenter );

		RectF		rtText( rt );
		rtText.Y += ITEM_IMAGE_HEIGHT;
		rtText.Height = ITEM_CTRLBAR_HEIGHT;

		CStringW	strText = m_asTitle[index];
		g.DrawString( strText, strText.GetLength(), &ft, rtText, &sf, &SolidBrush( Color(0x7fffffff) ));
	}
}


void CGalleryUI::reCalcLimit()
{
	int n = (int)m_asTitle.size();

	int nLine = n/3;
	if( n%3 )
		nLine++;

	int nRealHeight = nLine * (ITEM_HEIGHT+V_PADDING);
	m_offsetMax = nRealHeight - m_rcItem.Height();
}

void CGalleryUI::SetPos( RECT rc )
{
	CControlUI::SetPos( rc );
	reCalcLimit();
}

CDUIRect CGalleryUI::getItemRect( int index )
{
	CDUIRect		rt( m_rcItem );
	int w = rt.Width();
	int h = rt.Height();

	int	vIndex = index / 3;
	int	hIndex = index % 3;

	int x = hIndex * (ITEM_WIDTH + H_PADDING);
	int y = vIndex * (ITEM_HEIGHT + V_PADDING) - m_offsetY;

	return CDUIRect( x, y, x+ITEM_WIDTH, y+ITEM_HEIGHT );
}

void CGalleryUI::getItemButtonsRect( int index, CDUIRect &rtLeft, CDUIRect &rtRight )
{
	CDUIRect	rt = getItemRect( index );

	rtLeft = rt;
	rtLeft.right = rt.left + ITEM_WIDTH/2;
	rtLeft.top = rt.top + ITEM_IMAGE_HEIGHT;

	rtRight = rtLeft;
	rtRight.Offset( ITEM_WIDTH/2, 0 );
}


void CGalleryUI::eventPointToLogic( CDUIPoint pt, int &x, int &y )
{
	x = pt.x - m_rcItem.left;
	y = pt.y - m_rcItem.top;
}

CDUIPoint CGalleryUI::eventPointToLogic( CDUIPoint pt )
{
	int x, y;
	eventPointToLogic( pt, x, y );
	return CDUIPoint(x, y);
}


int CGalleryUI::hitTest( CDUIPoint pt )
{
	int		x, y;
	eventPointToLogic( pt, x, y );

	y += m_offsetY;

	if( x < 0 || y < 0 )
		return -1;

	int		hIndex = x / (ITEM_WIDTH + H_PADDING);
	if( x > hIndex*(ITEM_WIDTH + H_PADDING) + ITEM_WIDTH )
		return -1;

	int		vIndex = y / (ITEM_HEIGHT + V_PADDING);
	if( y > vIndex*(ITEM_HEIGHT + V_PADDING) + ITEM_HEIGHT )
		return -1;


	return vIndex*3 + hIndex;
}

void CGalleryUI::SetVSRatio( float f )
{
	int nNewOffset = int(m_offsetMax*f+0.5f);

	if( nNewOffset != m_offsetY )
	{
		m_offsetY = nNewOffset;
		Invalidate();
	}
}

void CGalleryUI::addTitleAndImageName( CStringW strTitle, CStringW strImageName )
{
	Image *img = addImageToLib( strImageName );
	if( img == nullptr )
		return;

	m_arImage.push_back( img );
	m_asTitle.push_back( strTitle );
}

Image* CGalleryUI::addImageToLib( CStringW strImage )
{
	auto it = m_mImageLib.find( strImage );
	if( it != m_mImageLib.end() )
		return it->second;

	CStringW	str = m_strImageFolder + strImage;
	Image	*img = new Image( str );
	m_mImageLib.insert( make_pair( strImage, img ) );
	return img;
}


