#include "StdAfx.h"
#include "LayeredWindowDrawer.h"


CLayeredWindowDrawer::CLayeredWindowDrawer( HWND hWnd, Image *image, const POINT ptTo )
	: m_hWnd( hWnd )
	, m_image( image )
	, m_ptTo( ptTo )
{
	_iw = m_image->GetWidth();
	_ih = m_image->GetHeight();

	_hDcScreen = GetDC( m_hWnd );
	_hDcMem = CreateCompatibleDC( _hDcScreen );

	_hBitmap = CreateCompatibleBitmap( _hDcScreen, _iw, _ih );
	_hBitmapOld = (HBITMAP)SelectObject( _hDcMem, _hBitmap);

	_spGr.reset( new Graphics( _hDcMem ) );

	_spGr->SetSmoothingMode( SmoothingModeHighQuality );
	_spGr->SetTextRenderingHint( TextRenderingHintAntiAlias );
	_spGr->DrawImage( m_image, 0, 0);
}


CLayeredWindowDrawer::~CLayeredWindowDrawer(void)
{
	BLENDFUNCTION bf = { 0 };
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 255;
	bf.AlphaFormat = AC_SRC_ALPHA;//按通道混合

	POINT	pSrc = { 0, 0 };
	SIZE	sizeWnd = { _iw, _ih };
	UpdateLayeredWindow( m_hWnd, _hDcScreen, &m_ptTo, &sizeWnd, _hDcMem, &pSrc, NULL, &bf, ULW_ALPHA);//更新分层窗口
	
	//收尾清理工作
	SelectObject( _hDcMem, _hBitmapOld);
	DeleteObject( _hBitmap );
	DeleteDC( _hDcMem );

	ReleaseDC( m_hWnd, _hDcScreen );
}






CLayeredWindowDrawer2::CLayeredWindowDrawer2( HWND hWnd, CDUIRect &rt )
{
	m_hWnd = hWnd;
	m_rt = rt;

	//m_hWnd2 = GetDesktopWindow();

	_hDcScreen = GetDC( m_hWnd );
	_hDcMem = CreateCompatibleDC( _hDcScreen );
	
	_hBitmap = CreateCompatibleBitmap( _hDcScreen, m_rt.Width(), m_rt.Height() );

	//BITMAPINFOHEADER bmih;
	//memset(&bmih, 0, sizeof(BITMAPINFOHEADER));
	//bmih.biSize = sizeof(BITMAPINFOHEADER);
	//bmih.biBitCount = 24;
	//bmih.biCompression = BI_RGB;
	//bmih.biPlanes = 1;
	//bmih.biWidth = m_rt.Width();
	//bmih.biHeight = m_rt.Height();
	//BITMAPINFO bmi;
	//memset(&bmi, 0, sizeof(BITMAPINFO));
	//bmi.bmiHeader = bmih;
	//void* p; 
	//_hBitmap = ::CreateDIBSection(_hDcScreen, &bmi, DIB_RGB_COLORS, &p, NULL, 0);




	_hBitmapOld = (HBITMAP)SelectObject( _hDcMem, _hBitmap);
}

CLayeredWindowDrawer2::~CLayeredWindowDrawer2()
{
	BLENDFUNCTION bf = { 0 };
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 250;
	bf.AlphaFormat = AC_SRC_ALPHA;//按通道混合

	POINT	pTo = { m_rt.left, m_rt.top };
	POINT	pSrc = { 0, 0 };
	SIZE	sizeWnd = { m_rt.Width(), m_rt.Height() };
	UpdateLayeredWindow( m_hWnd, _hDcScreen, &pTo, &sizeWnd, _hDcMem, &pSrc, NULL, &bf, ULW_ALPHA);//更新分层窗口
	 
	SelectObject( _hDcMem, _hBitmapOld);
	DeleteObject( _hBitmap );
	DeleteDC( _hDcMem );

	ReleaseDC( m_hWnd, _hDcScreen );
}
