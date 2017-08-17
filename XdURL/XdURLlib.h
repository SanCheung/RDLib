#pragma once;

#include <string>
#include <vector>
#include <map>
using namespace std;

typedef map<string, string> maps2s;

#ifdef XDURL_EXPORTS
#define XDURL_API __declspec(dllexport)
#else
#define XDURL_API __declspec(dllimport)

XDURL_API int urlReturn( string strURL, string &strValue );

XDURL_API int urlDownload( string strURL, string strFile );
XDURL_API int curlGetDownloadFileSize( string strURL );

// 获取信息
XDURL_API int urlGetInfo( string strURL, maps2s &m );

// 获取数组
XDURL_API int urlGetArray( string strURL, vector<maps2s> &as );

#include "maps2s_shell.h"
#endif

