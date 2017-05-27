// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <atltrace.h>
#include <atldef.h>
#include <objidl.h>
#pragma comment( lib, "atlsd.lib" )


//#include "v8.h"
//using namespace v8;
//#pragma comment( lib, "v8.lib" )

#include "macros.h"


#include "XdBaseLib.h"
#include "RdLib.h"

//#include "XdLibs.h"
#pragma comment( lib, "XdBase.lib" )
#pragma comment( lib, "RDLib.lib" )

#define	DEL(p)	{delete p; p=NULL;}
#define	DELX(p)	{delete[] p; p=NULL;}

#include <GdiPlus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;




#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif