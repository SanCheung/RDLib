#include "StdAfx.h"
#include "DialogBase.h"


CDialogBase::CDialogBase(void)
{
}


CDialogBase::~CDialogBase(void)
{
}

UINT CDialogBase::GetClassStyle() const
{
	return CS_DBLCLKS;
}

CControlUI* CDialogBase::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CDialogBase::OnFinalMessage(HWND /*hWnd*/)
{
	paint_manager_.RemovePreMessageFilter(this);
	paint_manager_.RemoveNotifier(this);
	paint_manager_.ReapObjects(paint_manager_.GetRoot());
}

void CDialogBase::Init()
{}

LRESULT CDialogBase::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CDialogBase::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

//#if defined(WIN32) && !defined(UNDER_CE)
LRESULT CDialogBase::OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CDialogBase::OnNcCalcSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT CDialogBase::OnNcPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{	
	return 0;
}

LRESULT CDialogBase::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if( !::IsZoomed(*this) ) {
		RECT rcSizeBox = paint_manager_.GetSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}

	RECT rcCaption = paint_manager_.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(pt));		
			if( pControl && _tcsicmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
				_tcsicmp(pControl->GetClass(), _T("OptionUI")) != 0 /*&&
				_tcsicmp(pControl->GetClass(), _T("TextUI")) != 0 */)
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CDialogBase::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDUIRect rcWork = oMonitor.rcWork;
	//rcWork.Offset(-rcWork.left, -rcWork.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right - rcWork.left;;
	lpMMI->ptMaxSize.y		= rcWork.bottom - rcWork.top;

	bHandled = FALSE;
	return 0;
}

LRESULT CDialogBase::OnMouseWheel(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}
//#endif


LRESULT CDialogBase::OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CDialogBase::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CDialogBase::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CDialogBase::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CDialogBase::OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CDialogBase::OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CDialogBase::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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


LRESULT CDialogBase::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//if (wParam == SC_CLOSE){
	//	bHandled = TRUE;
	//	return 0;
	//}
//#if defined(WIN32) && !defined(UNDER_CE)
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed )
	{
	}
//#else
//	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
//#endif
	return lRes;
}


LRESULT CDialogBase::OnCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{	
	bHandled = FALSE;
	return 0;
}

//CStringW CDialogBase::GetSkinFolder()
//{
//	return CAppData::GetInstancePath() + _T("plc\\");
//}

LRESULT CDialogBase::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	//styleValue &= ~WS_CAPTION;
	//::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	//::SetWindowLong(*this, GWL_STYLE, styleValue );

	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
		rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

	paint_manager_.Init(m_hWnd);
	paint_manager_.AddPreMessageFilter(this);

	CDialogBuilder builder;

	//CAppData::SetResourcePath( GetSkinFolder() );

//#if USE(ZIP_SKIN)
//	tString tstrSkin = GetSkinFile();
//	if (paint_manager_.GetResourceZip().IsEmpty())
//	{
//#if USE(EMBEDED_RESOURCE)
//		HRSRC hResource = ::FindResource(paint_manager_.GetResourceDll(), MAKEINTRESOURCE(IDR_ZIPRES), _T("ZIPRES"));
//		if( hResource == NULL )
//			return 0L;
//		DWORD dwSize = 0;
//		HGLOBAL hGlobal = ::LoadResource(paint_manager_.GetResourceDll(), hResource);
//		if( hGlobal == NULL ) {
//#if defined(WIN32) && !defined(UNDER_CE)
//			FreeResource(hResource);
//#endif
//			return 0L;
//		}
//		dwSize = ::SizeofResource(paint_manager_.GetResourceDll(), hResource);
//		if( dwSize == 0 )
//			return 0L;
//		resource_zip_buffer_ = new BYTE[ dwSize ];
//		if (resource_zip_buffer_ != NULL)
//		{
//			::CopyMemory(resource_zip_buffer_, (LPBYTE)::LockResource(hGlobal), dwSize);
//		}
//#if defined(WIN32) && !defined(UNDER_CE)
//		::FreeResource(hResource);
//#endif
//		paint_manager_.SetResourceZip(resource_zip_buffer_, dwSize);
//#else
//		paint_manager_.SetResourceZip(kResourceSkinZipFileName, true);
//#endif
//}
//
//#else
	//tString tstrSkin = paint_manager_.GetResourcePath();
	//tstrSkin += GetSkinFile();
	CStringW tstrSkin = GetSkinFile();
//#endif

	CControlUI* pRoot = builder.Create( (LPCTSTR)tstrSkin, (UINT)0, this, &paint_manager_);
	if( pRoot == NULL )
	{
		MsgBox( L"XML½âÎöÊ§°Ü£¬Çë¼ì²é£¡" );
		return 0;
	}

	paint_manager_.AttachDialog(pRoot);
	paint_manager_.AddNotifier(this);

	Init();

	if( !(styleValue & WS_DLGFRAME) )
	{
		//Ìí¼ÓÒõÓ°
		CWndShadow::Initialize(CAppData::GetInstance());
		m_shadow.Create(m_hWnd);
		m_shadow.SetSize(2);
		m_shadow.SetPosition(1, 1);
	}

	return 0;
}

