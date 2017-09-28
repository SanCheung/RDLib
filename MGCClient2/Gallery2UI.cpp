#include "StdAfx.h"
#include "Gallery2UI.h"
#include "SampleWnd.h"

#define		ITEM_WIDTH		496

#define		ITEM_IMAGE_HEIGHT	279
#define		ITEM_TEXT_FONTSIZE	24
#define		ITEM_CTRLBAR_HEIGHT 62
#define		ITEM_HEIGHT		(ITEM_IMAGE_HEIGHT+ITEM_CTRLBAR_HEIGHT)

#define		H_PADDING		30
#define		V_PADDING		10

#define		TFontFace		L"Microsoft Yahei"


CGallery2UI::CGallery2UI(void)
	: m_offsetY( 0 )
	, m_offsetMax( 0 )

	, m_nIndexHover( -1 )
	, m_nLButtonState( -1 )
	, m_nRButtonState( -1 )

	, m_bMouseTracking( false )
{
	setImageFolder( CAppData::GetResourcePath() );
}


CGallery2UI::~CGallery2UI(void)
{
}

void CGallery2UI::DoDraw()
{
	CDUIRect	rt;
	GetWindowRect( m_hWnd, &rt );

	int w = m_rt.GetWidth();
	int h = m_rt.GetHeight();

	float iw  = m_pBg->GetPixelSize().width;
	float ih  = m_pBg->GetPixelSize().height;

	float x = rt.left * (iw/w);
	float y = rt.top *(ih/h);


	D2D1_RECT_F d2rt1 = { 0, 0, w, h };
	D2D1_RECT_F d2rt2 = { x, y, iw, ih };
	_de._render_target->DrawBitmap( m_pBg, &d2rt1, 1.f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, &d2rt2 );

	int n = (int)m_asTitle.size();
	int i;
	for( i = 0; i < n; i++ )
	{
		CDUIRect	rt = getItemRect( i );
		if( rt.top < -ITEM_HEIGHT || rt.top > h )
			continue;
			
		drawItem( rt, i );
	}
}

void CGallery2UI::drawItem( CDUIRect rt, int index )
{
	ID2D1Bitmap* pBmp = m_arImage[index];
	_de.d2DrawBitmap( pBmp, rt.left, rt.top, ITEM_WIDTH, ITEM_IMAGE_HEIGHT );

	if( m_nIndexHover == index )
	{
		ID2D1Bitmap *pBmpFS = m_arFSImage[0];
		if( m_nLButtonState == 1 )
			pBmpFS = m_arFSImage[1];
		else if( m_nLButtonState == 2 )
			pBmpFS = m_arFSImage[2];

		CDUIRect	rtL, rtR;
		getItemButtonsRect( index, rtL, rtR );

		float iw = pBmpFS->GetPixelSize().width;
		float ih = pBmpFS->GetPixelSize().height;
		_de.d2DrawBitmap( pBmpFS, rtL.left, rtL.top, iw, ih );

		ID2D1Bitmap *pBmpSG = m_arSGImage[0];
		if( m_nRButtonState == 1 )
			pBmpSG = m_arSGImage[1];
		else if( m_nRButtonState == 2 )
			pBmpSG = m_arSGImage[2];

		_de.d2DrawBitmap( pBmpSG, rtR.left, rtR.top, iw, ih );

		D2D1_COLOR_F	d2Color = {1.f, 1.f, 1.f, .2f};

		D2D1_POINT_2F	pt0 = { rtR.left-1, rtR.top + (ITEM_CTRLBAR_HEIGHT-24)/2 };
		D2D1_POINT_2F	pt1 = { rtR.left-1, rtR.top + (ITEM_CTRLBAR_HEIGHT+24)/2 };
		_de._render_target->DrawLine( pt0, pt1, _de.dl_getSolidBrush( d2Color ), 2.f );
	}
	else
	{
		IDWriteTextFormat* font = _de.dl_getFont( TFontFace, 24, 700 );

		D2D1_COLOR_F	d2Color = {1.f, 1.f, 1.f, .5f};

		CStringW	strText = m_asTitle[index];
		_de.d2DrawText( strText, rt.left, rt.top+ITEM_IMAGE_HEIGHT, rt.Width(), ITEM_CTRLBAR_HEIGHT,
			font, _de.dl_getSolidBrush( d2Color ) );
	}
}


