#include "StdAfx.h"
#include "ChargeWnd.h"
#include "MainHelper.h"

#include "DlgConfirm.h"
#include "DlgPayment.h"

#include "MainHelper.h"
#include "SettingMgr.h"

#include "LayeredWindowDrawer.h"
#pragma comment( lib, "comctl32.lib")

#define		PNG_WIDTH		320
#define		PNG_HEIGHT1		304
#define		PNG_HEIGHT		166
#define		BUTTON_TOP1		226
#define		BUTTON_TOP		88

CChargeWnd*	CChargeWnd::s_instance = nullptr;

CChargeWnd::CChargeWnd(void)
	: m_bMouseTracking( false )
	, m_bPosChanged( false )
	, _hHostWnd( nullptr )
	, _bOfflineing( false )
{
	//int	sw = GetSystemMetrics( SM_CXSCREEN );
	//int sh = GetSystemMetrics( SM_CYSCREEN );

	//_ptNow.SetPoint( sw-PNG_WIDTH-10, sh-PNG_HEIGHT-50 );
	//_nType = 0;
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

				//RECT rt = {50, BUTTON_TOP, 50+220, BUTTON_TOP+50};
				RECT rt = {50, BUTTON_TOP1, 50+220, BUTTON_TOP1+50};
				if( PtInRect( &rt, TheFirstPoint ) && !_bOfflineing )
				{
					////MessageBox( m_hWnd, L"", L"", MB_OK );
					////PostQuitMessage( WM_QUIT );
					//Hide();


					_bOfflineing = true;

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

					_bOfflineing = false;
				}
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			CDUIPoint	pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			//RECT rt = {50, BUTTON_TOP, 50+220, BUTTON_TOP+50};
			RECT rt = {50, BUTTON_TOP1, 50+220, BUTTON_TOP1+50};
			if( PtInRect( &rt, pt ) )
				_nType = 2;
				//Update( 2 );
			else
				_nType = 1;
				//Update( 1 );

			Update();

			if( !m_bMouseTracking )
			{
				TRACKMOUSEEVENT tme = { 0 };
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_HOVER | TME_LEAVE;
				tme.hwndTrack = m_hWnd;
				tme.dwHoverTime = 200;
				_TrackMouseEvent(&tme);
				m_bMouseTracking = true;
			}
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
			_nType = 1;
			Update();
		}
		break;

	case WM_MOUSELEAVE:
		{
			if( m_bMouseTracking ) 
				::SendMessage(m_hWnd, WM_MOUSEMOVE, 0, (LPARAM) -1);

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

			////�����Ӱ
			//CWndShadow::Initialize(CAppData::GetInstance());
			//m_shadow.Create(m_hWnd);
			//m_shadow.SetSize(2);
			//m_shadow.SetPosition(1, 1);
		}
		break;
	case WM_CLOSE:
		{
			//::PostMessage( _hHostWnd, WM_SHOWA, 3, 0 );
			Show( _hHostWnd );
			return 1;
		}
		break;
	}

	return CWindowWnd::HandleMessage( uMsg, wParam, lParam );
}

HWND CChargeWnd::CreateThis( HWND hHostWnd )
{
	//CreateWindowEx(WS_EX_LAYERED, TEXT("AppTest"), GetWindowClassName(), WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP/*�ޱ߿���*/
	//	, 0, 0, 114, 52,
	//	NULL, NULL, hInstance, NULL);

	//Create( NULL, APP_TITLE, WS_POPUP, WS_EX_LAYERED|WS_EX_TOPMOST|WS_EX_TOOLWINDOW, 0, 0, PNG_WIDTH, PNG_HEIGHT );
	Create( NULL, APP_TITLE, WS_POPUP, WS_EX_LAYERED, 0, 0, PNG_WIDTH, PNG_HEIGHT );
	_hHostWnd = hHostWnd;

	 //RegisterTouchWindow(_hHostWnd, 0);  

	::ShowWindow( m_hWnd, SW_SHOW);
	UpdateWindow( m_hWnd );

	return m_hWnd;
}




void CChargeWnd::UpdateNormal()
{
	CStringW	strFile = CAppData::GetInstancePath() + L"mgc/mgc_bg3.png";
	Image	 image( strFile );

	int	sw = GetSystemMetrics( SM_CXSCREEN );
	int sh = GetSystemMetrics( SM_CYSCREEN );
	POINT	ptNow ={ sw-PNG_WIDTH-10, sh-PNG_HEIGHT-50 };

	CLayeredWindowDrawer	lwd( m_hWnd, &image, ptNow );

	Graphics	*pGr = lwd.getDrawer();
	DrawButton( pGr, BUTTON_TOP, false );
}


