#pragma once

#ifdef RDLIB_EXPORTS
#define RDLIB_API __declspec(dllexport)
#else
#define RDLIB_API __declspec(dllimport)
#endif

#define	EQUSTR( s1, s2 )			_tcscmp(s1,s2)==0
#define EQUSTR_TRUE( s )			_tcscmp(s, L"true")==0

// all system .h
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stddef.h>
#include <richedit.h>
#include <tchar.h>
#include <assert.h>
#include <crtdbg.h>
#include <malloc.h>
#include <objbase.h>
#include <olectl.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#define lengthof(x) (sizeof(x)/sizeof(*x))
#define MAX max
#define MIN min
#define CLAMP(x,a,b) (MIN(b,MAX(a,x)))

#include <vector>
#include <map>
#include <memory>
#include <list>
using namespace std;

template class RDLIB_API allocator<void*>;
template class RDLIB_API vector<void*>;
//template class RDLIB_API map<wstring, void*>;

#include "XdBaseLib.h"
#pragma comment( lib, "XdBase.lib" )

// all lib .h
#include "StringPtrMap.h"

#include "rddefine.h"

#include "AppData.h"
#include "WindowWnd.h"

#include "PaintManagerUI.h"

#include "UIDelegate.h"
#include "UIRender.h"

#include "UIControl.h"
#include "UIMarkup.h"
#include "ScrollBarUI.h"
#include "UIContainer.h"
#include "LabelUI.h"
#include "UIEdit.h"
#include "UIList.h"
#include "UICombo.h"
#include "ButtonUI.h"
#include "TextUI.h"
#include "ProgressUI.h"
#include "CheckBoxUI.h"
#include "OptionUI.h"

#include "UIDlgBuilder.h"
#include "DialogBase.h"