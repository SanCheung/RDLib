#include "StdAfx.h"
#include <atlcomcli.h>

#include "BarCodeOcxUI.h"



CBarCodeOcxUI::CBarCodeOcxUI(void)
{
	_pSite = new COcxSite;
}


CBarCodeOcxUI::~CBarCodeOcxUI(void)
{
}



void CBarCodeOcxUI::SetValue( CStringW strValue )
{
	if( _pSite == nullptr )
		return;

	DISPID dispidNamed = DISPID_PROPERTYPUT;

	VARIANT vps[1];
	vps[0] = CComVariant( 7 );
	DISPPARAMS params = { vps, &dispidNamed, 1, 1 };
	_pSite->SetProperty( L"Style", &params );

	vps[0] = CComVariant( L"123214343" );
	DISPPARAMS params2 = { vps, &dispidNamed, 1, 1 };
	_pSite->SetProperty( L"Value", &params2 );

	vps[0] = CComVariant( 0xff7f7f );
	DISPPARAMS params3 = { vps, &dispidNamed, 1, 1 };
	_pSite->SetProperty( L"BackColor", &params3 );

	VARIANT v;
	_pSite->GetProperty( L"BackColor", &v );
}


CBarCodeOcxUI* CBarCodeOcxUI::CreateUI( HWND hWnd )
{
	CBarCodeOcxUI* pThis = new CBarCodeOcxUI;
	pThis->Create( hWnd, L"", UI_WNDSTYLE_CHILD, 0 );

	CControlUI *pControl = new CWndUI( pThis );
	pThis->m_pHost = pControl;

	return pThis;
}


LRESULT CBarCodeOcxUI::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = 0;
	BOOL handled = FALSE;

	if( WM_CREATE == uMsg )
	{
		// ProgID不区分大小写
		_pSite->Create( m_hWnd, L"BARCODE.BarCodeCtrl" );
		//_pSite->Create( m_hWnd, L"TPVIEWER.TPVCtrl.1" );
	}
	else if( WM_SIZE == uMsg )
	{
		CDUIRect rt;
		GetClientRect( m_hWnd, &rt);

		if( rt.GetWidth() + rt.GetHeight() > 0 && _pSite != nullptr )
			_pSite->Resize( &rt );
	}
	else if( WM_DESTROY == uMsg )
	{
		if( _pSite != nullptr )
		{
			_pSite->ReleaseAll();
			_pSite->Release();
			_pSite = nullptr;
		}
	}

	if (!handled)
	{
		lResult = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

	return lResult;
}
