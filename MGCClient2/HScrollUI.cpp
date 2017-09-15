#include "StdAfx.h"
#include "HScrollUI.h"

CHScrollUI::CHScrollUI(void)
	: _x( 0 )
	, _bMouseDown( false )
	, _xDown( 0 )
	, _host( nullptr )
{
}


CHScrollUI::~CHScrollUI(void)
{
}

void CHScrollUI::DoPaint( HDC hDC, const RECT& rcPaint )
{
	if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) 
		return;

	CDUIRect		rt( m_rcItem );
	int		w = rt.GetWidth();
	int		h = rt.GetHeight();

	Bitmap		bmp( w, h );
	Graphics	g2( &bmp );

	g2.FillRectangle( &SolidBrush(Color::Gray), 0, 0, w, h );
	g2.FillRectangle( &SolidBrush(Color::Color(128,0,0,0)), _x, 0, 100, h );

	Graphics		g( hDC );
	g.DrawImage( &bmp, rt.left, rt.top );
}


void CHScrollUI::Event( TEventUI& event )
{
	CDUIRect		rt( m_rcItem );
	int				w = rt.GetWidth();
	int				h = rt.GetHeight();

	if( UIEVENT_BUTTONDOWN == event.Type )
	{
		int		xx = event.ptMouse.x - rt.left;
		int		yy = event.ptMouse.y - rt.top;

		if( xx > _x && xx < _x+100 )
		{
			_bMouseDown = true;
			_xDown = xx;

			SetCapture( GetManager()->GetPaintWindow() );
		}
	}
	else if( UIEVENT_MOUSEMOVE == event.Type )
	{
		if( _bMouseDown )
		{
			int		xx = event.ptMouse.x - rt.left;
			int		yy = event.ptMouse.y - rt.top;

			
			int		dx = xx-_xDown;
			_x += dx;
			
			_x = max( _x, 0 );
			_x = min( _x, w-100 );

			if( _xDown != xx )
			{
				_xDown = xx;
				Invalidate();

				PointF	ptF;
				ptF.X = _xDown/(rt.GetWidth()-100.f);
				ptF.Y = -1;
				//::SendMessage( GetManager()->GetPaintWindow(), WM_MAINDLG_SETRATIO, (WPARAM)&ptF, 0 );
				if( nullptr != _host )
					_host->OnSetRatio( ptF );
			}

		}
	}
	else if( UIEVENT_BUTTONUP == event.Type )
	{
		_bMouseDown = false;
		_xDown = 0;

		ReleaseCapture();
	}

	CControlUI::Event( event );
}

void CHScrollUI::SetRatio( float fRatio )
{
	fRatio = max( fRatio, 0 );
	fRatio = min( fRatio, 1.f );

	CDUIRect		rt( m_rcItem );
	int x = int( (rt.GetWidth()-100) * fRatio + 0.5f );
	if( x != _x )
	{
		_x = x;
		Invalidate();
	}
}





////////////////////////////////////////////////////////////////////////
CVScrollUI::CVScrollUI(void)
	: _y( 0 )
	, _bMouseDown( false )
	, _yDown( 0 )
	, _host( nullptr )
{
}


CVScrollUI::~CVScrollUI(void)
{
}

void CVScrollUI::DoPaint( HDC hDC, const RECT& rcPaint )
{
	if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) 
		return;

	CDUIRect		rt( m_rcItem );
	int		w = rt.GetWidth();
	int		h = rt.GetHeight();

	Bitmap		bmp( w, h, PixelFormat32bppARGB );
	Graphics	g2( &bmp );

	// draw line
	g2.FillRectangle( &SolidBrush(Color(127, 105,105,105)), (w-2)/2, 0, 2, h );
	g2.FillRectangle( &SolidBrush(Color(255,255,255)), (w-4)/2, _y, 4, 100 );
	Graphics		g( hDC );
	g.DrawImage( &bmp, rt.left, rt.top );
}

void CVScrollUI::Event( TEventUI& evt )
{
	CDUIRect		rt( m_rcItem );
	int				w = rt.GetWidth();
	int				h = rt.GetHeight();

	if( UIEVENT_SCROLLWHEEL == evt.Type)
	{
		float r = _y*1.f/(h-100);

		if( r >= 0 )
		{
			int nDir = LOWORD( evt.wParam );
			if( SB_LINEUP == nDir )
				r += .05f;
			else
				r -= .05f;

			r = CLAMP( r, 0, 1 );

			int newY = int(r*(h-100) + .5f);
			if( newY != _y )
			{
				_y = newY;
				Invalidate();

				PointF	ptF;
				ptF.X = -1;
				ptF.Y = r;
				if( nullptr != _host )
					_host->OnSetRatio( ptF );
			}

		}
	}
	else if( UIEVENT_BUTTONDOWN == evt.Type )
	{
		int		yy = evt.ptMouse.y - rt.top;

		if( yy > _y && yy < _y+100 )
		{
			_bMouseDown = true;
			_yDown = yy;
			SetCapture( GetManager()->GetPaintWindow() );
		}
	}
	else if( UIEVENT_MOUSEMOVE == evt.Type )
	{
		if( _bMouseDown )
		{
			int		yy = evt.ptMouse.y - rt.top;

			int		dy = yy - _yDown;
			_y += dy;

			_y = max( _y, 0 );
			_y = min( _y, h-100 );

			if( _yDown != yy )
			{
				_yDown = yy;
				Invalidate();

				PointF	ptF;
				ptF.X = -1;
				ptF.Y = _yDown*1.f/(h-100);

				ptF.Y = CLAMP( ptF.Y, 0, 1 );
				//::SendMessage( GetManager()->GetPaintWindow(), WM_MAINDLG_SETRATIO, (WPARAM)&ptF, 0 );
				if( nullptr != _host )
					_host->OnSetRatio( ptF );
			}

		}
	}
	else if( UIEVENT_BUTTONUP == evt.Type )
	{
		_bMouseDown = false;
		_yDown = 0;

		ReleaseCapture();
	}

	CControlUI::Event( evt );
}

void CVScrollUI::SetRatio( float fRatio )
{
	fRatio = max( fRatio, 0 );
	fRatio = min( fRatio, 1.f );

	CDUIRect		rt( m_rcItem );
	int v = int( (rt.GetHeight()-100) * fRatio + 0.5f );
	if( v != _y )
	{
		_y = v;
		Invalidate();
	}
}
