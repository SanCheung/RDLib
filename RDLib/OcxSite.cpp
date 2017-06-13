#include "StdAfx.h"
#include "OcxSite.h"

#include <crtdbg.h>
#include <OCIdl.h>

#include <atltrace.h>
#pragma comment(lib,"atlsd.lib")


COcxSite::COcxSite()
{
	m_hParentWnd = nullptr;
	m_refCnt = 1;

	_pUnkObject = nullptr;
}


COcxSite::~COcxSite(void)
{
	ReleaseAll();
	
	_ASSERT(m_refCnt == 0);
}

void COcxSite::ReleaseAll()
{
	if( _pUnkObject == nullptr )
		return;

	IOleObject * pObject;
	if (SUCCEEDED(_pUnkObject->QueryInterface(IID_IOleObject,(void**)&pObject)))
	{
		pObject->SetClientSite( NULL );
		pObject->Close(0);
		pObject->Release();
	}

	_pUnkObject->Release();
	_pUnkObject = nullptr;
}

HRESULT COcxSite::CreateOcx( HWND hParentWnd, const wchar_t* strProgID, IUnknown **ppCtrl, IUnknown **ppContainer )
{
	CLSID		cls;
	HRESULT		hr;
	hr = CLSIDFromProgID( strProgID, &cls );
	if (FAILED(hr))	return hr;

	return CreateOcx( hParentWnd,cls, ppCtrl, ppContainer );
}

HRESULT COcxSite::CreateOcx( HWND hParentWnd, const CLSID cls, IUnknown **ppCtrl, IUnknown **ppContainer )
{
	HRESULT		hr;

	COcxSite	*pSite = nullptr;
	IOleObject	*pObject = nullptr;
	hr = CoCreateInstance(cls,NULL,CLSCTX_ALL,IID_IOleObject,(void**)&pObject);
	if (FAILED(hr)) goto exit1;

	pSite = new COcxSite;
	pSite->m_hParentWnd = hParentWnd;

	hr = pObject->SetClientSite( pSite );
	if (FAILED(hr)) goto exit1;

	hr = pObject->DoVerb( OLEIVERB_SHOW, 0, pSite, 0, hParentWnd, NULL );
	if (FAILED(hr)) goto exit1;

	hr = pObject->QueryInterface( IID_IUnknown, (void **)ppCtrl );
	if (FAILED(hr)) goto exit1;

	hr = pSite->QueryInterface( IID_IUnknown, (void **)ppContainer );
	if (FAILED(hr)) goto exit1;

exit1:
	if (pObject) pObject->Release();
	if (pSite) pSite->Release();
	return hr;
}

bool COcxSite::Create( HWND hParentWnd, const wchar_t* strProgID )
{
	CLSID		cls;
	HRESULT		hr;
	hr = CLSIDFromProgID( strProgID, &cls );
	if (FAILED(hr))	return false;

	return Create( hParentWnd, cls );
}

bool COcxSite::Create( HWND hParentWnd, const CLSID cls )
{
	m_hParentWnd = hParentWnd;

	HRESULT		hr;

	IOleObject	*pObject = nullptr;
	hr = CoCreateInstance(cls,NULL,CLSCTX_ALL,IID_IOleObject,(void**)&pObject);
	if (FAILED(hr)) goto exit1;

	hr = pObject->SetClientSite( this );
	if (FAILED(hr)) goto exit1;

	hr = pObject->DoVerb( OLEIVERB_SHOW, 0, this, 0, hParentWnd, NULL );
	if (FAILED(hr)) goto exit1;

	hr = pObject->QueryInterface( IID_IUnknown, (void **)&_pUnkObject );
	if (FAILED(hr)) goto exit1;

	//hr = QueryInterface( IID_IUnknown, (void **)&_pUnkContainer );
	//if (FAILED(hr)) goto exit1;

exit1:
	if (pObject) pObject->Release();
	//Release();
	
	return SUCCEEDED(hr);
}

