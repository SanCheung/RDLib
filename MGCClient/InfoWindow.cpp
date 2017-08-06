#include "StdAfx.h"
#include "InfoWindow.h"


CInfoWindow*	CInfoWindow::s_instance = nullptr;

CInfoWindow::CInfoWindow(void)
	: _hHostWnd( nullptr )
{
}


CInfoWindow::~CInfoWindow(void)
{
}

LRESULT CInfoWindow::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			//MessageBox( m_hWnd, L"WM_LBUTTONDOWN", 0, 0 );
			//CInfoWindow::Hide();
			PostQuitMessage(0);
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
		}
		break;

	case WM_PAINT:
		{
		}
		break;
	}

	return CWindowWnd::HandleMessage( uMsg, wParam, lParam );
}



void CInfoWindow::Show( HWND hParentWnd )
{
	if( s_instance == nullptr )
	{
		s_instance = new CInfoWindow;
		s_instance->CreateThis( hParentWnd );
	}
	else
	{
		s_instance->ShowWindow();
	}
}

bool CInfoWindow::Hide()
{
	if( s_instance != nullptr && 
		IsWindow( s_instance->GetHWND() ) )
	{
		//if( IDNO == MessageBox( ::GetDesktopWindow(), L"È·¶¨ÍË³ö£¿", L"", MB_YESNO|MB_ICONQUESTION ) )
		//	return false;

		s_instance->ShowWindow( false );
		return true;
	}

	return false;
}

void CInfoWindow::Release()
{
	DestroyWindow( s_instance->m_hWnd );
	delete s_instance;
	s_instance = nullptr;
}

HWND CInfoWindow::CreateThis( HWND hHostWnd )
{
	Create( hHostWnd, NULL, WS_POPUP|WS_VISIBLE, WS_EX_TOPMOST, 0, 0, 640, 480 );
	_hHostWnd = hHostWnd;

	::ShowWindow( m_hWnd, SW_SHOW);
	UpdateWindow( m_hWnd );

	return m_hWnd;
}