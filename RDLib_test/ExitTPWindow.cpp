#include "StdAfx.h"
#include "ExitTPWindow.h"

CExitTPWindow*	CExitTPWindow::s_instance = nullptr;

#define		PNG_WIDTH		320
#define		PNG_HEIGHT		304


CExitTPWindow::CExitTPWindow(void)
	: m_bMouseTracking( false )
	, m_bPosChanged( false )
	, _hHostWnd( nullptr )
{
	int	sw = GetSystemMetrics( SM_CXSCREEN );
	int sh = GetSystemMetrics( SM_CYSCREEN );

	_ptNow.SetPoint( sw-PNG_WIDTH-10, sh-PNG_HEIGHT-50 );
}


CExitTPWindow::~CExitTPWindow(void)
{
}

LRESULT CExitTPWindow::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	static BOOL		ldown;
	static POINT	TheFirstPoint;

	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			ldown = TRUE;
			SetCapture( m_hWnd );
			TheFirstPoint.x = LOWORD(lParam);
			TheFirstPoint.y = HIWORD(lParam);
			Update( 1 );

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
					//MessageBox( m_hWnd, L"", L"", MB_OK );
					PostQuitMessage( WM_QUIT );
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
			{
				Update( 2 );
			}
			else
				Update( 1 );

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
			Update( 1 );
		}
		break;

	case WM_MOUSELEAVE:
		{
			if( m_bMouseTracking ) ::SendMessage(m_hWnd, WM_MOUSEMOVE, 0, (LPARAM) -1);
			m_bMouseTracking = false;
			Update( 0 );
		}
		break;

	case WM_CREATE:
		{
			Update( 0 );

			//�����Ӱ
			CWndShadow::Initialize(CAppData::GetInstance());
			m_shadow.Create(m_hWnd);
			m_shadow.SetSize(2);
			m_shadow.SetPosition(1, 1);
		}
		break;
	}

	return CWindowWnd::HandleMessage( uMsg, wParam, lParam );
}

HWND CExitTPWindow::CreateThis( HWND hHostWnd )
{
	//CreateWindowEx(WS_EX_LAYERED, TEXT("AppTest"), GetWindowClassName(), WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP/*�ޱ߿���*/
	//	, 0, 0, 114, 52,
	//	NULL, NULL, hInstance, NULL);

	Create( NULL, NULL, WS_POPUP, WS_EX_LAYERED|WS_EX_TOPMOST|WS_EX_TOOLWINDOW, 0, 0, PNG_WIDTH, PNG_HEIGHT );
	_hHostWnd = hHostWnd;

	 //RegisterTouchWindow(_hHostWnd, 0);  

	::ShowWindow( m_hWnd, SW_SHOW);
	UpdateWindow( m_hWnd );


	return m_hWnd;
}

void AddRoundRect( GraphicsPath &gp, INT x, INT y, INT width, INT height, INT cornerX, INT cornerY)  
{  
	INT elWid = 2*cornerX;  
	INT elHei = 2*cornerY;  

	gp.AddArc(x,y,elWid,elHei,180,90); // ���Ͻ�Բ��  
	gp.AddLine(x+cornerX,y,x+width-cornerX,y); // �ϱ�  

	gp.AddArc(x+width-elWid,y, elWid,elHei,270,90); // ���Ͻ�Բ��  
	gp.AddLine(x+width,y+cornerY, x+width,y+height-cornerY);// �ұ�  

	gp.AddArc(x+width-elWid,y+height-elHei, elWid,elHei,0,90); // ���½�Բ��  
	gp.AddLine(x+width-cornerX,y+height, x+cornerX,y+height); // �±�  

	gp.AddArc(x,y+height-elHei, elWid,elHei,90,90);   
	gp.AddLine(x,y+cornerY, x, y+height-cornerY);  
}  

