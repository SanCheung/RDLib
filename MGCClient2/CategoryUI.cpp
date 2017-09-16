#include "StdAfx.h"
#include "CategoryUI.h"

// Text Color + State
#define		TCNormal		0xFF5d6b77
#define		TCHover			0xFFcccccc
#define		TCSelected		0xFF27b5ff

// Text Font
#define		TFontFace		L"Microsoft Yahei"
#define		TFontSize		30
#define		LINEPadding		50
#define		LINEHEIGHT		(TFontSize+LINEPadding)
#define		WHEELSTEP		20


CCategoryUI::CCategoryUI(void)
	: m_nIndexHover( -1 )
	, m_nIndexSelected( -1 )
	, m_offsetY( 0 )
	, m_offsetMax( 0 )
{
	m_asText.push_back( L"推荐游戏" );
	m_asText.push_back( L"射击" );
	m_asText.push_back( L"动作" );
	m_asText.push_back( L"双人对战" );
	m_asText.push_back( L"解密" );
	m_asText.push_back( L"像素风" );
	m_asText.push_back( L"动作冒险" );
	m_asText.push_back( L"推塔" );
}


CCategoryUI::~CCategoryUI(void)
{
}

void CCategoryUI::Event( TEventUI& evt )
{
	if( UIEVENT_SCROLLWHEEL == evt.Type)
	{
		if( m_offsetMax > 0 )
		{
			int nDir = LOWORD( evt.wParam );
			if( SB_LINEUP == nDir )
				m_offsetY -= WHEELSTEP;
			else
				m_offsetY += WHEELSTEP;

			if( m_offsetY < 0 )
				m_offsetY = 0;

			if( m_offsetY > m_offsetMax )
				m_offsetY = m_offsetMax;

			Invalidate();
			ATLTRACE( L"%d\n", m_offsetY );
		}
	}
	else if( UIEVENT_MOUSEMOVE == evt.Type )
	{
		int nNewHover = hitTest( evt.ptMouse );
		if( nNewHover != m_nIndexHover )
		{
			m_nIndexHover = nNewHover;
			Invalidate();

			//ATLTRACE( L"%d\n", nNewHover );
		}
	}
	else if( UIEVENT_BUTTONUP == evt.Type )
	{
		int nNewSelected = hitTest( evt.ptMouse );
		if( nNewSelected != m_nIndexSelected )
		{
			m_nIndexSelected = nNewSelected;
			Invalidate();

			GetManager()->SendNotify( this, L"itemClick", nNewSelected );

			//ATLTRACE( L"%d\n", nNewHover );
		}
	}

	CControlUI::Event( evt );
}

void CCategoryUI::DoPaint( HDC hDC, const RECT& rcPaint )
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

void CCategoryUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	CControlUI::SetAttribute( pstrName, pstrValue );
}

void CCategoryUI::draw( Bitmap *pBmp )
{
	int h = pBmp->GetHeight();

	Graphics	g( pBmp );
	g.SetTextRenderingHint( TextRenderingHintAntiAlias );

	int n = (int)m_asText.size();
	int i;
	for( i = 0; i < n; i++ )
	{
		CDUIRect	rt = getItemRect( i );
		if( rt.top < -LINEHEIGHT || rt.top > h )
			continue;

		RectF		rtItem( rt.left, rt.top, rt.Width(), rt.Height() );
		drawItem( g, rtItem, i );
	}
}

void CCategoryUI::drawItem( Graphics &g, RectF rt, int index )
{
	Font			ft( TFontFace, TFontSize );
	Font			ft2( TFontFace, TFontSize, FontStyleBold );

	Color		cr( TCNormal );
	if( index == m_nIndexHover )
		cr.SetValue( TCHover );

	if( index == m_nIndexSelected )
		cr.SetValue( TCSelected );

	StringFormat	sf;
	//sf.SetAlignment( StringAlignmentCenter );
	sf.SetLineAlignment( StringAlignmentCenter );

	CStringW	strText = m_asText[index];

	if( index == m_nIndexSelected )
		g.DrawString( strText, strText.GetLength(), &ft2, rt, &sf, &SolidBrush( cr ));
	else
		g.DrawString( strText, strText.GetLength(), &ft, rt, &sf, &SolidBrush( cr ));

	//CDUIRect rt2 = getItemRect(index);
	//g.DrawRectangle( &Pen(Color::White), rt2.left, rt2.top, rt2.Width(), rt2.Height() );
}

CDUIRect CCategoryUI::getItemRect( int index )
{
	CDUIRect		rt( m_rcItem );
	int w = rt.Width();
	int h = rt.Height();

	int y = index * LINEHEIGHT - m_offsetY;

	return CDUIRect( 0, y, w, y+TFontSize+15 );
}

int CCategoryUI::hitTest( CDUIPoint pt )
{
	int y = pt.y + m_offsetY - m_rcItem.top;
	if( y < 0 )
		return -1;

	int		index = y / LINEHEIGHT;
	if( y > index*LINEHEIGHT && y < index*LINEHEIGHT + TFontSize+10 )
		return index;

	return -1;
}

void CCategoryUI::reCalcLimit()
{
	int n = (int)m_asText.size();

	int nRealHeight = n * LINEHEIGHT;
	m_offsetMax = nRealHeight - m_rcItem.Height();
}

void CCategoryUI::SetPos( RECT rc )
{
	CControlUI::SetPos( rc );
	reCalcLimit();
}


