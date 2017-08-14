#include "StdAfx.h"
#include "WindowTip.h"


CWindowTip*	CWindowTip::s_instance = nullptr;

#define		PNG_WIDTH		400
#define		PNG_HEIGHT		50


CWindowTip::CWindowTip(void)
	: _hHostWnd( nullptr )
{
}


CWindowTip::~CWindowTip(void)
{
}

LRESULT CWindowTip::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	static BOOL		ldown;
	static POINT	TheFirstPoint;

	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			//ldown = TRUE;
			//SetCapture( m_hWnd );
			//TheFirstPoint.x = LOWORD(lParam);
			//TheFirstPoint.y = HIWORD(lParam);
			//Update();

			//m_bPosChanged = false;
		}
		break;
	case WM_LBUTTONUP:
		{
			//ldown = FALSE;
			////Update( 2 );
			//ReleaseCapture();

			////int	dx, dy;
			////dx = LOWORD(lParam) - TheFirstPoint.x;
			////dy = HIWORD(lParam) - TheFirstPoint.y;
			//if( !m_bPosChanged )
			//{
			//	::SendMessage( _hHostWnd, WM_EXITTPWND_CLICK, 0, 0 );

			//	RECT rt = {50, 250, 50+220, 250+50};
			//	if( PtInRect( &rt, TheFirstPoint ) )
			//	{
			//		MessageBox( m_hWnd, L"", L"", MB_OK );
			//		//PostQuitMessage( WM_QUIT );
			//	}
			//}
			//MessageBox( m_hWnd, L"", L"", MB_OK );
		}
		break;
	case WM_MOUSEMOVE:
		{
			//CDUIPoint	pt;
			//pt.x = LOWORD(lParam);
			//pt.y = HIWORD(lParam);

			//RECT rt = {50, 250, 50+220, 250+50};
			//if( PtInRect( &rt, pt ) )
			//{
			//	Update( 2 );
			//}
			//else
			//	Update( 1 );

			// 			if( !m_bMouseTracking )
			// 			{
			// 				TRACKMOUSEEVENT tme = { 0 };
			// 				tme.cbSize = sizeof(TRACKMOUSEEVENT);
			// 				tme.dwFlags = TME_HOVER | TME_LEAVE;
			// 				tme.hwndTrack = m_hWnd;
			// 				tme.dwHoverTime = 200;
			// 				_TrackMouseEvent(&tme);
			// 				m_bMouseTracking = true;
			// 			}
			// 
			// 			if (ldown)
			// 			{
			// 				POINT	pt;
			// 				GetCursorPos(&pt);
			// 				pt.x -= TheFirstPoint.x;
			// 				pt.y -= TheFirstPoint.y;
			// 
			// 				_ptNow.x = pt.x;
			// 				_ptNow.y = pt.y;
			// 
			// 				SetWindowPos( m_hWnd, NULL, pt.x, pt.y, NULL, NULL, SWP_NOREDRAW | SWP_NOSIZE | SWP_NOZORDER);
			// 				m_bPosChanged = true;
			// 			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			//DestroyWindow( m_hWnd );
		}
		break;

	case WM_MOUSEHOVER:
		{
			//m_bMouseTracking = false;
			//Update( 1 );
		}
		break;

	case WM_MOUSELEAVE:
		{
			//if( m_bMouseTracking ) ::SendMessage(m_hWnd, WM_MOUSEMOVE, 0, (LPARAM) -1);
			//m_bMouseTracking = false;
			//Update( 0 );
		}
		break;

	case WM_CREATE:
		{
			Update();
		}
		break;
	}

	return CWindowWnd::HandleMessage( uMsg, wParam, lParam );
}

HWND CWindowTip::CreateThis( HWND hHostWnd )
{
	//CreateWindowEx(WS_EX_LAYERED, TEXT("AppTest"), GetWindowClassName(), WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP/*无边框风格*/
	//	, 0, 0, 114, 52,
	//	NULL, NULL, hInstance, NULL);

	Create( NULL, NULL, WS_POPUP, WS_EX_LAYERED|WS_EX_TOPMOST|WS_EX_TOOLWINDOW, 0, 0, PNG_WIDTH, PNG_HEIGHT );
	_hHostWnd = hHostWnd;

	//RegisterTouchWindow(_hHostWnd, 0);  

	::ShowWindow( m_hWnd, SW_SHOW);
	UpdateWindow( m_hWnd );


	return m_hWnd;
}



void CWindowTip::Update()
{
	CStringW	strFile = CAppData::GetInstancePath() + L"mgc/mgc_pc_dianji.png";
	Image	 image( strFile );
	int iWidth = image.GetWidth();
	int iHeight = image.GetHeight();


	int w = 240;
	int h = 44;


	HDC hdcScreen = GetDC(NULL);
	HDC hdcMem = CreateCompatibleDC(hdcScreen);
	//HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, iWidth, iHeight);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, w, h);
	HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

	Graphics gr(hdcMem);
	gr.SetSmoothingMode( SmoothingModeHighQuality );
	gr.SetTextRenderingHint( TextRenderingHintAntiAlias );
	gr.DrawImage(&image, 0, 0);//将png图像绘制到后台DC中

	Font		ft( L"Microsoft Yahei", 16.f, FontStyleBold );

	CStringW	str = L"点击任意位置开始";
	gr.DrawString( str, str.GetLength(), &ft, PointF(32, 0), &SolidBrush(Color(128,255,255,255)));

	BLENDFUNCTION blend = { 0 };
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;//按通道混合

	int	sw = GetSystemMetrics( SM_CXSCREEN );
	int sh = GetSystemMetrics( SM_CYSCREEN );

	CDUIPoint	_ptNow( (sw-w)/2, sh-h-10 );

	POINT	pSrc = { 0, 0 };
	SIZE	sizeWnd = { w, h };
	UpdateLayeredWindow( m_hWnd, hdcScreen, &_ptNow, &sizeWnd, hdcMem, &pSrc, NULL, &blend, ULW_ALPHA);//更新分层窗口
	//收尾清理工作
	SelectObject(hdcMem, hBitmapOld);
	DeleteObject(hBitmap);
	DeleteDC(hdcMem);

	ReleaseDC(m_hWnd, hdcScreen);
}

void CWindowTip::Show( HWND hParentWnd )
{
	if( s_instance == nullptr )
	{
		s_instance = new CWindowTip;
		s_instance->CreateThis( hParentWnd );
	}
	else
	{
		s_instance->ShowWindow();
	}
}

bool CWindowTip::Hide()
{
	if( s_instance != nullptr && 
		IsWindow( s_instance->GetHWND() ) )
	{
		////if( DUI_CANCEL == DuiMessageBox(::GetDesktopWindow(), L"确定终止同屏演示?",L"温馨提示",DUI_YESNO ) )
		////	return false;
		//if( IDNO == MessageBox( ::GetDesktopWindow(), L"确定退出？", L"", MB_YESNO|MB_ICONQUESTION ) )
		//	return false;

		s_instance->ShowWindow( false );
		return true;
	}

	return false;
}

void CWindowTip::Release()
{
	DestroyWindow( s_instance->m_hWnd );
	delete s_instance;
	s_instance = nullptr;
}