void CExitTPWindow::Update( int nType )
{
	//CStringW	strFile = CAppData::GetInstancePath() + L"tp/exittp_normal.png";
	//if( 1 == nType )
	//	strFile = CAppData::GetInstancePath() + L"tp/exittp_hover.png";
	//else if( 2 == nType )
	//	strFile = CAppData::GetInstancePath() + L"tp/exittp_pushed.png";
	CStringW	strFile = CAppData::GetInstancePath() + L"tp/mgc_bg2.png";

	Image	 image( strFile );
	int iWidth = image.GetWidth();
	int iHeight = image.GetHeight();

	HDC hdcScreen = GetDC(NULL);
	HDC hdcMem = CreateCompatibleDC(hdcScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, iWidth, iHeight);
	HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

	Graphics gr(hdcMem);
	gr.SetSmoothingMode( SmoothingModeHighQuality );
	gr.DrawImage(&image, 0, 0);//��pngͼ����Ƶ���̨DC��

	Pen		penLine( Color(0xFF999999), 1 );	
	gr.DrawLine( &penLine, 30, 120, 290, 120 );
	gr.DrawLine( &penLine, 30, 166, 290, 166 );

	SolidBrush	sbLabel( Color(0xFF999999) );
	SolidBrush	sbInfo( Color(0xFF222222) );

	Font			ft( L"΢���ź�", 14 );
	StringFormat	sf;
	//sf.SetAlignment( StringAlignmentCenter );

	gr.DrawString( L"����", 2, &ft, PointF(30,84), &sf, &sbLabel );
	gr.DrawString( L"�Ʒ�", 2, &ft, PointF(30,84+46), &sf, &sbLabel );
	gr.DrawString( L"��֧��", 3, &ft, PointF(30,84+46+46), &sf, &sbLabel );

	StringFormat	sf2;
	sf2.SetAlignment( StringAlignmentFar );
	gr.DrawString( L"1Сʱ23��", 6, &ft, PointF(290,84), &sf2, &sbInfo );
	gr.DrawString( L"1Ԫ/10����", 7, &ft, PointF(290,84+46), &sf2, &sbInfo );
	gr.DrawString( L"55.6Ԫ", 6, &ft, PointF(290,84+46+46), &sf2, &sbInfo );

	SolidBrush	sbButton( Color(0xFF222222) );
	if( 2 == nType )
		sbButton.SetColor( Color(0xCF222222) );

	GraphicsPath	gp;
	AddRoundRect( gp, 50, 226, 220, 50, 24, 24 );
	gr.FillPath( &sbButton, &gp );

	Font			ft2( L"΢���ź�", 16 );
	StringFormat	sf3;
	sf3.SetAlignment( StringAlignmentCenter );


	gr.DrawString( L"�������� >", 6, &ft2, RectF(50, 236, 220, 50), &sf3, &SolidBrush(Color::White) );


	BLENDFUNCTION blend = { 0 };
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;//��ͨ�����

	POINT	pSrc = { 0, 0 };
	SIZE	sizeWnd = { iWidth, iHeight };
	UpdateLayeredWindow( m_hWnd, hdcScreen, &_ptNow, &sizeWnd, hdcMem, &pSrc, NULL, &blend, ULW_ALPHA);//���·ֲ㴰��
	//��β������
	SelectObject(hdcMem, hBitmapOld);
	DeleteObject(hBitmap);
	DeleteDC(hdcMem);

	ReleaseDC(m_hWnd, hdcScreen);
}

void CExitTPWindow::Show( HWND hParentWnd )
{
	if( s_instance == nullptr )
	{
		s_instance = new CExitTPWindow;
		s_instance->CreateThis( hParentWnd );
	}
	else
	{
		s_instance->ShowWindow();
	}
}

bool CExitTPWindow::Hide()
{
	if( s_instance != nullptr && 
		IsWindow( s_instance->GetHWND() ) )
	{
		//if( DUI_CANCEL == DuiMessageBox(::GetDesktopWindow(), L"ȷ����ֹͬ����ʾ?",L"��ܰ��ʾ",DUI_YESNO ) )
		//	return false;
		if( IDNO == MessageBox( ::GetDesktopWindow(), L"ȷ���˳���", L"", MB_YESNO|MB_ICONQUESTION ) )
			return false;

		s_instance->ShowWindow( false );
		return true;
	}

	return false;
}

void CExitTPWindow::Release()
{
	DestroyWindow( s_instance->m_hWnd );
	delete s_instance;
	s_instance = nullptr;
}
