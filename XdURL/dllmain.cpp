// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "XdURLlib.h"
#include "helper.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


//extern "C" __declspec (dllexport) 
XDURL_API int urlReturn( string strURL, string &strValue )
{
	return CHelper::urlReturn( strURL, strValue );
}

XDURL_API int urlGetInfo( string strURL, maps2s &m )
{
	return CHelper::urlGetInfo( strURL, m );
}

XDURL_API int urlGetArray( string strURL, vector<maps2s> &as )
{
	return CHelper::urlGetArray( strURL, as );
}


XDURL_API int urlDownload( string strURL, string strFile )
{
	int nResCode = -1;
	CHelper::curlDownload( strURL, strFile, NULL, NULL, nResCode );
	return nResCode;
}
