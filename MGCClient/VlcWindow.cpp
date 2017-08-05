#include "StdAfx.h"
#include "VlcWindow.h"

// libvlc.lib;libvlccore.lib;
#pragma comment( lib, "libvlc.lib" )
#pragma comment( lib, "libvlccore.lib" )

CVlcWindow*	CVlcWindow::s_instance = nullptr;

CVlcWindow::CVlcWindow(void)
	: _hHostWnd( nullptr )
{
	libvlc_inst = libvlc_new (0, NULL);
}


CVlcWindow::~CVlcWindow(void)
{
	libvlc_release (libvlc_inst);
}

LRESULT CVlcWindow::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			MessageBox( m_hWnd, L"WM_LBUTTONDOWN", 0, 0 );
		}
		break;
	case WM_LBUTTONUP:
		{
		}
		break;
	case WM_MOUSEMOVE:
		{
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
		}
		break;

	case WM_MOUSEHOVER:
		{

		}
		break;

	case WM_MOUSELEAVE:
		{

		}
		break;

	case WM_CREATE:
		{
			// Create a new item
			//m = libvlc_media_new_location (libvlc_inst, "http://mycool.movie.com/test.mov");

			libvlc_m = libvlc_media_new_path (libvlc_inst, "F:\\Clip_1080_5sec_VC1_15mbps.wmv" );

			// Create a media player playing environement
			libvlc_mp = libvlc_media_player_new_from_media (libvlc_m);

			// No need to keep the media now
			libvlc_media_release (libvlc_m);

			//on windows
			libvlc_media_player_set_hwnd (libvlc_mp, m_hWnd );

			// play the media_player
			int x=libvlc_media_player_play (libvlc_mp);
		}
		break;

	case WM_PAINT:
		{
			// 停止绘制
			HWND	hWnd = GetHWND();
			PAINTSTRUCT ps = { 0 };
			HDC hdc = ::BeginPaint( hWnd, &ps);
			LineTo( hdc, 800, 600 );
			::EndPaint(hWnd, &ps);
		}
		break;
	}

	return CWindowWnd::HandleMessage( uMsg, wParam, lParam );
}



void CVlcWindow::Show( HWND hParentWnd )
{
	if( s_instance == nullptr )
	{
		s_instance = new CVlcWindow;
		s_instance->CreateThis( hParentWnd );
	}
	else
	{
		s_instance->ShowWindow();
	}
}

bool CVlcWindow::Hide()
{
	if( s_instance != nullptr && 
		IsWindow( s_instance->GetHWND() ) )
	{
		if( IDNO == MessageBox( ::GetDesktopWindow(), L"确定退出？", L"", MB_YESNO|MB_ICONQUESTION ) )
			return false;

		s_instance->ShowWindow( false );
		return true;
	}

	return false;
}

void CVlcWindow::Release()
{
	DestroyWindow( s_instance->m_hWnd );
	delete s_instance;
	s_instance = nullptr;
}

HWND CVlcWindow::CreateThis( HWND hHostWnd )
{
	Create( NULL, NULL, WS_POPUP|WS_BORDER|WS_VISIBLE, WS_EX_TOPMOST, 0, 0, 800, 600 );
	_hHostWnd = hHostWnd;

	::ShowWindow( m_hWnd, SW_SHOW);
	UpdateWindow( m_hWnd );

	return m_hWnd;
}
