#include "StdAfx.h"
#include "01WndScanEnter.h"


C01WndScanEnter::C01WndScanEnter(void)
{
	_spImageBK.reset( new Image( CAppData::GetInstancePath() + L"mgc2/bg.jpg" ) );
	_spImage2w.reset( new Image( CAppData::GetInstancePath() + L"mgc2/info2w.jpg" ) );
	_spImageBn.reset( new Image( CAppData::GetInstancePath() + L"mgc2/btn-1.png" ) );

	_bHover = 0;
	_time = 30;
}


C01WndScanEnter::~C01WndScanEnter(void)
{
}

LRESULT C01WndScanEnter::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT	nRet = 0;
	BOOL handled = FALSE;

	switch (uMsg)
	{
	case WM_TIMER:
		{
			CDUIRect		rt;
			GetClientRect( m_hWnd, rt );

			int w = rt.GetWidth();
			int h = rt.GetHeight();

			int iw = _spImageBn->GetWidth();
			int ih = _spImageBn->GetHeight();

			CDUIPoint		pt( LOWORD(lParam), HIWORD(lParam) );
			CDUIRect		rtHover( w-iw, 0,  w, ih );
			InvalidateRect( m_hWnd, rtHover, FALSE );

			_time--;
			if( _time <= 0 )
			{
				KillTimer( m_hWnd, 1 );
				PostQuitMessage( 0 );
			}
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
			if( _bHover )
			{
				Hide();
				//PostQuitMessage( 0 );
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			CDUIRect		rt;
			GetClientRect( m_hWnd, rt );

			int w = rt.GetWidth();
			int h = rt.GetHeight();

			int iw = _spImageBn->GetWidth();
			int ih = _spImageBn->GetHeight();

			CDUIPoint		pt( LOWORD(lParam), HIWORD(lParam) );
			CDUIRect		rtHover( w-iw, 0,  w, ih );
			int nHoverNew = PtInRect( &rtHover, pt );
			
			if( _bHover != nHoverNew )
			{
				_bHover = nHoverNew;
				InvalidateRect( m_hWnd, &rtHover, FALSE );
			}
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

			Graphics		g( hDC );

			CDUIRect		rt;
			GetClientRect( m_hWnd, rt );

			int w = rt.GetWidth();
			int h = rt.GetHeight();

			Bitmap	bmp( w, h );
			Graphics		gr( &bmp );

			int iw = _spImageBK->GetWidth();
			int ih = _spImageBK->GetHeight();

			gr.DrawImage( _spImageBK.get(), Rect(0,0,w,h), 
				0, 0, iw, ih, UnitPixel );

			iw = _spImage2w->GetWidth();
			ih = _spImage2w->GetHeight();
			int isw = 200;
			int ish = 200;

			gr.DrawImage( _spImage2w.get(), Rect((w-isw)/2,(h-ish)/2,isw,ish),
				0, 0, iw, ih, UnitPixel );


			iw = _spImageBn->GetWidth();
			ih = _spImageBn->GetHeight();

			if( _bHover )
			{
				ColorMatrix		cm =  {
								1.f, 0.f, 0.f, 0.f, 0.f,
								0.f, 1.f, 0.f, 0.f, 0.f,
								0.f, 0.f, 1.f, 0.f, 0.f,
								0.f, 0.f, 0.f, .5f, 0.f,
								0.f, 0.f, 0.f, 0.f, 1.f,
							};

				ImageAttributes		ia;
				ia.SetColorMatrix( &cm );

				gr.DrawImage( _spImageBn.get(), Rect(w-iw,0,iw,ih), 
					0, 0, iw, ih, UnitPixel, &ia );
			}
			else
				gr.DrawImage( _spImageBn.get(), Rect(w-iw,0,iw,ih), 
								0, 0, iw, ih, UnitPixel );

			SolidBrush	sbLabel( Color(0xFFffffff) );

			Font			ft( L"Î¢ÈíÑÅºÚ", 16 );
			StringFormat	sf;
			sf.SetAlignment( StringAlignmentCenter );

			CStringW		str;
			str.Format( L"·µ»ØÊ×Ò³£¨%ds£©", _time );

			gr.DrawString( str, str.GetLength(), &ft, RectF(w-iw+20,10,iw,ih), &sf, &sbLabel );

			g.DrawImage( &bmp, 0, 0 );

			EndPaint(*this, &ps);
			handled = TRUE;
		}
		break;
	case WM_SHOWWINDOW:
		{
			//bool bShow = (wParam==TRUE);
			//CMainHelper::ShowTrayWndAndStartWnd( !bShow );
		}
		break;
	case WM_CLOSE:
		{
			//CMainHelper::ShowTrayWndAndStartWnd( true );
			//PostQuitMessage(0);
		}
		break;
	}

	if( handled )
		return nRet;

	return CWindowWnd::HandleMessage( uMsg, wParam, lParam );
}






C01ScanEnterDlg::C01ScanEnterDlg(void)
{
	_time = 30;
}


C01ScanEnterDlg::~C01ScanEnterDlg(void)
{
}

void C01ScanEnterDlg::Init()
{
	::ShowWindow( m_hWnd, SW_MAXIMIZE );
	//CLayeredDialogBase::Init();

	SetTimer( m_hWnd, 1, 1000, nullptr );
}

LRESULT C01ScanEnterDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//EndModal( IDCANCEL );
	ShowWindow( false );
	return 0;
}

CControlUI* C01ScanEnterDlg::CreateControl( LPCTSTR pstrClass )
{
	return NULL;
}

void C01ScanEnterDlg::Notify( TNotifyUI& msg )
{
	CStringW	strSenderName = msg.pSender->GetName();
	CStringW	strType = msg.sType;

	if( strType == L"click")
	{
		if( strSenderName == L"bnOK" )
		{
			EndModal( IDOK );
		}
		else if( strSenderName == L"bnCancel" )
		{
			EndModal( IDCANCEL );
		}
	}
	else if( strType == L"timer" )
	{
	}
}

LRESULT C01ScanEnterDlg::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( WM_TIMER == uMsg )
	{
		CStringW		str;
		str.Format( L"·µ»ØÊ×Ò³£¨%ds£©", _time );
		FindCtrl( L"bnOK" )->SetText( str );

		_time--;
		if( _time <= 0 )
		{
			KillTimer( m_hWnd, 1 );
			EndModal( 0 );
		}
	}

	return CDialogBase::HandleMessage(uMsg, wParam, lParam);
}