void CChargeWnd::UpdateHover()
{
	CStringW	strFile = CAppData::GetInstancePath() + L"mgc/mgc_bg2.png";
	Image	 image( strFile );

	int	sw = GetSystemMetrics( SM_CXSCREEN );
	int sh = GetSystemMetrics( SM_CYSCREEN );
	POINT	ptNow ={ sw-PNG_WIDTH-10, sh-PNG_HEIGHT1-50 };

	CLayeredWindowDrawer	lwd( m_hWnd, &image, ptNow );

	Graphics	*pGr = lwd.getDrawer();
	DrawInfo( pGr );
	DrawButton( pGr, BUTTON_TOP1, false );
}

void CChargeWnd::UpdateButtonHover()
{
	CStringW	strFile = CAppData::GetInstancePath() + L"mgc/mgc_bg2.png";
	Image	 image( strFile );

	int	sw = GetSystemMetrics( SM_CXSCREEN );
	int sh = GetSystemMetrics( SM_CYSCREEN );
	POINT	ptNow ={ sw-PNG_WIDTH-10, sh-PNG_HEIGHT1-50 };

	CLayeredWindowDrawer	lwd( m_hWnd, &image, ptNow );

	Graphics	*pGr = lwd.getDrawer();
	DrawInfo( pGr );
	DrawButton( pGr, BUTTON_TOP1, true );
}

void CChargeWnd::DrawButton( Graphics *pGr, int top, bool bButtonHover )
{
#define		BTN_LEFT		50

#define		BTN_W			220
#define		BTN_H			50

	SolidBrush	sbButton( Color(0xFF222222) );
	if( bButtonHover )
		sbButton.SetColor( Color(0xCF222222) );

	GraphicsPath	gp;
	CMainHelper::AddRoundRect( gp, BTN_LEFT, top, BTN_W, BTN_H, 24, 24 );
	pGr->FillPath( &sbButton, &gp );

	Font			ft2( L"΢���ź�", 16 );
	StringFormat	sf3;
	sf3.SetAlignment( StringAlignmentCenter );


	pGr->DrawString( L"�������� >", 6, &ft2, RectF(BTN_LEFT, top+10, BTN_W, BTN_H), &sf3, &SolidBrush(Color::White) );
}

void CChargeWnd::DrawInfo( Graphics *pGr )
{
	Pen		penLine( Color(0xFF999999), 1 );	
	pGr->DrawLine( &penLine, 30, 120, 290, 120 );
	pGr->DrawLine( &penLine, 30, 166, 290, 166 );

	SolidBrush	sbLabel( Color(0xFF999999) );
	SolidBrush	sbInfo( Color(0xFF222222) );

	Font			ft( L"΢���ź�", 14 );
	StringFormat	sf;

	pGr->DrawString( L"����", 2, &ft, PointF(30,84), &sf, &sbLabel );
	pGr->DrawString( L"�Ʒ�", 2, &ft, PointF(30,84+46), &sf, &sbLabel );
	pGr->DrawString( L"��֧��", 3, &ft, PointF(30,84+46+46), &sf, &sbLabel );

	StringFormat	sf2;
	sf2.SetAlignment( StringAlignmentFar );
	//pGr->DrawString( L"1Сʱ23��", 6, &ft, PointF(290,84), &sf2, &sbInfo );
	//pGr->DrawString( L"1Ԫ/10����", 7, &ft, PointF(290,84+46), &sf2, &sbInfo );
	//pGr->DrawString( L"55.6Ԫ", 6, &ft, PointF(290,84+46+46), &sf2, &sbInfo );

	CSettingMgr		*s = SetMgr();

	CStringW		str;
	int		minute = s->_duration.ToInt();
	if( minute >= 60 )
	{
		if( minute%60 == 0 )
			str.Format( L"%dСʱ", minute/60 );
		else
			str.Format( L"%dСʱ%d����", minute/60, minute%60 );
	}
	else
		str.Format( L"%d����", minute );
	pGr->DrawString( str, str.GetLength(), &ft, PointF(290,84), &sf2, &sbInfo );

	str.Format( L"%sԪ/����", s->_charging );
	pGr->DrawString( str, str.GetLength(), &ft, PointF(290,84+46), &sf2, &sbInfo );

	str.Format( L"%sԪ", s->_cost );
	pGr->DrawString( str, str.GetLength(), &ft, PointF(290,84+46+46), &sf2, &sbInfo );
}





