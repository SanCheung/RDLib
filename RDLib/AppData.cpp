#include "StdAfx.h"
#include "AppData.h"

#include "../other/xzip_static/XUnzip.h"
#pragma comment( lib, "xzip_static.lib" )

#include "PaintManagerUI.h"


HINSTANCE CAppData::m_hInstance = NULL;
HINSTANCE CAppData::m_hResourceInstance = NULL;
HINSTANCE CAppData::m_ThisDllModule = NULL;

CStringW	CAppData::m_pStrResourcePath;
CStringW	CAppData::m_pStrResourceZip;
bool		CAppData::m_bCachedResourceZip = false;
HANDLE		CAppData::m_hResourceZip = NULL;

short CAppData::m_H = 180;
short CAppData::m_S = 100;
short CAppData::m_L = 100;

CStdPtrArray CAppData::m_arPreMessages;
CStdPtrArray CAppData::m_arPlugins;

CAppData::CAppData()
{

}

CAppData::~CAppData()
{
	m_arPreMessages.Remove(m_arPreMessages.Find(this));
}

HINSTANCE CAppData::GetInstance()
{
	return m_hInstance;
}

CStringW CAppData::GetInstancePath()
{
	if( m_hInstance == NULL ) return _T('\0');

	TCHAR tszModule[MAX_PATH + 1] = { 0 };
	::GetModuleFileName(m_hInstance, tszModule, MAX_PATH);
	CStringW sInstancePath = tszModule;
	int pos = sInstancePath.ReverseFind(_T('\\'));
	if( pos >= 0 ) sInstancePath = sInstancePath.Left(pos + 1);
	return sInstancePath;
}

//CStringW CAppData::GetCurrentPath()
//{
//	TCHAR tszModule[MAX_PATH + 1] = { 0 };
//	::GetCurrentDirectory(MAX_PATH, tszModule);
//	return tszModule;
//}

HINSTANCE CAppData::GetResourceDll()
{
	if( m_hResourceInstance == NULL ) return m_hInstance;
	return m_hResourceInstance;
}

const CStringW& CAppData::GetResourcePath()
{
	return m_pStrResourcePath;
}

const CStringW& CAppData::GetResourceZip()
{
	return m_pStrResourceZip;
}

bool CAppData::IsCachedResourceZip()
{
	return m_bCachedResourceZip;
}

HANDLE CAppData::GetResourceZipHandle()
{
	return m_hResourceZip;
}

void CAppData::SetInstance(HINSTANCE hInst)
{
	m_hInstance = hInst;
}

//void CAppData::SetCurrentPath(LPCTSTR pStrPath)
//{
//	::SetCurrentDirectory(pStrPath);
//}

void CAppData::SetResourceDll(HINSTANCE hInst)
{
	m_hResourceInstance = hInst;
}

void CAppData::SetResourcePath(LPCTSTR pStrPath)
{
	m_pStrResourcePath = pStrPath;
	if( m_pStrResourcePath.IsEmpty() ) 
		return;

	TCHAR cEnd = m_pStrResourcePath.GetAt(m_pStrResourcePath.GetLength() - 1);
	if( cEnd != _T('\\') && cEnd != _T('/') ) 
		m_pStrResourcePath += _T('\\');
}

void CAppData::SetResourceZip(LPVOID pVoid, unsigned int len)
{
	if( m_pStrResourceZip == _T("membuffer") )
		return;

	if( m_bCachedResourceZip && m_hResourceZip != NULL ) 
	{
		CloseZip((HZIP)m_hResourceZip);
		m_hResourceZip = NULL;
	}

	m_pStrResourceZip = _T("membuffer");
	m_bCachedResourceZip = true;
	if( m_bCachedResourceZip ) 
		m_hResourceZip = (HANDLE)OpenZip(pVoid, len, 3);
}

