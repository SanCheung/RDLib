#include "StdAfx.h"
#include "ChargeWnd.h"
#include "MainHelper.h"

#include "DlgConfirm.h"
#include "DlgPayment.h"

#include "MainHelper.h"
#include "SettingMgr.h"


#define		PNG_WIDTH		320
#define		PNG_HEIGHT		304

CChargeWnd*	CChargeWnd::s_instance = nullptr;

CChargeWnd::CChargeWnd(void)
	: m_bMouseTracking( false )
	, m_bPosChanged( false )
	, _hHostWnd( nullptr )
{
	int	sw = GetSystemMetrics( SM_CXSCREEN );
	int sh = GetSystemMetrics( SM_CYSCREEN );

	_ptNow.SetPoint( sw-PNG_WIDTH-10, sh-PNG_HEIGHT-50 );
	_nType = 0;
}


CChargeWnd::~CChargeWnd(void)
{
}

LRESULT CChargeWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	static BOOL		ldown;
	static POINT	TheFirstPoint;

	switch (uMsg)
	{
	case WM_TIMER:
		Update();
		break;

	case WM_LBUTTONDOWN:
		{
			ldown = TRUE;
			SetCapture( m_hWnd );
			TheFirstPoint.x = LOWORD(lParam);
			TheFirstPoint.y = HIWORD(lParam);

			_nType = 1;
			Update();
			//Update( 1 );

			m_bPosChanged = false;
		}
		break;
	case WM_LBUTTONUP:
		{
			ldown = FALSE;
			//Update( 2 );
			ReleaseCapture();

			//int	dx, dy;
			//dx = LOWORD(lParam) - TheFirstPoint.x;
			//dy = HIWORD(lParam) - TheFirstPoint.y;
			if( !m_bPosChanged )
			{
				::SendMessage( _hHostWnd, WM_EXITTPWND_CLICK, 0, 0 );

				RECT rt = {50, 226, 50+220, 226+50};
				if( PtInRect( &rt, TheFirstPoint ) )
				{
					////MessageBox( m_hWnd, L"", L"", MB_OK );
					////PostQuitMessage( WM_QUIT );
					//Hide();

					CDlgConfirm		dlg;
					if( IDOK == dlg.DoModalNoCaption( GetDesktopWindow() ) )
					{
						bool	bOK = CMainHelper::web_offline();
						if( bOK )
						{
							Hide();
							::PostMessage( _hHostWnd, WM_SHOWA, 5, 0 );
						}

					}
				}
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			CDUIPoint	pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			RECT rt = {50, 226, 50+220, 226+50};
			if( PtInRect( &rt, pt ) )
				_nType = 2;
				//Update( 2 );
			else
				_nType = 1;
				//Update( 1 );

			Update();

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
			m_bMouseTracking = false;
			//Update( 1 );
			_nType = 1;
			Update();
		}
		break;

	case WM_MOUSELEAVE:
		{
			if( m_bMouseTracking ) ::SendMessage(m_hWnd, WM_MOUSEMOVE, 0, (LPARAM) -1);
			m_bMouseTracking = false;
			//Update( 0 );
			_nType = 0;
			Update();
		}
		break;

	case WM_CREATE:
		{
			//thStart();
			_nType = 0;
			Update();
			SetTimer( m_hWnd, 1, 100, NULL );

			////添加阴影
			//CWndShadow::Initialize(CAppData::GetInstance());
			//m_shadow.Create(m_hWnd);
			//m_shadow.SetSize(2);
			//m_shadow.SetPosition(1, 1);
		}
		break;
	}

	return CWindowWnd::HandleMessage( uMsg, wParam, lParam );
}

HWND CChargeWnd::CreateThis( HWND hHostWnd )
{
	//CreateWindowEx(WS_EX_LAYERED, TEXT("AppTest"), GetWindowClassName(), WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP/*无边框风格*/
	//	, 0, 0, 114, 52,
	//	NULL, NULL, hInstance, NULL);

	Create( NULL, NULL, WS_POPUP, WS_EX_LAYERED|WS_EX_TOOLWINDOW, 0, 0, PNG_WIDTH, PNG_HEIGHT );
	_hHostWnd = hHostWnd;

	 //RegisterTouchWindow(_hHostWnd, 0);  

	::ShowWindow( m_hWnd, SW_SHOW);
	UpdateWindow( m_hWnd );


	return m_hWnd;
}

