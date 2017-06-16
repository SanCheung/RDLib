#include "StdAfx.h"
#include "D2ViewUI.h"

#include "WndUI.h"

#include <math.h>
#define		IDT_ANIMATION		100


////////////////////////////////////////////////////////////////////////////////
CD2ViewUI::CD2ViewUI(void)
	//: _at( ImageState( 100.f, 100.f, 100.f, 100.f, 0.f, .5f ),
	//	   ImageState( 500.f, 200.f, 200.f, 200.f, 0.f, 1.f ),
	//	   50 )
{
	//_sp.reset( new Image( L"d:/info.png" ));
	//_angle = 0;
	//_alpha = 100;

	_nOffsetIcon = 3;
	_bToRight = true;
	_animing = false;

	_spBK2 = nullptr;
	_spIconBK2 = nullptr;

	_d2Font = nullptr;
	_d2sb = nullptr;

	_bPanning = false;
	_bClickItem = false;
}


CD2ViewUI::~CD2ViewUI(void)
{
	_asImageFile.clear();
	_asText.clear();

	_de.d2_safe_release( _spBK2 );
	_de.d2_safe_release( _spIconBK2 );
	for( auto it = _mapImage2.begin(); it != _mapImage2.end(); ++it )
	{
		d2Bitmap *bmp = it->second;
		_de.d2_safe_release( bmp );
	}
	_mapImage2.clear();

	_de.d2_safe_release( _d2Font );
	_de.d2_safe_release( _d2sb );
}

LRESULT CD2ViewUI::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = 0;
	BOOL handled = FALSE;
	if(uMsg == WM_PAINT)
	{
		_de.d2Draw( this );
		if( _animing )
		{
			AniProc();
		}
		else
		{
			// 停止绘制
			HWND	hWnd = GetHWND();
			PAINTSTRUCT ps = { 0 };
			HDC hdc = ::BeginPaint( hWnd, &ps);
			::EndPaint(hWnd, &ps);
		}

		handled = TRUE;
	}
	else if( uMsg == WM_LBUTTONDBLCLK )
	{
		m_pHost->SetFocus();
		SetFocus(m_hWnd);

		SetCapture(m_hWnd);

		UINT	nFlags = (UINT)wParam;
		POINT	point;
		point.x = GET_X_LPARAM(lParam);
		point.y = GET_Y_LPARAM(lParam);
		OnDoubleClick(nFlags, point);
	}
	else if( uMsg == WM_LBUTTONDOWN )
	{
		SetCapture(m_hWnd);

		UINT	nFlags = 0;
		POINT	point;

		nFlags = (UINT)wParam;
		point.x = GET_X_LPARAM(lParam);
		point.y = GET_Y_LPARAM(lParam);
		m_bMouseDown = true;
		OnLButtonDown(nFlags, point);

		InvalidateRect( GetHWND(), NULL, FALSE );
	}
	else if( uMsg == WM_LBUTTONUP )
	{	
		UINT	nFlags = 0;
		POINT	point;

		nFlags = (UINT)wParam;
		point.x = GET_X_LPARAM(lParam);
		point.y = GET_Y_LPARAM(lParam);

		m_bMouseDown = false;
		OnLButtonUp(nFlags, point);

		ReleaseCapture();
	}
	else if( uMsg == WM_MOUSEMOVE )
	{
		UINT	nFlags = (UINT)wParam;
		POINT	point  = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		OnMouseMove(nFlags, point);
	}
	else if( uMsg == WM_KILLFOCUS )
	{
		lResult = 1;
		handled = TRUE;
	}
	//else if( uMsg == WM_TIMER )
	//{
	//	AniProc();
	//	lResult = 1;
	//	handled = TRUE;
	//}
	else if( uMsg == WM_SIZE )
	{
		lResult = 1;
		handled = TRUE;

		int w = LOWORD(lParam);
		int h = HIWORD(lParam);

		if( w > 0 && h > 0 )
		{
			_de.d2InitHRT( GetHWND() );
			_de.d2Resize( w, h );
			InitImageStateArray();
			BuildAllBitmap();
		}
	}
	else if( WM_GESTURE == uMsg )
	{
		lResult = 1;
		handled = TRUE;

		//OnMessageGesture( wParam, lParam );
	}


	if (!handled)
	{
		lResult = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

	return lResult;
}