void CChargeWnd::Update()
{
	if( _nType == 0 )
		UpdateNormal();
	else if( _nType == 1 )
		UpdateHover();
	else if( _nType == 2 )
		UpdateButtonHover();

	return;


	////CStringW	strFile = CAppData::GetInstancePath() + L"tp/exittp_normal.png";
	////if( 1 == nType )
	////	strFile = CAppData::GetInstancePath() + L"tp/exittp_hover.png";
	////else if( 2 == nType )
	////	strFile = CAppData::GetInstancePath() + L"tp/exittp_pushed.png";
	//CStringW	strFile = CAppData::GetInstancePath() + L"mgc/mgc_bg.png";

	//Image	 image( strFile );
	//int iWidth = image.GetWidth();
	//int iHeight = image.GetHeight();

	//HDC hdcScreen = GetDC(NULL);
	//HDC hdcMem = CreateCompatibleDC(hdcScreen);
	//HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, iWidth, iHeight);
	//HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

	//Graphics gr(hdcMem);
	//gr.SetSmoothingMode( SmoothingModeHighQuality );
	//gr.SetTextRenderingHint( TextRenderingHintAntiAlias );
	//gr.DrawImage(&image, 0, 0);//��pngͼ����Ƶ���̨DC��

	////Pen		penLine( Color(0xFF999999), 1 );	
	////gr.DrawLine( &penLine, 30, 120, 290, 120 );
	////gr.DrawLine( &penLine, 30, 166, 290, 166 );

	////SolidBrush	sbLabel( Color(0xFF999999) );
	////SolidBrush	sbInfo( Color(0xFF222222) );

	////Font			ft( L"΢���ź�", 14 );
	////StringFormat	sf;
	//////sf.SetAlignment( StringAlignmentCenter );

	////gr.DrawString( L"����", 2, &ft, PointF(30,84), &sf, &sbLabel );
	////gr.DrawString( L"�Ʒ�", 2, &ft, PointF(30,84+46), &sf, &sbLabel );
	////gr.DrawString( L"��֧��", 3, &ft, PointF(30,84+46+46), &sf, &sbLabel );

	////StringFormat	sf2;
	////sf2.SetAlignment( StringAlignmentFar );


	////CSettingMgr		*s = SetMgr();

	////CStringW		str;
	////int		minute = s->_duration.ToInt();
	////if( minute >= 60 )
	////{
	////	if( minute%60 == 0 )
	////		str.Format( L"%dСʱ", minute/60 );
	////	else
	////		str.Format( L"%dСʱ%d����", minute/60, minute%60 );
	////}
	////else
	////	str.Format( L"%d����", minute );
	////gr.DrawString( str, str.GetLength(), &ft, PointF(290,84), &sf2, &sbInfo );

	////str.Format( L"%sԪ/����", s->_charging );
	////gr.DrawString( str, str.GetLength(), &ft, PointF(290,84+46), &sf2, &sbInfo );

	////str.Format( L"%sԪ", s->_cost );
	////gr.DrawString( str, str.GetLength(), &ft, PointF(290,84+46+46), &sf2, &sbInfo );

	//SolidBrush	sbButton( Color(0xFF222222) );
	//if( 2 == _nType )
	//	sbButton.SetColor( Color(0xCF222222) );

	//GraphicsPath	gp;
	//CMainHelper::AddRoundRect( gp, 50, BUTTON_TOP, 220, 50, 24, 24 );
	//gr.FillPath( &sbButton, &gp );

	//Font			ft2( L"΢���ź�", 16 );
	//StringFormat	sf3;
	//sf3.SetAlignment( StringAlignmentCenter );


	//gr.DrawString( L"�������� >", 6, &ft2, RectF(50, BUTTON_TOP+10, 220, 50), &sf3, &SolidBrush(Color::White) );


	//BLENDFUNCTION blend = { 0 };
	//blend.BlendOp = AC_SRC_OVER;
	//blend.SourceConstantAlpha = 255;
	//blend.AlphaFormat = AC_SRC_ALPHA;//��ͨ�����

	//POINT	pSrc = { 0, 0 };
	//SIZE	sizeWnd = { iWidth, iHeight };
	//UpdateLayeredWindow( m_hWnd, hdcScreen, &_ptNow, &sizeWnd, hdcMem, &pSrc, NULL, &blend, ULW_ALPHA);//���·ֲ㴰��
	////��β������
	//SelectObject(hdcMem, hBitmapOld);
	//DeleteObject(hBitmap);
	//DeleteDC(hdcMem);

	//ReleaseDC(m_hWnd, hdcScreen);
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
		////if( DUI_CANCEL == DuiMessageBox(::GetDesktopWindow(), L"ȷ����ֹͬ����ʾ?",L"��ܰ��ʾ",DUI_YESNO ) )
		////	return false;
		//if( IDNO == MessageBox( ::GetDesktopWindow(), L"ȷ���˳���", L"", MB_YESNO|MB_ICONQUESTION ) )
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
	//			MessageBox( m_hWnd, L"���ϻ���!!!\n��ʽ�棬��������»������������", L"", 0 );
	//			//Hide();
	//			break;
	//		}
	//	}

	//	if( WAIT_OBJECT_0 == thWaitEvent(10000) )
	//		break;
	//}
}
