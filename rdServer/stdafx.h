// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


#include <Winsock2.h>
#pragma comment( lib, "ws2_32.lib" )

// ATLTRACE
#include <atlbase.h>


#define NOCOPY_CLASS( className ) private: className( const className & ) {} className& operator= ( const className& ) {}