void CD2ViewUI::InitImageStateArray()
{
	if( _asImageFile.empty() )
		return;

	CDUIRect	rt;
	GetClientRect( m_hWnd, &rt );

	//// 获取图片和文字
	//CStringW	strImageFile = _asImageFile[0];
	//shared_ptr<Image> sp = _mapImage.find(strImageFile)->second;

	//float iw = (float)sp->GetWidth();
	//float ih = (float)sp->GetHeight();

	CDUIPoint cp = rt.CenterPoint();

	float x = (float)cp.x;
	float y = (float)cp.y;

	//float r = rt.GetWidth()*.5f;
	//float angle = 3.1416f/9.f;

	////原先样式
	//float sina = sin( angle );
	//float cosa = cos( angle );

	//float sina2 = sin( angle*2.f );
	//float cosa2 = cos( angle*2.f );

	//PointF		pts[5] = {
	//	PointF( x - r*sina2, y - r + r*cosa2 ),
	//	PointF( x - r*sina, y - r + r*cosa ),
	//	PointF( x, y ),
	//	PointF( x + r*sina, y - r+ r*cosa ),
	//	PointF( x + r*sina2, y - r + r*cosa2 )
	//};

	//float z = .618f;
	//float z2 = .3f;

	//float a = .5f;
	//float a2 = .2f;

	//_iss[0] = ImageState( pts[0].X-iw*z2/2, pts[0].Y-ih*z2/2, iw*z2, ih*z2, 0, a2 );
	//_iss[1] = ImageState( pts[1].X-iw*z/2, pts[1].Y-ih*z/2, iw*z, ih*z, 0, a );
	//_iss[2] = ImageState( pts[2].X-iw/2, pts[2].Y-ih/2, iw, ih, 0, 1.f );
	//_iss[3] = ImageState( pts[3].X-iw*z/2, pts[3].Y-ih*z/2, iw*z, ih*z, 0, a );
	//_iss[4] = ImageState( pts[4].X-ih*z2/2, pts[4].Y-ih*z2/2, iw*z2, ih*z2, 0, a2 );

	float r = rt.GetWidth()/5.f;
	if( r > 300 )
		r = rt.GetWidth()/6.f;


	PointF		pts[5] = {
		PointF( x - r*2+20, y ),
		PointF( x - r, y ),
		PointF( x, y ),
		PointF( x + r, y ),
		PointF( x + r*2-20, y )
	};

	float	
		wb = 248, 
		hb = 288,
		wm = 228, 
		hm = 268,
		ws = 208, 
		hs = 248;


	//float a = .7f;
	//float a2 = .5f;

	//_iss[0] = ImageState( pts[0].X-wb/2, pts[0].Y-hb/2, wb, hb, 0, a2 );
	//_iss[1] = ImageState( pts[1].X-wm/2, pts[1].Y-hm/2, wm, hm, 0, a );
	//_iss[2] = ImageState( pts[2].X-ws/2, pts[2].Y-hs/2, ws, hs, 0, 1.f );
	//_iss[3] = ImageState( pts[3].X-wm/2, pts[3].Y-hm/2, wm, hm, 0, a );
	//_iss[4] = ImageState( pts[4].X-wb/2, pts[4].Y-hb/2, wb, hb, 0, a2 );

	_iss[0] = ImageState( pts[0].X, pts[0].Y, ws, hs, 0, .5f, 1.f );
	_iss[1] = ImageState( pts[1].X, pts[1].Y, wm, hm, 0, .7f, .7f );
	_iss[2] = ImageState( pts[2].X, pts[2].Y, wb, hb, 0, 1.f, .5f );
	_iss[3] = ImageState( pts[3].X, pts[3].Y, wm, hm, 0, .7f, .7f );
	_iss[4] = ImageState( pts[4].X, pts[4].Y, ws, hs, 0, .5f, 1.f );
}



