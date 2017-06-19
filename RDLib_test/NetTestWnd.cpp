#include "StdAfx.h"
#include "NetTestWnd.h"

// IsNetworkAlive
#include <Sensapi.h>
#pragma comment( lib, "Sensapi.lib" )


#define		PNG_WIDTH		400
#define		PNG_HEIGHT		300

CNetTestWnd*	CNetTestWnd::s_instance = nullptr;

CNetTestWnd::CNetTestWnd(void)
{
}


CNetTestWnd::~CNetTestWnd(void)
{
}


bool CNetTestWnd::NetOK()
{
	DWORD	dwFlags = 0;
	if( IsNetworkAlive( &dwFlags ) )
		return true;

	return false;
}

LRESULT CNetTestWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT nRet = 0;
	BOOL bHandled = TRUE;

	switch (uMsg)
	{
		case WM_CREATE:	nRet = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_SIZE:	nRet = OnSize(uMsg, wParam, lParam, bHandled); break;
	}

	if (paint_manager_.MessageHandler(uMsg, wParam, lParam, nRet)) 
		return nRet;

	return CWindowWnd::HandleMessage( uMsg, wParam, lParam );
}

LRESULT CNetTestWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	if( styleValue & WS_DLGFRAME )
	{
		bHandled = FALSE;
		return 0;
	}

	SIZE szRoundCorner = paint_manager_.GetRoundCorner();
	//#if defined(WIN32) && !defined(UNDER_CE)
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		CDUIRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		if (GetWindowStyle(m_hWnd) & WS_MAXIMIZE){
			szRoundCorner.cx = szRoundCorner.cy = 0;
		}
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}
	//#endif
	bHandled = FALSE;
	return 0;
}

void CNetTestWnd::Show()
{
	if( s_instance == nullptr )
	{
		s_instance = new CNetTestWnd;
		s_instance->CreateThis();
	}
	else
	{
		s_instance->ShowWindow();
	}
}

bool CNetTestWnd::Hide()
{
	if( s_instance != nullptr && IsWindow( s_instance->GetHWND() ) )
	{
		////if( DUI_CANCEL == DuiMessageBox(::GetDesktopWindow(), L"确定终止同屏演示?",L"温馨提示",DUI_YESNO ) )
		//if( IDYES == ::MessageBox( ::GetDesktopWindow(), L"确认退出？", L"", MB_YESNO ) )
		//	return false;

		s_instance->HideThis();
		s_instance->ShowWindow( false );
		return true;
	}

	return false;
}

void CNetTestWnd::Release()
{
	DestroyWindow( s_instance->m_hWnd );
	delete s_instance;
	s_instance = nullptr;
}

HWND CNetTestWnd::CreateThis()
{
	Create( NULL, NULL, WS_POPUP, WS_EX_TOPMOST|WS_EX_TOOLWINDOW, 0, 0, PNG_WIDTH, PNG_HEIGHT );

	ShowWindow( m_hWnd, SW_SHOW);
	UpdateWindow( m_hWnd );

	return m_hWnd;
}

CControlUI* CNetTestWnd::CreateControl( LPCTSTR pstrClass )
{
	return nullptr;
}

LRESULT CNetTestWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);

	paint_manager_.Init(m_hWnd);
	paint_manager_.AddPreMessageFilter(this);

	CDialogBuilder builder;
	CStringW tstrSkin = L"NetTest.xml";

	CControlUI* pRoot = builder.Create( (LPCTSTR)tstrSkin, (UINT)0, this, &paint_manager_);
	if( pRoot == NULL )
	{
		//MsgBox( L"XML解析失败，请检查！" );
		return 0;
	}

	paint_manager_.AttachDialog(pRoot);
	paint_manager_.AddNotifier(this);

	//if( !(styleValue & WS_DLGFRAME) )
	{
		//添加阴影
		CWndShadow::Initialize(CAppData::GetInstance());
		m_shadow.Create(m_hWnd);
		m_shadow.SetSize(5);
		m_shadow.SetPosition(1, 1);
	}


	CDUIRect rt;
	::GetClientRect(m_hWnd, &rt);

	int w = rt.Width();
	int h = rt.Height();
	int	sw = GetSystemMetrics( SM_CXSCREEN );
	int sh = GetSystemMetrics( SM_CYSCREEN );
	::SetWindowPos(m_hWnd, NULL, (sw-w)/2, (sh-h)/2, w, h, SWP_FRAMECHANGED);

	CControlUI *pIcon = paint_manager_.FindControl( L"icon" );
	pIcon->GetManager()->SetTimer( pIcon, 1, 500 );

	return 0;
}

void CNetTestWnd::Notify( TNotifyUI& msg )
{
	CStringW	strSenderName = msg.pSender->GetName();
	CStringW	strType = msg.sType;

	if( strType == L"click")
	{
		if( strSenderName == L"bn" )
		{
			Hide();
		}
	}
	else if( strType == L"timer" )
	{
		static int sIndex = -1;
		sIndex = (sIndex+1)%3;

		CStringW	strImage;
		strImage.Format( L"net/net%d.png", sIndex );

		CControlUI *pIcon = paint_manager_.FindControl( L"icon" );
		pIcon->SetBkImage( strImage );

		//if( NetOK() )
		if( false )
		{
			CNetTestWnd::Hide();
		}
	}
}

LRESULT CNetTestWnd::MessageHandler( UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled )
{
	return FALSE;
}

void CNetTestWnd::HideThis()
{
	CControlUI *pIcon = paint_manager_.FindControl( L"icon" );
	pIcon->GetManager()->KillTimer( pIcon );
}
