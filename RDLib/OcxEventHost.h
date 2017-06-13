#pragma once

//#include "tpviewer.tlh"
#include <atltrace.h>
#pragma comment(lib,"atlsd.lib")

//83AF7B37-FA62-44FE-8A83-9AD7B5ADC7B8
const IID IID_TPV_EVENT= { 0x83AF7B37, 0xFA62, 0x44FE, { 0x8A, 0x83, 0x9A, 0xD7, 0xB5, 0xAD, 0xC7, 0xB8 } };

class COcxEventHost : public IDispatch //tpViewerLib::_DtpViewerEvents
{
public:
	COcxEventHost(void) {}
	virtual ~COcxEventHost(void) {}

public:
	ULONG m_dwRef;

	STDMETHOD_(ULONG,AddRef)()	 { return ++m_dwRef; }
	STDMETHOD_(ULONG,Release)()
	{
		LONG lRef = --m_dwRef;
		if( lRef == 0 ) delete this;
			return lRef;
	}
	STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObject)
	{
		*ppvObject = NULL;
		if( riid == IID_TPV_EVENT || riid == IID_IDispatch )	
			//*ppvObject = static_cast<tpViewerLib::_DtpViewerEvents*>(this);
			*ppvObject = static_cast<IDispatch*>(this);

		return S_OK;
	}


	STDMETHOD( GetTypeInfoCount( UINT *pctinfo ) ) { return S_OK; }
	STDMETHOD( GetTypeInfo( UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo))
	{ return S_OK; }

	STDMETHOD( GetIDsOfNames( REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId))
	{ return S_OK; }

	STDMETHOD( Invoke( DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
		DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr))
	{
		if( dispIdMember == DISPID_CLICK )
		{
			//Click();
			ATLTRACE( L"Click\n");
		}
		else if( 2 == dispIdMember )
		{
			ATLTRACE( L"%d\n", pDispParams->rgvarg[0].lVal );
		}

		return S_OK;
	}

	//HRESULT Click()
	//{
	//	return S_OK;
	//}

	//HRESULT SizeChanged ( long cx, long cy )
	//{
	//	return S_OK;
	//}
};

