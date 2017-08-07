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
			//		//MessageBox( m_hWnd, L"", L"", MB_OK );
			//		PostQuitMessage( WM_QUIT );
			//	}
			//}
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
	CStringW	strFile = CAppData::GetInstancePath() + L"mgc/mgc_tip.png";

	Image	 image( strFile );
	int iWidth = image.GetWidth();
	int iHeight = image.GetHeight();

	HDC hdcScreen = GetDC(NULL);
	HDC hdcMem = CreateCompatibleDC(hdcScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, iWidth, iHeight);
	HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

	Graphics gr(hdcMem);
	gr.SetSmoothingMode( SmoothingModeHighQuality );
	gr.DrawImage(&image, 0, 0);//将png图像绘制到后台DC中

	//Pen		penLine( Color(0xFF999999), 1 );	
	//gr.DrawLine( &penLine, 30, 120, 290, 120 );
	//gr.DrawLine( &penLine, 30, 166, 290, 166 );

	//SolidBrush	sbLabel( Color(0xFF999999) );
	//SolidBrush	sbInfo( Color(0xFF222222) );

	//Font			ft( L"微软雅黑", 14 );
	//StringFormat	sf;
	////sf.SetAlignment( StringAlignmentCenter );

	//gr.DrawString( L"已玩", 2, &ft, PointF(30,84), &sf, &sbLabel );
	//gr.DrawString( L"计费", 2, &ft, PointF(30,84+46), &sf, &sbLabel );
	//gr.DrawString( L"需支付", 3, &ft, PointF(30,84+46+46), &sf, &sbLabel );

	//StringFormat	sf2;
	//sf2.SetAlignment( StringAlignmentFar );
	//gr.DrawString( L"1小时23分", 6, &ft, PointF(290,84), &sf2, &sbInfo );
	//gr.DrawString( L"1元/10分钟", 7, &ft, PointF(290,84+46), &sf2, &sbInfo );
	//gr.DrawString( L"55.6元", 6, &ft, PointF(290,84+46+46), &sf2, &sbInfo );

	//SolidBrush	sbButton( Color(0xFF222222) );
	//if( 2 == nType )
	//	sbButton.SetColor( Color(0xCF222222) );

	//GraphicsPath	gp;
	//AddRoundRect( gp, 50, 240, 220, 50, 24, 24 );
	//gr.FillPath( &sbButton, &gp );

	//Font			ft2( L"微软雅黑", 16 );
	//StringFormat	sf3;
	//sf3.SetAlignment( StringAlignmentCenter );


	//gr.DrawString( L"结束体验 >", 6, &ft2, RectF(50, 250, 220, 50), &sf3, &SolidBrush(Color::White) );


	BLENDFUNCTION blend = { 0 };
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;//按通道混合

	CDUIPoint	_ptNow( 0, 0 );

	POINT	pSrc = { 0, 0 };
	SIZE	sizeWnd = { iWidth, iHeight };
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
