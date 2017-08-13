#include "StdAfx.h"
#include "InfoWindow.h"
#include "MainHelper.h"

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
	case WM_TIMER:
		{
			 if( CMainHelper::TestTimeOut( 5000 ) )
			 {
				 Hide();
				 ::PostMessage( _hHostWnd, WM_SHOWA, 1, 0 );
				 break;
			 }

			 _time = ::GetTickCount() - IdleTrackerGetLastTickCount();
			 ::InvalidateRect(m_hWnd, NULL, FALSE);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			Hide();
			::PostMessage( _hHostWnd, WM_SHOWA, 3, 0 );
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
			//::SetTimer( m_hWnd, 1, 1000, NULL );
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


			Font		ft( L"Microsoft Yahei", 32.f );

			static wchar_t	 buf[32] = {0};
			wsprintf( buf, L"%d", _time );
			gr.DrawString( buf, wcslen(buf), &ft, PointF(100, 100), &SolidBrush(Color::Red));

			Font		ft2( L"Microsoft Yahei", 12.f );
			CStringW	str = L"请点击任意位置模拟扫描二维码的动作，测试5秒没动作就回到视频界面";
			gr.DrawString( str, str.GetLength(), &ft2, PointF(100, 0), &SolidBrush(Color::White));


			EndPaint(*this, &ps);
			handled = TRUE;
		}
		break;
	case WM_SHOWWINDOW:
		{
			bool bShow = (wParam==TRUE);
			CMainHelper::ShowTrayWndAndStartWnd( !bShow );
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
		::ShowWindow( s_instance->m_hWnd, SW_SHOW);
	}

	::SetTimer( s_instance->m_hWnd, 1, 1000, NULL );
}

bool CInfoWindow::Hide()
{
	if( s_instance != nullptr && 
		IsWindow( s_instance->GetHWND() ) )
	{
		::KillTimer( s_instance->GetHWND(), 1 );
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
	//Create( hHostWnd, NULL, WS_POPUP|WS_BORDER, 0, 0, 0, 400, 300 );
	Create( hHostWnd, NULL, WS_POPUP, 0, 0, 0, 400, 300 );
	_hHostWnd = hHostWnd;

	::ShowWindow( m_hWnd, SW_SHOWMAXIMIZED);
	UpdateWindow( m_hWnd );

	return m_hWnd;
}