void CD2ViewUI::DrawImageByState( Graphics *pGr, shared_ptr<Image> sp, ImageState *s )
{
	//if( s->_a == 0 )
	//	return;

	bool bHasRotate = (abs(s->_r) > 1e-4);

	if( bHasRotate )
	{
		Matrix		mx;
		mx.RotateAt( s->_r, PointF( s->_x, s->_y) );
		pGr->SetTransform( &mx );
	}

	ImageAttributes		ia2;
	if( s->_a2 > 0.f && s->_a2 < 1.f )
	{
		ColorMatrix		mxColor =  {
			1.f, 0.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 0.f, s->_a2, 0.f,
			0.f, 0.f, 0.f, 0.f, 1.f,
		};

		ia2.SetColorMatrix( &mxColor );
	}

	//Image *img = _spBg3[1].get();
	pGr->DrawImage( _spIconBK.get(), RectF(s->_x-s->_w/2, s->_y-s->_h/2, s->_w, s->_h ), 
		0, 0, (REAL)_spIconBK->GetWidth(), (REAL)_spIconBK->GetHeight(), UnitPixel, &ia2 );

	ImageAttributes		ia;
	if( s->_a > 0.f && s->_a < 1.f )
	{
		ColorMatrix		mxColor =  {
			1.f, 0.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 0.f, s->_a, 0.f,
			0.f, 0.f, 0.f, 0.f, 1.f,
		};

		ia.SetColorMatrix( &mxColor );
	}

	float iw = (float)sp->GetWidth();
	float ih = (float)sp->GetHeight();
	pGr->DrawImage( sp.get(), RectF(s->_x-iw/2, s->_y-ih/2-20, iw, ih ), 
		0, 0, iw, ih, UnitPixel, &ia );

	if( bHasRotate )
		pGr->ResetTransform();

	//pGr->FillEllipse( &SolidBrush(Color::Red), s->_x-5, s->_y-5, 10.f, 10.f );
}


void CD2ViewUI::DrawTextByState( Graphics *pGr, CStringW strText, ImageState *s )
{
	Font			ft( L"微软雅黑", 18 );
	StringFormat	sf;
	sf.SetAlignment( StringAlignmentCenter );

	CDUIRect	rtClient;
	GetClientRect( m_hWnd, &rtClient );
	CDUIPoint cp = rtClient.CenterPoint();

	int  r, g, b;
	r = g = b = 51;
	if( abs( s->_x - cp.x ) < 20 )
		r = g = b = 255;

	RectF			rt( s->_x-80.f, s->_y+50, 160.f, 30.f );
	pGr->DrawString( strText, strText.GetLength(), &ft, rt, &sf, &SolidBrush(Color(BYTE(s->_a*255), r, g, b)) );
}


void CD2ViewUI::DoDraw()
{
	CDUIRect	rt;
	GetClientRect( m_hWnd, &rt );

	int w = rt.GetWidth();
	int h = rt.GetHeight();

	_de.d2DrawBitmap( _spBK2, 0, 0, (float)w, (float)h );

	for( int i = 0; i < 5; i++ )
	{
		int nObjectCount = GetObjectCount();
		int nObjectIndex = (i+_nOffsetIcon)%nObjectCount;

		CStringW	strImageFile = _asImageFile[nObjectIndex];
		CStringW	strText = _asText[nObjectIndex];

		ImageState	*s = &_iss[ i ];
		if( aj_valid() )
			s = aj_getIs( i );

		d2DrawItem( strImageFile, strText, s );
	}

	//_de.d2DrawText( L"刘德华 欢迎您！", 0, 0, 200, 20, _d2Font, _d2sb );
}






void CD2ViewUI::Draw( Graphics &g )
{
	g.SetSmoothingMode( _animing ? SmoothingModeHighSpeed : SmoothingModeAntiAlias);

	CDUIRect	rt;
	GetClientRect( m_hWnd, &rt );

	// 白色背景
	if( _spBK == nullptr )
		g.FillRectangle( &SolidBrush(Color::AliceBlue), 0, 0, rt.Width(), rt.Height() );
	else
		g.DrawImage( _spBK.get(), Rect(0,0,rt.Width(),rt.Height()), 
		0, 0, _spBK->GetWidth(), _spBK->GetHeight(), UnitPixel );

	if( _asImageFile.empty() )
	{
		Font			ft( L"Arial", 15 );
		StringFormat	sf;
		sf.SetAlignment( StringAlignmentCenter );
		g.DrawString( L"没有图片！", 5, &ft, PointF(100,100), &sf, &SolidBrush(Color::Black) );
		return;
	}

	for( int i = 0; i < 5; i++ )
	{
		int nObjectCount = GetObjectCount();
		int nObjectIndex = (i+_nOffsetIcon)%nObjectCount;

		CStringW	strImageFile = _asImageFile[nObjectIndex];
		CStringW	strText = _asText[nObjectIndex];

		ImageState	*s = &_iss[ i ];
		if( aj_valid() )
			s = aj_getIs( i );

		shared_ptr<Image> sp = _mapImage.find(strImageFile)->second;
		DrawImageByState( &g, sp, s );
		DrawTextByState( &g, strText, s );
	}
}