void CAppData::SetResourceZip(LPCTSTR pStrPath, bool bCachedResourceZip)
{
	if( m_pStrResourceZip == pStrPath && m_bCachedResourceZip == bCachedResourceZip ) return;
	if( m_bCachedResourceZip && m_hResourceZip != NULL ) 
	{
		CloseZip((HZIP)m_hResourceZip);
		m_hResourceZip = NULL;
	}

	m_pStrResourceZip = pStrPath;
	m_bCachedResourceZip = bCachedResourceZip;
	if( m_bCachedResourceZip )
	{
		CStringW sFile = CAppData::GetResourcePath();
		sFile += CAppData::GetResourceZip();
		m_hResourceZip = (HANDLE)OpenZip((void*)sFile.GetData(), 0, 2);
	}
}

void CAppData::GetHSL(short* H, short* S, short* L)
{
	*H = m_H;
	*S = m_S;
	*L = m_L;
}

void CAppData::SetHSL(bool bUseHSL, short H, short S, short L)
{
	if( H == m_H && S == m_S && L == m_L ) return;
	m_H = CLAMP(H, 0, 360);
	m_S = CLAMP(S, 0, 200);
	m_L = CLAMP(L, 0, 200);
	for( int i = 0; i < m_arPreMessages.GetSize(); i++ ) 
	{
		CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(m_arPreMessages[i]);
		if( pManager != NULL && pManager->GetRoot() != NULL )
			pManager->GetRoot()->Invalidate();
	}
}

void CAppData::ReloadSkin()
{
	for( int i = 0; i < m_arPreMessages.GetSize(); i++ ) {
		CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(m_arPreMessages[i]);
		pManager->ReloadAllImages();
	}
}

bool CAppData::LoadPlugin(LPCTSTR pstrModuleName)
{
	ASSERT( !::IsBadStringPtr(pstrModuleName,-1) || pstrModuleName == NULL );
	if( pstrModuleName == NULL ) 
		return false;

	HMODULE hModule = ::LoadLibrary(pstrModuleName);
	if( hModule != NULL )
	{
		LPCREATECONTROL lpCreateControl = (LPCREATECONTROL)::GetProcAddress(hModule, "CreateControl");
		if( lpCreateControl != NULL )
		{
			if( m_arPlugins.Find(lpCreateControl) >= 0 ) 
				return true;

			m_arPlugins.Add(lpCreateControl);
			return true;
		}
	}
	return false;
}

CStdPtrArray* CAppData::GetPlugins()
{
	return &m_arPlugins;
}

void CAppData::MessageLoop()
{
	MSG msg = { 0 };
	while( ::GetMessage(&msg, NULL, 0, 0) ) 
	{
		if( ::IsWindow(msg.hwnd) && !CAppData::TranslateMessage(&msg) ) 
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}

bool CAppData::TranslateMessage(const LPMSG pMsg)
{
	// Pretranslate Message takes care of system-wide messages, such as
	// tabbing and shortcut key-combos. We'll look for all messages for
	// each window and any child control attached.
	HWND hWndParent = ::GetParent(pMsg->hwnd);
	UINT uStyle = GetWindowStyle(pMsg->hwnd);
	LRESULT lRes = 0;
	for( int i = 0; i < m_arPreMessages.GetSize(); i++ ) 
	{
		CPaintManagerUI* pT = static_cast<CPaintManagerUI*>(m_arPreMessages[i]);        
		HWND hTempParent = hWndParent;
		while(hTempParent)
		{
			if(pMsg->hwnd == pT->GetPaintWindow() || (hTempParent == pT->GetPaintWindow() && ((uStyle & WS_CHILD) != 0)) )
			{
				if (pT->TranslateAccelerator(pMsg))
					return true;

				if( pT->PreMessageHandler(pMsg->message, pMsg->wParam, pMsg->lParam, lRes) ) 
					return true;

				return false;
			}
			hTempParent = GetParent(hTempParent);
		}
	}
	return false;
}

void CAppData::Term()
{
	if( m_bCachedResourceZip && m_hResourceZip != NULL )
	{
		CloseZip((HZIP)m_hResourceZip);
		m_hResourceZip = NULL;
	}
}