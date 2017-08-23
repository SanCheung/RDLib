#include "StdAfx.h"
#include "InfoWindow.h"
#include "MainHelper.h"
#include "SettingMgr.h"

CInfoWindow*	CInfoWindow::s_instance = nullptr;

CInfoWindow::CInfoWindow(void)
	: _hHostWnd( nullptr )
{
	_spImageBK.reset( new Image( CAppData::GetInstancePath() + L"mgc/infobk.png" ) );
	_spImage2w.reset( new Image( CAppData::GetInstancePath() + L"mgc/info2w.png" ) );
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
			if( CMainHelper::TestTimeOut( 20000 ) )
			{
				Hide();
				::PostMessage( _hHostWnd, WM_SHOWA, 1, 0 );
				break;
			}

			//_time = ::GetTickCount() - IdleTrackerGetLastTickCount();
			//CDUIRect	rt( 0, 0, 200, 30 );
			//::InvalidateRect(m_hWnd, rt, FALSE);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			//Hide();
			//::PostMessage( _hHostWnd, WM_SHOWA, 3, 0 );
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

			CDUIRect		rt;
			GetClientRect( m_hWnd, rt );

			int w = rt.GetWidth();
			int h = rt.GetHeight();


			gr.DrawImage( _spImageBK.get(), 
				Rect(0,0,w,h), 0, 0, 
				_spImageBK->GetWidth(), _spImageBK->GetHeight(), UnitPixel );


			int iw = _spImage2w->GetWidth();
			int ih = _spImage2w->GetHeight();
			int isw = 200;
			int ish = 200;

			gr.DrawImage( _spImage2w.get(), 
				Rect((w-isw)/2,(h-ish)/2,isw,ish), 0, 0, iw, ih, UnitPixel );

			//////// 测试提示
			//Font		ft( L"Microsoft Yahei", 20.f );

			//static wchar_t	 buf[32] = {0};
			//wsprintf( buf, L"%d", _time );
			//gr.DrawString( buf, wcslen(buf), &ft, PointF(0, 0), &SolidBrush(Color::Red));

			//Font		ft2( L"Microsoft Yahei", 12.f );
			//CStringW	str = L"请点击任意位置模拟扫描二维码的动作，测试10秒没动作就回到视频界面";
			//gr.DrawString( str, str.GetLength(), &ft2, PointF(20, h-20), &SolidBrush(Color::White));


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
	case WM_CLOSE:
		{
			CMainHelper::ShowTrayWndAndStartWnd( true );
			PostQuitMessage(0);
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
	mgTrace( L"CInfoWindow::Show" );

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
	mgTrace( L"CInfoWindow::Hide" );
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
	Create( hHostWnd, NULL, WS_POPUP, WS_EX_TOPMOST, 0, 0, 1, 1 );
	_hHostWnd = hHostWnd;

	::ShowWindow( m_hWnd, SW_SHOWMAXIMIZED);
	UpdateWindow( m_hWnd );

	return m_hWnd;
}

void CInfoWindow::thread_main()
{
	Sleep(2000);
	//maps2s			m;
	//maps2s_shell	ms( &m );
	//while(1)
	//{
	//	int nRet = CMainHelper::webStatus_client( m );
	//	if( nRet > 0 )
	//	{
	//		if( ms.intValue("onlineStatus") == 2 )
	//		{
	//			Hide();
	//			::PostMessage( _hHostWnd, WM_SHOWA, 3, 0 );
	//			break;
	//		}
	//	}

	//	if( WAIT_OBJECT_0 == thWaitEvent(1000) )
	//		break;
	//}
}