LRESULT CGallery2UI::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	bool bHandled = false;

	if( WM_CREATE == uMsg )
	{
		m_pVideoWnd = new CSampleWnd;
		m_pVideoWnd->Create( m_hWnd, L"", UI_WNDSTYLE_CHILD, 0 );

		bHandled = true;
	}
	else if( WM_MOUSELEAVE == uMsg )
	{
		if( m_bMouseTracking ) 
			::SendMessage(m_hWnd, WM_MOUSEMOVE, 0, (LPARAM) -1);

		m_bMouseTracking = false;

		m_nIndexHover = -1;
		m_nLButtonState = -1;
		m_nRButtonState = -1;
		Invalidate();

		bHandled = true;
	}
	else if( WM_MOUSEWHEEL == uMsg )
	{
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		::ScreenToClient( m_hWnd, &pt);
		if( PtInRect( &m_rt, pt ) )
		{
			if( m_offsetMax >= 0 )
			{
				//int nDir = LOWORD( wParam );
				//if( SB_LINEUP == nDir )
				int	  nDelta = GET_WHEEL_DELTA_WPARAM( wParam );
				if( nDelta > 0 )
					m_offsetY -= 50;
				else
					m_offsetY += 50;

				if( m_offsetY < 0 )
					m_offsetY = 0;

				if( m_offsetY > m_offsetMax )
					m_offsetY = m_offsetMax;

				Invalidate();

				int pc = int( m_offsetY*100.f/m_offsetMax);
				::SendMessage( GetParent( m_hWnd ), WM_GLY_SETVS_RATIO, pc, 0 );
			}
		}

		bHandled = true;
	}
	else if( WM_MOUSEMOVE == uMsg )
	{
		//SetFocus( m_hWnd );
		if( !m_bMouseTracking )
		{
			TRACKMOUSEEVENT tme = { 0 };
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_HOVER | TME_LEAVE;
			tme.hwndTrack = m_hWnd;
			tme.dwHoverTime = 500;
			_TrackMouseEvent(&tme);
			m_bMouseTracking = true;
		}

		CDUIPoint	pt( LOWORD(lParam), HIWORD(lParam) );
		//if( !PtInRect( m_rt, pt ) )
		//	ReleaseCapture();
		//else
		//	SetCapture( m_hWnd );

		int nNewHover = hitTest( pt );
		if( nNewHover != m_nIndexHover )
		{
			//if( nNewHover == -1 )
			//{
			//	//m_pVideoWnd->ShowWindow( false );
			//	//ReleaseCapture();
			//}
			//else
			//{
			//	CDUIRect	rtItem = getItemRect( nNewHover );
			//	m_pVideoWnd->MoveWindow( rtItem.left, rtItem.top, ITEM_WIDTH, ITEM_IMAGE_HEIGHT );
			//	if( !::IsWindowVisible( m_pVideoWnd->GetHWND() ) )
			//		//m_pVideoWnd->ShowWindow( true );
			//		::ShowWindow( m_pVideoWnd->GetHWND(), SW_SHOW );
			//}

			PostMessage( WM_GL_SHOWVIDEOWND, nNewHover );

			m_nIndexHover = nNewHover;
			//Invalidate();
			//ATLTRACE( L"%d\n", nNewHover );
		}

		if( nNewHover != -1 )
		{
			CDUIPoint	ptLogic = eventPointToLogic( pt );

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

			PostMessage( WM_GL_SHOWVIDEOWND, nNewHover );
		}

		Invalidate();
		bHandled = true;
	}
	else if( WM_LBUTTONDOWN == uMsg )
	{
		CDUIPoint	pt( LOWORD(lParam), HIWORD(lParam) );
		int nNewHover = hitTest( pt );
		if( nNewHover != -1 )
		{
			CDUIPoint	ptLogic = eventPointToLogic( pt );

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

		bHandled = true;
	}
	else if( WM_LBUTTONUP == uMsg )
	{
		CDUIPoint	pt( LOWORD(lParam), HIWORD(lParam) );
		int nNewHover = hitTest( pt );
		if( nNewHover != -1 )
		{
			CDUIPoint	ptLogic = eventPointToLogic( pt );

			CDUIRect	rtL, rtR;
			getItemButtonsRect( nNewHover, rtL, rtR );

			if( PtInRect( rtL, ptLogic ) )
				::SendMessage( GetParent( m_hWnd ), WM_GLY_CLICK_LEFT, nNewHover, 0 );

			if( PtInRect( rtR, ptLogic ) )
				::SendMessage( GetParent( m_hWnd ), WM_GLY_CLICK_RIGHT, nNewHover, 0 );
		}

		bHandled = true;
	}
	else if( WM_GL_SHOWVIDEOWND == uMsg )
	{
		int nNewHover = (int)wParam;
		if( nNewHover == -1 )
		{
			m_pVideoWnd->ShowWindow( false );
			//ReleaseCapture();
		}
		else
		{
			CDUIRect	rtItem = getItemRect( nNewHover );
			m_pVideoWnd->MoveWindow( rtItem.left, rtItem.top, ITEM_WIDTH, ITEM_IMAGE_HEIGHT );
			if( !::IsWindowVisible( m_pVideoWnd->GetHWND() ) )
				m_pVideoWnd->ShowWindow( true );
		}

		bHandled = true;
	}


	if( bHandled )
		return 0;

	return CD2HostableWnd::HandleMessage( uMsg, wParam, lParam );
}

