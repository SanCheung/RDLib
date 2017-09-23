#include "StdAfx.h"
#include "D2HostableWnd.h"


CD2HostableWnd::CD2HostableWnd(void)
	: _animing( false )
{
}


CD2HostableWnd::~CD2HostableWnd(void)
{
}

void CD2HostableWnd::dh_AniStart()
{

}

void CD2HostableWnd::dh_AniProc()
{

}

void CD2HostableWnd::dh_AniEnd()
{

}

LRESULT CD2HostableWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = 0;
	BOOL handled = FALSE;
	if(uMsg == WM_PAINT)
	{
		_de.d2Draw( this );
		if( _animing )
		{
			dh_AniProc();
		}
		else
		{
			// Í£Ö¹»æÖÆ
			HWND	hWnd = GetHWND();
			PAINTSTRUCT ps = { 0 };
			HDC hdc = ::BeginPaint( hWnd, &ps);
			::EndPaint(hWnd, &ps);
		}

		handled = TRUE;
	}
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
			dh_BuildAllBitmap();
		}
	}


	if (!handled)
		lResult = CHostableWnd::HandleMessage(uMsg, wParam, lParam);

	return lResult;
}