void CD2ViewUI::DoPaint( HDC hDC )
{
	Graphics	g( hDC );
	g.SetSmoothingMode( SmoothingModeHighSpeed );

	CDUIRect	rt;
	GetClientRect( m_hWnd, &rt );

	Bitmap		bmp( rt.Width(), rt.Height()+1 );
	Graphics	g2( &bmp );
	Draw( g2 );

	g.DrawImage( &bmp, 0, 0 );
}

void CD2ViewUI::OnLButtonDown( UINT nFlags, POINT point )
{
}

void CD2ViewUI::OnLButtonUp( UINT nFlags, POINT point )
{
	if( _animing )
		return;

	int i = HitTest( point.x, point.y );

	_bClickItem = true;
	if( i == 0 )
		AniStart( false, 2 );
	if( i == 1 )
		AniStart( false );
	else if( i == 2 )
		m_pHost->GetManager()->SendNotify( m_pHost,  L"itemclick", GetCurrentIndex() );
	else if( i == 3 )
		AniStart( true );
	else if( i == 4 )
		AniStart( true, 2 );

}

void CD2ViewUI::OnMouseMove( UINT nFlags, POINT point )
{

}

void CD2ViewUI::OnDoubleClick( UINT nFlags, POINT point )
{

}

CD2ViewUI* CD2ViewUI::CreateUI( HWND hWnd )
{
	CD2ViewUI* pThis = new CD2ViewUI;
	pThis->Create( hWnd, L"", UI_WNDSTYLE_CHILD, 0 );

	CControlUI *pControl = new CWndUI( pThis );
	pThis->m_pHost = pControl;

	return pThis;
}


void CD2ViewUI::ReloadBKimage()
{
	CStringW	strFolder = CAppData::GetInstancePath();
	strFolder += _strIconSubFolder;

	m_strFileBK = strFolder + L"背景.png" ;
	m_strFileIconBK = strFolder + L"bgb.png";
}


void CD2ViewUI::AddIconAndText( CStringW strImageFile, CStringW strText )
{
	_asImageFile.push_back( strImageFile );
	_asText.push_back( strText );

	CStringW	strFolder = CAppData::GetInstancePath();
	strFolder += _strIconSubFolder;

	auto it = _mapName2File.find( strImageFile );
	if( it == _mapName2File.end() )
		_mapName2File.insert( make_pair( strImageFile, strFolder + strImageFile ) );
}

void CD2ViewUI::AniStart( bool bToRight, int nTime )
{
	if( _animing )
		return;

	aj_clearAll();

	int i;
	if( bToRight )
	{
		for( i = 0; i < 5; i++ )
			aj_addTask( new CAniTask( &_iss[i], &_iss[(i+4)%5], 20) );
	}
	else
	{
		for( i = 0; i < 5; i++ )
			aj_addTask( new CAniTask( &_iss[i], &_iss[(i+1)%5], 20) );
	}

	_bToRight = bToRight;
	_nTime = nTime;
	_animing = true;

	InvalidateRect( m_hWnd, NULL, FALSE );
}

void CD2ViewUI::AniEnd()
{
	_animing = false;
	_nTime = 0;

	if( _bClickItem )
	{
		m_pHost->GetManager()->SendNotify( m_pHost,  L"itemclick", GetCurrentIndex() );
		_bClickItem = false;
	}
}

