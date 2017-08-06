#include "StdAfx.h"
#include "VlcWindow.h"

#include "WindowTip.h"

// libvlc.lib;libvlccore.lib;
#pragma comment( lib, "libvlc.lib" )
#pragma comment( lib, "libvlccore.lib" )

CVlcWindow*	CVlcWindow::s_instance = nullptr;

CVlcWindow::CVlcWindow(void)
	: _hHostWnd( nullptr )
{
	libvlc_inst = libvlc_new (0, NULL);

	libvlc_mp = nullptr;
}


CVlcWindow::~CVlcWindow(void)
{
	libvlc_release (libvlc_inst);
	vlcReleasePlayer();
}

LRESULT CVlcWindow::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			MessageBox( m_hWnd, L"CVlcWindow::WM_LBUTTONDOWN", 0, 0 );
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
			//// Create a new item
			////m = libvlc_media_new_location (libvlc_inst, "http://mycool.movie.com/test.mov");

			//libvlc_m = libvlc_media_new_path (libvlc_inst, "F:\\abc.wmv" );

			//// Create a media player playing environement
			//libvlc_mp = libvlc_media_player_new_from_media (libvlc_m);

			//// No need to keep the media now
			//libvlc_media_release (libvlc_m);

			////on windows
			//libvlc_media_player_set_hwnd (libvlc_mp, m_hWnd );

			//// play the media_player
			//int x=libvlc_media_player_play (libvlc_mp);


			//libvlc_event_manager_t *vlc_evt_man = libvlc_media_player_event_manager( libvlc_mp );
			//libvlc_event_attach(vlc_evt_man, libvlc_MediaPlayerEndReached, OnVLC_Event, this);


			Play();
		}
		break;

	case WM_PAINT:
		{
		}
		break;
	case WM_APP+100:
		{
			//Play();
			Replay();
		}
		break;
	case WM_SHOWWINDOW:
		{
			if( wParam )
			{
				Replay();

				CWindowTip::Show( m_hWnd );
			}
			else
			{
				libvlc_media_player_stop(libvlc_mp);

				CWindowTip::Hide();
			}

		}
		break;
	}

	return CWindowWnd::HandleMessage( uMsg, wParam, lParam );
}



bool CVlcWindow::IsShow()
{
	if( s_instance == nullptr )
		return false;

	HWND	hWnd = s_instance->GetHWND();
	if( hWnd == nullptr || !::IsWindow(hWnd) )
		return false;

	return ::IsWindowVisible( hWnd );
}



HWND CVlcWindow::Show( HWND hParentWnd )
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

	return s_instance->GetHWND();
}

bool CVlcWindow::Hide()
{
	if( s_instance != nullptr && 
		IsWindow( s_instance->GetHWND() ) )
	{
		//if( IDNO == MessageBox( ::GetDesktopWindow(), L"ȷ���˳���", L"", MB_YESNO|MB_ICONQUESTION ) )
		//	return false;

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
	Create( NULL, NULL, WS_POPUP|WS_BORDER|WS_VISIBLE, 0, 0, 0, 640, 480 );
	_hHostWnd = hHostWnd;

	::ShowWindow( m_hWnd, SW_SHOW);
	UpdateWindow( m_hWnd );

	return m_hWnd;
}

void CVlcWindow::OnVLC_Event( const libvlc_event_t *event, void *data )
{
	CVlcWindow *pThis = (CVlcWindow *)data;
	//pThis->Play();
	pThis->PostMessage( WM_APP+100 );
}

void CVlcWindow::Play()
{
	vlcReleasePlayer();

	// Create a new item
	//m = libvlc_media_new_location (libvlc_inst, "http://mycool.movie.com/test.mov");

	libvlc_media_t* libvlc_m = libvlc_media_new_path (libvlc_inst, "F:\\chicken.wmv" );

	if( libvlc_m )
	{
		vlcReleasePlayer();

		// Create a media player playing environement
		libvlc_mp = libvlc_media_player_new_from_media (libvlc_m);

		//on windows
		libvlc_media_player_set_hwnd (libvlc_mp, m_hWnd );

		// play the media_player
		int x=libvlc_media_player_play (libvlc_mp);


		libvlc_event_manager_t *vlc_evt_man = libvlc_media_player_event_manager( libvlc_mp );
		libvlc_event_attach(vlc_evt_man, libvlc_MediaPlayerEndReached, OnVLC_Event, this );

		// No need to keep the media now
		libvlc_media_release (libvlc_m);
	}
}

void CVlcWindow::vlcReleasePlayer()
{
	if( libvlc_mp )
	{
		libvlc_media_player_stop( libvlc_mp );
		libvlc_media_player_release( libvlc_mp );
		libvlc_mp = nullptr;
	}
}

void CVlcWindow::Replay()
{
	libvlc_media_player_stop(libvlc_mp);
	libvlc_media_player_set_position( libvlc_mp, 0.f );
	int r = libvlc_media_player_play (libvlc_mp);
}
