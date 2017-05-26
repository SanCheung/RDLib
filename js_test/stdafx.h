// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <atltrace.h>
#include <atldef.h>
#include <objidl.h>
#pragma comment( lib, "atlsd.lib" )

#include <vector>
#include <map>
#include <memory>
#include <list>
using namespace std;

#include "v8.h"
using namespace v8;
#pragma comment( lib, "v8.lib" )