void CD2ViewUI::AniProc()
{
	if( !aj_step() )
	{
		_nTime--;

		int nObjectCount = GetObjectCount();

		if( _bToRight )
			_nOffsetIcon = (_nOffsetIcon+1)%nObjectCount;
		else
			_nOffsetIcon = (_nOffsetIcon+nObjectCount-1)%nObjectCount;

		if( _nTime == 0 )
			AniEnd();
	}

	Sleep(1);
}

void CD2ViewUI::DrawBigPoint( Graphics *pGr, PointF pt, int size )
{
	pGr->DrawEllipse( &Pen(Color::Red, 2), pt.X, pt.Y, (REAL)size, (REAL)size );
}

int CD2ViewUI::GetCurrentIndex()
{
	return (2+_nOffsetIcon)%GetObjectCount();
}

CStringW CD2ViewUI::GetTextByIndex( int i )
{
	if( i < 0 || i >= (int)_asText.size() )
		return L"";

	return _asText[i];
}

int CD2ViewUI::GetObjectCount() const
{
	return (int)_asImageFile.size();
}

int CD2ViewUI::HitTest( int x, int y )
{
	for( int i = 0; i < 5; i++ )
	{
		ImageState &s = _iss[i];

		CDUIRect	rt( int(s._x - s._w/2), int(s._y - s._h/2), 
			int(s._x + s._w/2), 
			int(s._y + s._h/2) );

		if( PtInRect( &rt, CDUIPoint(x, y) ) )
			return i;
	}

	return -1;
}

void CD2ViewUI::onPan( int dx, int dy )
{
	if( _bPanning )
		return;

	_bPanning = true;

	if( abs(dx) > 100 )
		AniStart( dx < 0 );
}


void CD2ViewUI::onGestureEnd()
{
	_bPanning = false;
}


void CD2ViewUI::InitAllImage()
{
	SetIconSubFolder( L"icons2/" );
	ReloadBKimage();

	AddIconAndText( L"会议讨论.png", L"会议讨论" );
	AddIconAndText( L"议题查询.png", L"议题查询" );
	AddIconAndText( L"议题批示.png", L"议题批示" );
	AddIconAndText( L"议题统计.png", L"议题统计" );
	AddIconAndText( L"笔录校对.png", L"笔录校对" );
}

void CD2ViewUI::d2DrawItem( CStringW strName, CStringW strText, ImageState *s )
{
	// 面板
	_de.d2DrawBitmap( _spIconBK2, s->_x-s->_w/2, s->_y-s->_h/2, s->_w, s->_h, 0.5f );

	float w, h;
	_de.d2GetSize( w, h );

	_d2sb->SetColor( D2D1::ColorF(RGB(51,51,51)) );
	if( abs( s->_x - w/2 ) < 10 )
		_d2sb->SetColor( D2D1::ColorF(RGB(255,255,255)) );

	float tx = s->_x-50.f;
	float ty = s->_y+50.f;
	//g_render_target->DrawText( strText, strText.GetLength(),  
	//	g_text_format, D2D1::RectF( tx, ty, tx+120.f, ty+30.f),  
	//	bWhite ? g_brush2 : g_brush );

	_de.d2DrawText( strText, tx, ty, 120.f, 30.f, _d2Font, _d2sb );

	auto it = _mapImage2.find( strName );
	if( it == _mapImage2.end() )
		return;

	ID2D1Bitmap *icon = it->second;

	D2D1_SIZE_U	sz = icon->GetPixelSize();
	float iw = (float)sz.width;
	float ih = (float)sz.height;
	_de.d2DrawBitmap( icon, s->_x-iw/2, s->_y-ih/2-20.f, iw, ih, s->_a );
}

void CD2ViewUI::BuildAllBitmap()
{
	if( _spBK2 != nullptr )
		return;

	_spBK2 = _de.d2NewBitmap( m_strFileBK );
	_spIconBK2 = _de.d2NewBitmap( m_strFileIconBK );

	for( auto it = _mapName2File.begin(); it != _mapName2File.end(); ++it )
	{
		d2Bitmap *bmp = _de.d2NewBitmap(it->second);
		_mapImage2.insert( make_pair( it->first, bmp ) );
	}

	_d2Font = _de.d2NewFont( L"黑体", 24 );
	_d2sb = _de.d2NewSolidBrush( RGB(255,255,255) );
}


