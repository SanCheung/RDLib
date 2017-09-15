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
{
	m_asText.push_back( L"NBA2018" );
	m_asText.push_back( L"星露谷物语" );
	m_asText.push_back( L"极速狂飙" );
	m_asText.push_back( L"星际争霸II" );
	m_asText.push_back( L"旅途" );
	m_asText.push_back( L"愤怒的小鸟" );
	m_asText.push_back( L"无双大蛇" );
	m_asText.push_back( L"黏土世界" );
	m_asText.push_back( L"上古卷轴7" );
	m_asText.push_back( L"文明6" );
}


CGalleryUI::~CGalleryUI(void)
{
}

void CGalleryUI::Event( TEventUI& evt )
{
	if( UIEVENT_SCROLLWHEEL == evt.Type)
	{
		//if( m_offsetMax > 0 )
		//{
		//	int nDir = LOWORD( evt.wParam );
		//	if( SB_LINEUP == nDir )
		//		m_offsetY += WHEELSTEP;
		//	else
		//		m_offsetY -= WHEELSTEP;

		//	if( m_offsetY < 0 )
		//		m_offsetY = 0;

		//	if( m_offsetY > m_offsetMax )
		//		m_offsetY = m_offsetMax;

		//	Invalidate();
		//	ATLTRACE( L"%d\n", m_offsetY );
		//}
	}
	else if( UIEVENT_MOUSEMOVE == evt.Type )
	{
		//int nNewHover = hitTest( evt.ptMouse );
		//if( nNewHover != m_nIndexHover )
		//{
		//	m_nIndexHover = nNewHover;
		//	Invalidate();

		//	//ATLTRACE( L"%d\n", nNewHover );
		//}
	}
	else if( UIEVENT_BUTTONUP == evt.Type )
	{
		//int nNewSelected = hitTest( evt.ptMouse );
		//if( nNewSelected != m_nIndexSelected )
		//{
		//	m_nIndexSelected = nNewSelected;
		//	Invalidate();

		//	GetManager()->SendNotify( this, L"itemClick", nNewSelected );

		//	//ATLTRACE( L"%d\n", nNewHover );
		//}
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

	//Pen			pen( Color::Red );
	//g.DrawLine( &pen, 0, 0, w, h );
	//g.DrawLine( &pen, w, 0, 0, h );

	int n = (int)m_asText.size();
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
	g.FillRectangle( &SolidBrush( Color(0x7f7f7fff) ), Rect( rt.X, rt.Y, ITEM_WIDTH, ITEM_IMAGE_HEIGHT) );
	
	
	Font			ft( TFontFace, 24, FontStyleBold );

	StringFormat	sf;
	sf.SetLineAlignment( StringAlignmentCenter );

	RectF		rtText( rt );
	rtText.Y += ITEM_IMAGE_HEIGHT;
	rtText.Height = ITEM_CTRLBAR_HEIGHT;

	CStringW	strText = m_asText[index];
	g.DrawString( strText, strText.GetLength(), &ft, rtText, &sf, &SolidBrush( Color(0x7fffffff) ));


	g.DrawRectangle( &Pen(Color::Red,2), rtText );
}


void CGalleryUI::reCalcLimit()
{
	int n = (int)m_asText.size();

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

int CGalleryUI::hitTest( CDUIPoint pt )
{
	//int y = pt.y + m_offsetY - m_rcItem.top;
	//if( y < 0 )
	//	return -1;

	//int		index = y / LINEHEIGHT;
	//if( y > index*LINEHEIGHT && y < index*LINEHEIGHT + TFontSize+10 )
	//	return index;

	return -1;
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
