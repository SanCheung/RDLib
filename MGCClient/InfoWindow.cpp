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
	LRESULT	nRet = 0;
	BOOL handled = FALSE;

	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			//PostQuitMessage(0);
			Hide();
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
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(*this, &ps);

			Graphics		gr( hDC );

			CStringW		strFile = CAppData::GetInstancePath() + L"mgc/2wm.png";

			CDUIRect		rt;
			GetClientRect( m_hWnd, rt );

			Image			img( strFile );
			gr.DrawImage( &img, Rect(0,0,rt.Width(),rt.Height()), 0, 0, img.GetWidth(), img.GetHeight(), UnitPixel );

			EndPaint(*this, &ps);
			handled = TRUE;
		}
		break;
	}

	if( handled )
		return nRet;

	return CWindowWnd::HandleMessage( uMsg, wParam, lParam );
}

bool CInfoWindow::IsShow()
{
	if( s_instance == nullptr )
		return false;

	HWND	hWnd = s_instance->GetHWND();
	if( hWnd == nullptr || !::IsWindow(hWnd) )
		return false;

	return ::IsWindowVisible( hWnd );
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
	Create( hHostWnd, NULL, WS_POPUP|WS_BORDER, 0, 100, 100, 640, 480 );
	_hHostWnd = hHostWnd;

	::ShowWindow( m_hWnd, SW_SHOW);
	UpdateWindow( m_hWnd );

	return m_hWnd;
}