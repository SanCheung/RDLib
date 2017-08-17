#include "StdAfx.h"
#include "Helper.h"

// curl
#define BUILDING_LIBCURL
#define CURL_STATICLIB
#include "../curl/include/curl/curl.h"

#pragma comment( lib, "ws2_32.lib" )
#pragma comment( lib, "wldap32.lib")

#ifdef _DEBUG
#pragma comment( lib, "../curl/libcurld.lib" )
#else
#pragma comment( lib, "../curl/libcurl.lib" )
#endif

// json
#include "../json/json.h"
#pragma comment( lib, "libJson.lib")


CHelper::CHelper(void)
{
}


CHelper::~CHelper(void)
{
}


size_t CHelper::writeToString(char *buffer,size_t size, size_t nitems,void *outstream)
{
	buffer[size*nitems] = 0;

	string	*strOut = (string *)outstream;
	strOut->append( buffer );

	return size*nitems;
}




wstring CHelper::utf2Unicode( string str )
{
	int len = ::MultiByteToWideChar( CP_UTF8, 0, str.c_str(), str.size(), NULL, 0 );
	if( len <= 0 )
		return L"";

	wchar_t *ws = new wchar_t[len+1];
	::MultiByteToWideChar( CP_UTF8, 0, str.c_str(), str.size(), ws, len );
	*(ws+len) = 0;

	wstring		wstr = ws;
	delete[] ws;
	return wstr;
}

string CHelper::unicode2Mbs( wstring ws )
{
	int len = ::WideCharToMultiByte(CP_ACP, 0, ws.c_str(), ws.size(), 0, 0, NULL, NULL);
	if( len <= 0 )
		return "";

	char *s = new char[len+1];
	::WideCharToMultiByte(CP_ACP, 0, ws.c_str(), ws.size(), s, len, NULL, NULL);
	s[len]='\0';

	string		str = s;
	delete[] s;

	return str;
}

int CHelper::jsValue2Map( Json::Value jv, maps2s &m )
{
	char buffer[128] = {0};

	Json::Value::Members mem = jv.getMemberNames();
	for( auto it = mem.begin(); it != mem.end(); ++it )
	{
		string	strItem = *it;
		Json::Value	 v = jv[strItem];

		if( v.isString() )		m[strItem] = v.asString();
		else if( v.isBool() )	m[strItem] = v.asBool()?"true":"false";
		else if( v.isInt() || v.isUInt() )
		{
			itoa(v.asInt(), buffer, 10);
			m[strItem] = buffer;
		}
		else if( v.isDouble() )
		{
			sprintf( buffer, "%f", v.asDouble() );
			m[strItem] = buffer;
		}
		else
			m[strItem] = "";
	}

	return (int)m.size();
}

bool CHelper::curlDownload( string strURL, string strLocalFile, void *lpData, cbCurlProcess callback, int &nResCode )
{
	FILE* pFile = NULL;
	if( 0 != fopen_s( &pFile, (char *)strLocalFile.c_str(), "wb+" ) )
		return false;

	CURL* pCurl = curl_easy_init();

	curl_easy_setopt(pCurl, CURLOPT_URL, strURL.c_str() );

	curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, (void*)pFile );
	curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, write_data);
	//使用1.1
	curl_easy_setopt(pCurl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
	if( NULL != callback )
	{
		curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, FALSE); 
		curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, callback );
		curl_easy_setopt(pCurl, CURLOPT_PROGRESSDATA, lpData );
	}


	CURLcode res = curl_easy_perform(pCurl);

	nResCode = 0;
	if( CURLE_OK == res )
		curl_easy_getinfo( pCurl, CURLINFO_RESPONSE_CODE, &nResCode); 

	fclose(pFile);

	curl_easy_cleanup(pCurl);
	return CURLE_OK==res;
}

size_t CHelper::write_data( char *buffer,size_t size, size_t nitems,void *outstream )
{
	int written = fwrite(buffer, size, nitems, (FILE*)outstream);
	return written;
}

int CHelper::urlReturn( string strURL, string &strValue )
{
	// 访问接口
	CURL* pCurl = curl_easy_init();

	curl_easy_setopt(pCurl, CURLOPT_URL, strURL.c_str() );
	curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &strValue );
	curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, writeToString );

	CURLcode res = curl_easy_perform(pCurl);
	curl_easy_cleanup(pCurl);

	if( CURLE_OK!=res && CURLE_RECV_ERROR!=res )
		return -1;

	wstring	strW = utf2Unicode( strValue );
	string	str = unicode2Mbs( strW );

	if( str.empty() )
		return -2;

	strValue = str;
	return 0;
}


int CHelper::urlGetInfo( string strURL, map<string, string> &m )
{
	string		str;
	int		nRet = urlReturn( strURL, str );
	if( nRet != 0 )
		return nRet;

	Json::Value		jRoot;
	Json::Reader	jReader;
	if( !jReader.parse( str, jRoot ) )
		return -3;

	jsValue2Map( jRoot, m );
	return (int)m.size();
}

int CHelper::urlGetArray( string strURL, vector<maps2s> &as )
{
	string		str;
	int		nRet = urlReturn( strURL, str );
	if( nRet != 0 )
		return nRet;

	Json::Value		jRoot;
	Json::Reader	jReader;
	if( !jReader.parse( str, jRoot ) )
		return -3;

	if( !jRoot.isArray() )
		return -4;

	int n = jRoot.size();
	for( int i = 0; i < n; i++ )
	{
		Json::Value jv = jRoot[i];

		maps2s	m;
		jsValue2Map( jv, m );
		as.push_back( m );
	}

	return (int)as.size();
}

size_t CHelper::save_header(void *ptr, size_t size, size_t nmemb, void *data)
{
	return (size_t)(size * nmemb);
}

int CHelper::curlGetDownloadFileSize( string strURL )
{
	double len= 0.0;
	CURL *handle = curl_easy_init();
	curl_easy_setopt(handle, CURLOPT_URL, strURL.c_str() );
	curl_easy_setopt(handle, CURLOPT_HEADER, 1);    //只要求header头
	curl_easy_setopt(handle, CURLOPT_NOBODY, 1);    //不需求body
	curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, save_header);

	if (curl_easy_perform(handle) == CURLE_OK) 
	{
		if(CURLE_OK == curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &len))
		{
			;
		}
		else 
		{
			;//pirntf("curl_easy_getinfo failed!\n");
		}

	} 
	else 
	{
		len= -1;
	}

	return len;
}