LRESULT CDialogBase::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CDialogBase::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_CREATE:			lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:			lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:		lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
//#if defined(WIN32) && !defined(UNDER_CE)
	//case WM_NCACTIVATE:		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	//case WM_NCCALCSIZE:		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	//case WM_NCPAINT:			lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	//case WM_GETMINMAXINFO:	lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEWHEEL:		lRes = OnMouseWheel(uMsg, wParam, lParam, bHandled); break;
//#endif
	case WM_SIZE:			lRes = OnSize(uMsg, wParam, lParam, bHandled); break;	
	case WM_SYSCOMMAND:		lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	case WM_COMMAND:		lRes = OnCommand(uMsg, wParam, lParam, bHandled); break;
	case WM_KEYDOWN:		lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
	case WM_KILLFOCUS:		lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
	case WM_SETFOCUS:		lRes = OnSetFocus(uMsg, wParam, lParam, bHandled); break;
	case WM_LBUTTONUP:		lRes = OnLButtonUp(uMsg, wParam, lParam, bHandled); break;
	case WM_LBUTTONDOWN:	lRes = OnLButtonDown(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEMOVE:		lRes = OnMouseMove(uMsg, wParam, lParam, bHandled); break;
	case WM_DRAWITEM:		
		{
			LPDRAWITEMSTRUCT pDraw = LPDRAWITEMSTRUCT(lParam);
			HWND hChild = NULL;
			if (pDraw->CtlType != ODT_MENU)	// not from a menu
				hChild = pDraw->hwndItem;
			else if (::IsWindow(pDraw->hwndItem))// Status bar control sends this message with type set to ODT_MENU
					hChild = pDraw->hwndItem;

			if (hChild){
				::SendMessage(hChild, OCM__BASE+WM_DRAWITEM, wParam, lParam);				
			}			
		}
		break;
	case WM_MEASUREITEM:
		{	
			LPMEASUREITEMSTRUCT pMeasure = (LPMEASUREITEMSTRUCT)lParam;
			HWND hChild = NULL;
			if(pMeasure->CtlType != ODT_MENU)	// not from a menu
				hChild = GetDlgItem(*this, pMeasure->CtlID);
			
			if (hChild)	{
				::SendMessage(hChild, OCM__BASE+WM_MEASUREITEM, wParam, lParam);				
			}	
		}
		break;
	case WM_NOTIFY:
		{
			LPNMHDR pNMHDR = (LPNMHDR)lParam;
			HWND hChild = pNMHDR->hwndFrom;
			if (hChild)
			{
				::SendMessage(hChild, OCM__BASE+WM_NOTIFY, wParam, lParam);
			}
		}
		break;
	default:				bHandled = FALSE; break;
	}
	if (bHandled) 
		return lRes;

	lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
	if (bHandled) 
		return lRes;

	if (paint_manager_.MessageHandler(uMsg, wParam, lParam, lRes)) 
		return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CDialogBase::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/)
{
	if (uMsg == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			return ResponseDefaultKeyEvent(wParam);
		default:
			break;
		}
	}
	return FALSE;
}

LRESULT CDialogBase::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{

	}
	else if (wParam == VK_ESCAPE)
	{
		int i = 0;
	}

	return FALSE;
}

void CDialogBase::Cleanup()
{
	//if (resource_zip_buffer_ != NULL)
	//{
	//	delete[] resource_zip_buffer_;
	//	resource_zip_buffer_ = NULL;
	//}
}

int CDialogBase::MsgBox( CStringW strInfo, CStringW strTitle, int nType /*= 0*/, int nButton /*= MB_OK */ )
{
	return MessageBox( m_hWnd, strInfo, strTitle, nType|nButton );
}

UINT CDialogBase::DoModal( HWND hWnd, CStringW strCaption, DWORD dwStyle )
{
	//Create( hWnd, strCaption, WS_POPUPWINDOW | WS_CAPTION, 0);
	Create( hWnd, strCaption, dwStyle, 0);
	CenterWindow();
	return ShowModal();
}

void CDialogBase::setEditText( CStringW strCtrl, CStringW strText )
{
	CEditUI *pCtrl = FindCtrl<CEditUI>( strCtrl );
	if( !pCtrl )
		return;

	pCtrl->SetText( strText );
}

CStringW CDialogBase::getEditText( CStringW strCtrl )
{
	CEditUI *pCtrl = FindCtrl<CEditUI>( strCtrl );
	if( !pCtrl )
		return L"";

	return pCtrl->GetText();
}

void CDialogBase::getEditText( CStringW strCtrl, CStringW &strText )
{
	CEditUI *pCtrl = FindCtrl<CEditUI>( strCtrl );
	if( !pCtrl )
		strText = L"";
	else
		strText = pCtrl->GetText();
}

void CDialogBase::setComboValue( CStringW strCtrl, int i )
{
	CComboUI *pCtrl = FindCtrl<CComboUI>( strCtrl );
	if( !pCtrl )
		return;

	pCtrl->SelectItem( i );
}

int CDialogBase::getComboValue( CStringW strCtrl )
{
	CComboUI *pCtrl = FindCtrl<CComboUI>( strCtrl );
	if( !pCtrl )
		return -1;

	return pCtrl->GetCurSel();
}

UINT CDialogBase::DoModalNoCaption( HWND hWnd )
{
	Create( hWnd, L"", WS_POPUPWINDOW, WS_EX_WINDOWEDGE );
	CenterWindow();
	return ShowModal();
}
