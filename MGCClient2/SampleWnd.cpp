#include "StdAfx.h"
#include "SampleWnd.h"


CSampleWnd::CSampleWnd(void)
{
}


CSampleWnd::~CSampleWnd(void)
{
}

LRESULT CSampleWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( WM_PAINT == uMsg )
	{
		PAINTSTRUCT ps = { 0 };
		HDC hdc = ::BeginPaint( m_hWnd, &ps);
		CDUIRect rt;
		GetClientRect( m_hWnd, rt );
		Graphics g(hdc);
		g.FillRectangle( &SolidBrush(Color::Gray), 0, 0, rt.Width(), rt.Height() );
		
		StringFormat	sf;
		sf.SetAlignment( StringAlignmentCenter );
		sf.SetLineAlignment( StringAlignmentCenter );
		g.DrawString( L"ÊÓÆµ²¥·Å´°¿Ú", 6, &Font(L"Microsoft Yahei", 12), RectF(0,0,rt.Width(),rt.Height()),
			&sf, &SolidBrush(Color::Black) );
		::EndPaint(m_hWnd, &ps);
	}
	else if( WM_MOUSEMOVE == uMsg )
	{
		CDUIPoint	pt( LOWORD(lParam), HIWORD(lParam) );
		ClientToScreen( m_hWnd, &pt );
		
		HWND	hParentWnd = GetParent(m_hWnd);
		ScreenToClient( hParentWnd, &pt );

		//TRACE( L"Sample mousemove: %d %d\n", pt.x, pt.y );
		::SendMessage( hParentWnd, WM_MOUSEMOVE, wParam, MAKELPARAM(pt.x, pt.y ) );
	}

	return CWindowWnd::HandleMessage( uMsg, wParam, lParam );
}