bool COcxSite::Resize( LPRECT lprc )
{
	IOleInPlaceObject * pInPlaceObject = nullptr;
	if (_pUnkObject && SUCCEEDED(_pUnkObject->QueryInterface(IID_IOleInPlaceObject,(void**)&pInPlaceObject)))
	{
		pInPlaceObject->SetObjectRects( lprc, lprc );
		pInPlaceObject->Release();
		return true;
	}

	return false;
}


bool COcxSite::SetProperty( const wchar_t* strProp, DISPPARAMS *pDispValue )
{
	if( _pUnkObject == nullptr )
		return false;

	IDispatch *pDisp = NULL;
	HRESULT hr = _pUnkObject->QueryInterface(IID_IDispatch,(void**)&pDisp);
	if (FAILED(hr)) return false;

	DISPID anPropertyID[1] = {0};
	BSTR asPropName[1];
	asPropName[0] = SysAllocString( strProp );
	hr = pDisp->GetIDsOfNames(IID_NULL, asPropName, 1, 0, anPropertyID);
	SysFreeString(asPropName[0]);

	hr = pDisp->Invoke(anPropertyID[0], IID_NULL, 0, DISPATCH_PROPERTYPUT, 
		pDispValue, NULL, NULL, NULL);

	if (pDisp) 
		pDisp->Release();

	return SUCCEEDED(hr);
}



bool COcxSite::GetProperty( const wchar_t* strProp, VARIANT *pVar )
{
	if( _pUnkObject == nullptr )
		return false;

	IDispatch *pDisp = NULL;
	HRESULT hr = _pUnkObject->QueryInterface(IID_IDispatch,(void**)&pDisp);
	if (FAILED(hr)) return false;

	DISPID anPropertyID[1] = {0};
	BSTR asPropName[1];
	asPropName[0] = SysAllocString( strProp );
	hr = pDisp->GetIDsOfNames(IID_NULL, asPropName, 1, 0, anPropertyID);
	SysFreeString(asPropName[0]);

	DISPPARAMS pp = {0};
	hr = pDisp->Invoke(anPropertyID[0], IID_NULL, 0, DISPATCH_PROPERTYGET, 
		&pp, pVar, NULL, NULL);

	if (pDisp) 
		pDisp->Release();

	return SUCCEEDED(hr);
}



bool COcxSite::InvokeMethod( const wchar_t* strMethodName, DISPPARAMS *pDispParams )
{
	if( _pUnkObject == nullptr )
		return false;

	IDispatch *pDisp = NULL;
	HRESULT hr = _pUnkObject->QueryInterface(IID_IDispatch,(void**)&pDisp);
	if (FAILED(hr)) return false;

	DISPID anPropertyID[1] = {0};
	BSTR asPropName[1];
	asPropName[0] = SysAllocString( strMethodName );
	hr = pDisp->GetIDsOfNames(IID_NULL, asPropName, 1, 0, anPropertyID);
	SysFreeString(asPropName[0]);

	//VARIANT vps[1];
	//vps[0].vt = VT_BSTR;
	//vps[0].bstrVal = SysAllocString(L"192.168.0.99");

	//DISPPARAMS params = { vps, NULL, 1, 0 };
	//VARIANT res;
	//res.vt = VT_EMPTY;

	hr = pDisp->Invoke(anPropertyID[0], IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, 
		pDispParams, NULL, NULL, NULL);

	if (pDisp) 
		pDisp->Release();

	return SUCCEEDED(hr);
}

bool COcxSite::AttachEventHost( IDispatch *pDisp, const IID iidEvent, bool bAttach )
{
	if( _pUnkObject == nullptr )
		return false;

	HRESULT hr;

	IConnectionPointContainer  *pCPC=NULL;
	hr = _pUnkObject->QueryInterface( IID_IConnectionPointContainer,(void **)&pCPC);
	if (FAILED(hr)) return false;

	IConnectionPoint	*pCP=NULL;
	hr = pCPC->FindConnectionPoint( iidEvent, &pCP );

	DWORD	dwCookie = 0;
	if( bAttach )
		hr = pCP->Advise( pDisp, &dwCookie );
	else
		hr = pCP->Unadvise( dwCookie );

	return SUCCEEDED(hr);
}

