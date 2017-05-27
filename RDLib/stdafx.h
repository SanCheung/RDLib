// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers


#pragma warning (disable : 4511) // copy operator could not be generated
#pragma warning (disable : 4512) // assignment operator could not be generated
#pragma warning (disable : 4702) // unreachable code (bugs in Microsoft's STL)
#pragma warning (disable : 4786) // identifier was truncated
#pragma warning (disable : 4996) // function or variable may be unsafe (deprecated)


//#include "v8.h"
//using namespace v8;
//#pragma comment( lib, "v8.lib" )
//#include "js_macros.h"

#include <vector>
using namespace std;



#pragma comment( lib, "comctl32.lib" ) 
#pragma comment( lib, "riched20.lib" )
#pragma comment( lib, "xzip_static.lib" )
#pragma comment( lib, "stimage_static.lib" )

#include "rdLib.h"
