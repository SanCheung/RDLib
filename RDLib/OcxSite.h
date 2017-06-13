#pragma once

class RDLIB_API COcxSite	: public IOleClientSite
							, public IOleInPlaceSite
							, public IOleInPlaceFrame
{
public:
	COcxSite();
	virtual ~COcxSite(void);

	static HRESULT	CreateOcx( HWND hParentWnd, const CLSID cls, IUnknown **ppCtrl, IUnknown **ppContainer );
	static HRESULT	CreateOcx( HWND hParentWnd, const wchar_t* strProgID, IUnknown **ppCtrl, IUnknown **ppContainer );

	bool	Create( HWND hParentWnd, const wchar_t* strProgID );
	bool	Create( HWND hParentWnd, const CLSID cls );
	bool	Resize( LPRECT lprc );
	void	ReleaseAll();

	// DISPATCH_PROPERTYGET
	bool	SetProperty( const wchar_t* strProp, DISPPARAMS *pDispValue );
	bool	GetProperty( const wchar_t* strProp, VARIANT *pVar );

	bool	InvokeMethod( const wchar_t* strMethodName, DISPPARAMS *pDispParams );
	bool	AttachEventHost( IDispatch *pDisp, const IID iidEvent, bool bAttach );

private:
	ULONG	m_refCnt;
	HWND	m_hParentWnd;

	// all interface
	IUnknown	*_pUnkObject;
	//IUnknown	*_pUnkContainer;

public:
	//IUnknown
	STDMETHOD_(ULONG,AddRef())	{ return ++m_refCnt; }
	STDMETHOD_(ULONG,Release()) 
	{
		if (--m_refCnt == 0)
		{
			delete this;
			return 0;
		}

		return m_refCnt;
	}

	STDMETHOD(QueryInterface(REFIID riid,void **ppvObject))
	{
		*ppvObject = NULL;
		if ( IID_IUnknown == riid)
		{
			*ppvObject = (IUnknown*)(IOleInPlaceSite*)this;
		}
		else if (IID_IOleClientSite == riid)
		{
			*ppvObject = (IOleClientSite*)this;
		}
		//else if (IID_IOleWindow == riid)
		//{
		//	*ppvObject = (IOleWindow*)this;
		//}
		else if (IID_IOleInPlaceSite == riid)
		{
			*ppvObject = (IOleInPlaceSite*)this;
		}
		else if (IID_IOleInPlaceFrame == riid)
		{
			*ppvObject = (IOleInPlaceFrame*)this;
		}
		else
			return E_NOINTERFACE;

		AddRef();
		return S_OK;
	}

	//IOleControlSite
	STDMETHOD(SaveObject())							
	{ return E_NOTIMPL; }
	STDMETHOD(GetMoniker(DWORD,DWORD,IMoniker**))	
	{ return E_NOTIMPL; }
	STDMETHOD(GetContainer(IOleContainer **ppContainer)) 
	{ return E_NOTIMPL; }
	STDMETHOD(ShowObject())
	{ return S_OK; }
	STDMETHOD(OnShowWindow(BOOL bShow))
	{ return S_OK; }
	STDMETHOD(RequestNewObjectLayout())
	{ return S_OK; }


	// IOleInPlaceUIWindow
	STDMETHOD(InsertMenus( HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths))
	{ return S_OK; }

	STDMETHOD(SetMenu( HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject))
	{ return S_OK; }

	STDMETHOD(RemoveMenus(HMENU hmenuShared))
	{ return S_OK; }

	STDMETHOD(SetStatusText( LPCOLESTR pszStatusText))
	{ return S_OK; }

	STDMETHOD(EnableModeless( BOOL fEnable))
	{ return S_OK; }

	STDMETHOD(TranslateAccelerator( LPMSG lpmsg, WORD wID))
	{ return S_OK; }

	STDMETHOD( GetBorder( LPRECT lprectBorder))
	{ return S_OK; }

	STDMETHOD( RequestBorderSpace( LPCBORDERWIDTHS pborderwidths))
	{ return S_OK; }

	STDMETHOD( SetBorderSpace( LPCBORDERWIDTHS pborderwidths))
	{ return S_OK; }

	STDMETHOD( SetActiveObject( IOleInPlaceActiveObject *pActiveObject, LPCOLESTR pszObjName))
	{ return S_OK; }

	//IOleWindow
	STDMETHOD(GetWindow(HWND * pHwnd))
	{
		*pHwnd = m_hParentWnd;
		return S_OK;
	}
	STDMETHOD(ContextSensitiveHelp(BOOL bEnterMode))
	{ return S_OK; }


	//IOleInPlaceSite
	STDMETHOD(GetWindowContext( IOleInPlaceFrame **ppFrame,
		IOleInPlaceUIWindow **ppDoc,
		LPRECT lprcPosRect,
		LPRECT lprcClipRect,
		LPOLEINPLACEFRAMEINFO lpFrameInfo))
	{
		// Set pointer to frame
		QueryInterface( IID_IOleInPlaceFrame, (LPVOID*)ppFrame );

		GetClientRect( m_hParentWnd, lprcPosRect );
		GetClientRect( m_hParentWnd, lprcClipRect );

		// Fill in frame info
		lpFrameInfo->fMDIApp = FALSE;
		lpFrameInfo->hwndFrame = m_hParentWnd;
		lpFrameInfo->haccel = NULL;
		lpFrameInfo->cAccelEntries = 0;

		return S_OK;
	}

	STDMETHOD(CanInPlaceActivate())
	{ return S_OK; }
	STDMETHOD(OnInPlaceActivate())
	{ return S_OK; }
	STDMETHOD(OnUIActivate())
	{ return S_OK; }
	STDMETHOD(Scroll(SIZE scrollSize))
	{ return S_OK; }
	STDMETHOD(OnUIDeactivate(BOOL bUndoable))
	{ return S_OK; }
	STDMETHOD(OnInPlaceDeactivate())
	{ return S_OK; }
	STDMETHOD(DiscardUndoState())
	{ return S_OK; }
	STDMETHOD(DeactivateAndUndo())
	{ return S_OK; }
	STDMETHOD(OnPosRectChange(LPCRECT lprcPosRect))
	{ return S_OK; }

	STDMETHOD(SetMenu( IOleInPlaceFrame * This, 
		HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject))
	{ return S_OK; }

	STDMETHOD( RemoveMenus( IOleInPlaceFrame * This, HMENU hmenuShared))
	{ return S_OK; }
};

