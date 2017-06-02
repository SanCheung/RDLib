#pragma once

#include "WindowWnd.h"


class RDLIB_API CAppData
{
public:
	CAppData();
	virtual ~CAppData();

	//	主调用入口
public:
	static void		SetInstance( HINSTANCE hInst );
	static void		MessageLoop();
	static bool		TranslateMessage(const LPMSG pMsg);
	static void		Term();

	static HINSTANCE GetInstance();
	static CStringW GetInstancePath();

	//static CStringW GetCurrentPath();
	//static void SetCurrentPath(LPCTSTR pStrPath);

	static HINSTANCE GetResourceDll();
	static void SetResourceDll(HINSTANCE hInst);
	
	static const CStringW& GetResourcePath();
	static void SetResourcePath(LPCTSTR pStrPath);

	static const CStringW& GetResourceZip();
	static bool IsCachedResourceZip();
	static HANDLE GetResourceZipHandle();


	static void SetResourceZip(LPVOID pVoid, unsigned int len);
	static void SetResourceZip(LPCTSTR pstrZip, bool bCachedResourceZip = false);
	static void GetHSL(short* H, short* S, short* L);
	static void SetHSL(bool bUseHSL, short H, short S, short L); // H:0~360, S:0~200, L:0~200 
	static void ReloadSkin();
	static bool LoadPlugin(LPCTSTR pstrModuleName);
	//static CStdPtrArray* GetPlugins();

	static void SetThisDllInstance(HINSTANCE hInstance)
	{
		m_ThisDllModule = hInstance;
	}
	static HINSTANCE GetThisDllInstance()
	{
		return m_ThisDllModule;
	}

protected:
	static HINSTANCE m_hInstance;
	static HINSTANCE m_hResourceInstance;
	static HINSTANCE m_ThisDllModule;

	static CStringW m_pStrResourcePath;
	static CStringW m_pStrResourceZip;
	static bool m_bCachedResourceZip;
	static HANDLE m_hResourceZip;

	static short m_H;
	static short m_S;
	static short m_L;

public:
	static vectorv	m_arPreMessages;
	static setv		m_arPlugins;

	static void RemovePreMsg( void *pPM );

};

