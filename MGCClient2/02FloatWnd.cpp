#include "StdAfx.h"
#include "02FloatWnd.h"

#include "LayeredWindowDrawer.h"
#pragma comment( lib, "comctl32.lib")

#define		PNG_WIDTH		320
#define		PNG_HEIGHT1		304
#define		PNG_HEIGHT		166
#define		BUTTON_TOP1		226
#define		BUTTON_TOP		88

C02FloatWnd*	C02FloatWnd::s_instance = nullptr;

C02FloatWnd::C02FloatWnd(void)
	: _hHostWnd( nullptr )
{
}


C02FloatWnd::~C02FloatWnd(void)
{
}

LRESULT C02FloatWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_TIMER:
		Update();
		break;

	case WM_CREATE:
		{
			Update();
			SetTimer( m_hWnd, 1, 100, NULL );
		}
		break;
	case WM_CLOSE:
		{
			//::PostMessage( _hHostWnd, WM_SHOWA, 3, 0 );
			Show( _hHostWnd, _ptNow );
			return 1;
		}
		break;
	}

	return CWindowWnd::HandleMessage( uMsg, wParam, lParam );
}

HWND C02FloatWnd::CreateThis( HWND hHostWnd )
{
	Create( NULL, L"", WS_POPUP, WS_EX_LAYERED|WS_EX_TOOLWINDOW|WS_EX_TOPMOST, 0, 0, PNG_WIDTH, PNG_HEIGHT );
	_hHostWnd = hHostWnd;

	::ShowWindow( m_hWnd, SW_SHOW);
	UpdateWindow( m_hWnd );

	return m_hWnd;
}

//void C02FloatWnd::DrawInfo( Graphics *pGr )
//{
//	Pen		penLine( Color(0xFF999999), 1 );	
//	pGr->DrawLine( &penLine, 30, 120, 290, 120 );
//	pGr->DrawLine( &penLine, 30, 166, 290, 166 );
//
//	SolidBrush	sbLabel( Color(0xFF999999) );
//	SolidBrush	sbInfo( Color(0xFF222222) );
//
//	Font			ft( L"微软雅黑", 14 );
//	StringFormat	sf;
//
//	pGr->DrawString( L"已玩", 2, &ft, PointF(30,84), &sf, &sbLabel );
//	pGr->DrawString( L"计费", 2, &ft, PointF(30,84+46), &sf, &sbLabel );
//	pGr->DrawString( L"需支付", 3, &ft, PointF(30,84+46+46), &sf, &sbLabel );
//
//	StringFormat	sf2;
//	sf2.SetAlignment( StringAlignmentFar );
//	//pGr->DrawString( L"1小时23分", 6, &ft, PointF(290,84), &sf2, &sbInfo );
//	//pGr->DrawString( L"1元/10分钟", 7, &ft, PointF(290,84+46), &sf2, &sbInfo );
//	//pGr->DrawString( L"55.6元", 6, &ft, PointF(290,84+46+46), &sf2, &sbInfo );
//
//	CSettingMgr		*s = SetMgr();
//
//	CStringW		str;
//	int		minute = s->_duration.ToInt();
//	if( minute >= 60 )
//	{
//		if( minute%60 == 0 )
//			str.Format( L"%d小时", minute/60 );
//		else
//			str.Format( L"%d小时%d分钟", minute/60, minute%60 );
//	}
//	else
//		str.Format( L"%d分钟", minute );
//	pGr->DrawString( str, str.GetLength(), &ft, PointF(290,84), &sf2, &sbInfo );
//
//	str.Format( L"%s元/分钟", s->_charging );
//	pGr->DrawString( str, str.GetLength(), &ft, PointF(290,84+46), &sf2, &sbInfo );
//
//	str.Format( L"%s元", s->_cost );
//	pGr->DrawString( str, str.GetLength(), &ft, PointF(290,84+46+46), &sf2, &sbInfo );
//}

void C02FloatWnd::Update()
{
	if( _ptNow.x + _ptNow.y <= 10 )
		return;

	CStringW strName, strPhone, strV1, strV2, strV3;
	getInfo( strName, strPhone, strV1, strV2, strV3 );


	CStringW	strFile = CAppData::GetResourcePath() + L"costbg.png";
	Image	 image( strFile );

	CLayeredWindowDrawer	lwd( m_hWnd, &image, _ptNow );
	Graphics	*pGr = lwd.getDrawer();

	// 头像
	CStringW	strFace = CAppData::GetResourcePath() + L"face.png";
	Image		imgFace( strFace );
	pGr->DrawImage( &imgFace, 
		RectF(20.f, 20.f, 40.f, 40.f), 
		0, 0, 40, 40, UnitPixel );


	// 垂直居中
	StringFormat	sf;
	//	sf.SetAlignment( StringAlignmentFar );
	sf.SetLineAlignment( StringAlignmentCenter );

	SolidBrush		sbWhite( Color::White );

	// name
	Font		ft18b( L"Microsoft yahei", 18, FontStyleBold, UnitPixel );
	pGr->DrawString( strName, strName.GetLength(), &ft18b, 
		RectF(70.f, 15.5f, 182.f, 25.f), &sf, &sbWhite );

	// phone6207
	Font		ft12( L"Microsoft yahei", 12, 0, UnitPixel );
	pGr->DrawString( strPhone, strPhone.GetLength(), &ft12, 
		RectF(70.f, 40.f, 182.f, 25.f), &sf, &sbWhite );

	Font		ft14( L"Microsoft yahei", 14, 0, UnitPixel );
	SolidBrush	sbText( Color(0xFFd1d1d1) );
	pGr->DrawString( L"已玩：", 3, &ft14, RectF(20,80,60,20), &sf, &sbText );
	pGr->DrawString( L"计费：", 3, &ft14, RectF(20,80+30,60,20), &sf, &sbText );
	pGr->DrawString( L"需付：", 3, &ft14, RectF(20,80+60,60,20), &sf, &sbText );

	pGr->DrawString( strV1, strV1.GetLength(), &ft14, RectF(70,80,190,20), &sf, &sbText );
	pGr->DrawString( strV2, strV2.GetLength(), &ft14, RectF(70,80+30,190,20), &sf, &sbText );
	pGr->DrawString( strV3, strV3.GetLength(), &ft14, RectF(70,80+60,190,20), &sf, &sbText );
}

void C02FloatWnd::Show( HWND hHostWnd, const CDUIPoint &pt)
{
	if( s_instance == nullptr )
	{
		s_instance = new C02FloatWnd;
		s_instance->CreateThis( hHostWnd );
	}
	else
	{
		s_instance->_ptNow = pt;
		s_instance->ShowWindow();
		s_instance->Update();
	}
}

bool C02FloatWnd::Hide()
{
	if( s_instance != nullptr && 
		IsWindow( s_instance->GetHWND() ) )
	{
		s_instance->ShowWindow( false );
		return true;
	}

	return false;
}

void C02FloatWnd::Release()
{
	DestroyWindow( s_instance->m_hWnd );
	delete s_instance;
	s_instance = nullptr;
}

void C02FloatWnd::thread_main()
{

}

void C02FloatWnd::getInfo( CStringW &strName, CStringW &strPhone, CStringW &strV1, CStringW &strV2, CStringW &strV3 )
{
	//todo
	strName = L"ramble";
	strPhone = L"136****6207";
	strV1 = L"122小时15分";
	strV2 = L"0.2元/分钟";
	strV3 = L"222元";
}