void CGallery2UI::dh_BuildAllBitmap()
{
	::GetClientRect( m_hWnd, m_rt );

	m_pBg = _de.dl_getImage( m_strImageFolder + L"bg.png" );

	m_arFSImage.push_back( _de.dl_getImage( m_strImageFolder + L"fullvideo-normal.png" ) );
	m_arFSImage.push_back( _de.dl_getImage( m_strImageFolder + L"fullvideo-hover.png" ) );
	m_arFSImage.push_back( _de.dl_getImage( m_strImageFolder + L"fullvideo-pushed.png" ) );

	m_arSGImage.push_back( _de.dl_getImage( m_strImageFolder + L"startgame1-normal.png" ) );
	m_arSGImage.push_back( _de.dl_getImage( m_strImageFolder + L"startgame1-hover.png" ) );
	m_arSGImage.push_back( _de.dl_getImage( m_strImageFolder + L"startgame1-pushed.png" ) );

	//addTitleAndImageName( L"NBA2018",	L"game1.png" );
	//addTitleAndImageName( L"星露谷物语", L"game2.png" );
	//addTitleAndImageName( L"极速狂飙",	L"game3.png" );
	//addTitleAndImageName( L"星际争霸II", L"game2.png" );
	//addTitleAndImageName( L"旅途",		L"game1.png" );
	//addTitleAndImageName( L"愤怒的小鸟", L"game3.png" );
	//addTitleAndImageName( L"无双大蛇",	L"game2.png" );
	//addTitleAndImageName( L"黏土世界",	L"game3.png" );
	//addTitleAndImageName( L"上古卷轴7",	L"game1.png" );
	//addTitleAndImageName( L"文明6",		L"game3.png" );


	reCalcLimit();
}




bool CGallery2UI::reCalcLimit()
{
	m_offsetY = 0;

	int n = (int)m_asTitle.size();

	int nLine = n/3;
	if( n%3 )
		nLine++;

	int nRealHeight = nLine * (ITEM_HEIGHT+V_PADDING) - V_PADDING;
	m_offsetMax = nRealHeight - m_rt.Height();

	if( m_offsetMax < 0 )
	{
		m_offsetMax = 0;
		return false;
	}

	return true;
}


CDUIRect CGallery2UI::getItemRect( int index )
{
	CDUIRect		rt( m_rt );
	int w = rt.Width();
	int h = rt.Height();

	int	vIndex = index / 3;
	int	hIndex = index % 3;

	int x = hIndex * (ITEM_WIDTH + H_PADDING);
	int y = vIndex * (ITEM_HEIGHT + V_PADDING) - m_offsetY;

	return CDUIRect( x, y, x+ITEM_WIDTH, y+ITEM_HEIGHT );
}

void CGallery2UI::getItemButtonsRect( int index, CDUIRect &rtLeft, CDUIRect &rtRight )
{
	CDUIRect	rt = getItemRect( index );

	rtLeft = rt;
	rtLeft.right = rt.left + ITEM_WIDTH/2;
	rtLeft.top = rt.top + ITEM_IMAGE_HEIGHT;

	rtRight = rtLeft;
	rtRight.Offset( ITEM_WIDTH/2, 0 );
}


void CGallery2UI::eventPointToLogic( CDUIPoint pt, int &x, int &y )
{
	x = pt.x - m_rt.left;
	y = pt.y - m_rt.top;
}

CDUIPoint CGallery2UI::eventPointToLogic( CDUIPoint pt )
{
	int x, y;
	eventPointToLogic( pt, x, y );
	return CDUIPoint(x, y);
}


int CGallery2UI::hitTest( CDUIPoint pt )
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


	int index = vIndex*3 + hIndex;
	if( index >= (int)m_asTitle.size() )
		return -1;

	return index;
}

void CGallery2UI::SetVSRatio( float f )
{
	if( m_offsetMax == 0 )
		return;

	int nNewOffset = int(m_offsetMax*f+0.5f);

	if( nNewOffset != m_offsetY )
	{
		m_offsetY = nNewOffset;
		Invalidate();
	}
}

void CGallery2UI::addTitleAndImageName( CStringW strTitle, CStringW strImageName )
{
	CStringW	strFullPath = m_strImageFolder + strImageName;
	ID2D1Bitmap* d2Bmp = _de.dl_getImage( strFullPath );

	if( d2Bmp != nullptr )
	{
		m_asTitle.push_back( strTitle );
		m_arImage.push_back( d2Bmp );
	}
}

void CGallery2UI::clearAllItems()
{
	m_asTitle.clear();
	m_arImage.clear();
}

bool CGallery2UI::resetAllItems()
{
	bool bScrollEnable = reCalcLimit();
	Invalidate();
	return bScrollEnable;
}

void CGallery2UI::setData( const vector<mgcGameInfo> &arGameInfo, CStringW strType )
{
	clearAllItems();

	int n = (int)arGameInfo.size();
	for( int i = 0; i < n; ++i )
	{
		const mgcGameInfo &gi = arGameInfo[i];

		CStringW	strText  = gi.gameName.c_str();
		CStringW	strImage = gi.gameCover.c_str();
		CStringW	strFamily = gi.gameFamily.c_str();
		if( -1 != strFamily.Find( strType+L"," ) )
			addTitleAndImageName( strText, strImage );
	}
}