void CChargeWnd::Update()
{
	//CStringW	strFile = CAppData::GetInstancePath() + L"tp/exittp_normal.png";
	//if( 1 == nType )
	//	strFile = CAppData::GetInstancePath() + L"tp/exittp_hover.png";
	//else if( 2 == nType )
	//	strFile = CAppData::GetInstancePath() + L"tp/exittp_pushed.png";
	CStringW	strFile = CAppData::GetInstancePath() + L"mgc/mgc_bg.png";

	Image	 image( strFile );
	int iWidth = image.GetWidth();
	int iHeight = image.GetHeight();

	HDC hdcScreen = GetDC(NULL);
	HDC hdcMem = CreateCompatibleDC(hdcScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, iWidth, iHeight);
	HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

	Graphics gr(hdcMem);
	gr.SetSmoothingMode( SmoothingModeHighQuality );
	gr.SetTextRenderingHint( TextRenderingHintAntiAlias );
	gr.DrawImage(&image, 0, 0);//将png图像绘制到后台DC中

	Pen		penLine( Color(0xFF999999), 1 );	
	gr.DrawLine( &penLine, 30, 120, 290, 120 );
	gr.DrawLine( &penLine, 30, 166, 290, 166 );

	SolidBrush	sbLabel( Color(0xFF999999) );
	SolidBrush	sbInfo( Color(0xFF222222) );

	Font			ft( L"微软雅黑", 14 );
	StringFormat	sf;
	//sf.SetAlignment( StringAlignmentCenter );

	gr.DrawString( L"已玩", 2, &ft, PointF(30,84), &sf, &sbLabel );
	gr.DrawString( L"计费", 2, &ft, PointF(30,84+46), &sf, &sbLabel );
	gr.DrawString( L"需支付", 3, &ft, PointF(30,84+46+46), &sf, &sbLabel );

	StringFormat	sf2;
	sf2.SetAlignment( StringAlignmentFar );


	CSettingMgr		*s = SetMgr();

	CStringW		str;
	int		minute = s->_duration.ToInt();
	if( minute >= 60 )
	{
		if( minute%60 == 0 )
			str.Format( L"%d小时", minute/60 );
		else
			str.Format( L"%d小时%d分钟", minute/60, minute%60 );
	}
	else
		str.Format( L"%d分钟", minute );
	gr.DrawString( str, str.GetLength(), &ft, PointF(290,84), &sf2, &sbInfo );

	str.Format( L"%s元/分钟", s->_charging );
	gr.DrawString( str, str.GetLength(), &ft, PointF(290,84+46), &sf2, &sbInfo );

	str.Format( L"%s元", s->_cost );
	gr.DrawString( str, str.GetLength(), &ft, PointF(290,84+46+46), &sf2, &sbInfo );

	SolidBrush	sbButton( Color(0xFF222222) );
	if( 2 == _nType )
		sbButton.SetColor( Color(0xCF222222) );

	GraphicsPath	gp;
	CMainHelper::AddRoundRect( gp, 50, 226, 220, 50, 24, 24 );
	gr.FillPath( &sbButton, &gp );

	Font			ft2( L"微软雅黑", 16 );
	StringFormat	sf3;
	sf3.SetAlignment( StringAlignmentCenter );


	gr.DrawString( L"结束体验 >", 6, &ft2, RectF(50, 236, 220, 50), &sf3, &SolidBrush(Color::White) );


	BLENDFUNCTION blend = { 0 };
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;//按通道混合

	POINT	pSrc = { 0, 0 };
	SIZE	sizeWnd = { iWidth, iHeight };
	UpdateLayeredWindow( m_hWnd, hdcScreen, &_ptNow, &sizeWnd, hdcMem, &pSrc, NULL, &blend, ULW_ALPHA);//更新分层窗口
	//收尾清理工作
	SelectObject(hdcMem, hBitmapOld);
	DeleteObject(hBitmap);
	DeleteDC(hdcMem);

	ReleaseDC(m_hWnd, hdcScreen);
}

void CChargeWnd::Show( HWND hParentWnd )
{
	if( s_instance == nullptr )
	{
		s_instance = new CChargeWnd;
		s_instance->CreateThis( hParentWnd );
	}
	else
	{
		s_instance->ShowWindow();
		s_instance->_nType = 0;
		s_instance->Update();
	}
}

bool CChargeWnd::Hide()
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

void CChargeWnd::Release()
{
	DestroyWindow( s_instance->m_hWnd );
	delete s_instance;
	s_instance = nullptr;
}

void CChargeWnd::thread_main()
{
	//maps2s			m;
	//maps2s_shell	ms( &m );
	//CSettingMgr		*s = SetMgr();
	//while(1)
	//{
	//	int nRet = CMainHelper::webStatus_client( m );
	//	if( nRet > 0 )
	//	{
	//		s->_duration = ms.stringValue( "duration" ).c_str();
	//		s->_charging = ms.stringValue( "charging" ).c_str();
	//		s->_cost = ms.stringValue( "cost" ).c_str();
	//		Update(0);


	//		int		onlineStatus = 	ms.intValue("onlineStatus");
	//		if( onlineStatus != 2 )
	//		{
	//			MessageBox( m_hWnd, L"非上机中!!!\n正式版，这种情况下会重启计算机！", L"", 0 );
	//			//Hide();
	//			break;
	//		}
	//	}

	//	if( WAIT_OBJECT_0 == thWaitEvent(10000) )
	//		break;
	//}
}
