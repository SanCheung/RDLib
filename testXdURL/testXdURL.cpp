// testXdURL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../XdURL/XdURLlib.h"
#pragma comment( lib, "XdURL.lib" )
//
//void showMaps2s( const maps2s &m )
//{
//	for( auto it = m.begin(); it != m.end(); it++ )
//	{
//		printf( "%s:%s\n", it->first.c_str(), it->second.c_str() );
//	}
//}
//
//int fun()
//{
//	return new char - "";
//}
//
//int test_xdurl_dll()
//{
//	string		url = "http://127.0.0.1:10008/interface/GetMeetingInfo";
//
//	maps2s		m;
//	urlGetInfo( url, m );
//	showMaps2s( m );
//
//
//	string		url2 = "http://127.0.0.1:10008/interface/GetFileList";
//	vector<maps2s> as;
//	urlGetArray( url2, as );
//
//	int n = (int)as.size();
//	for( int i = 0; i < n; i++ )
//	{
//		printf( "%d\n", i+1 );
//		showMaps2s( as[i] );
//		printf( "\n" );
//	}
//}

#include <windows.h>
//#pragma comment( lib, "advapi32.lib" )
//::EncryptFile( L"E:\\table2.jpg" );
//::DecryptFile( L"E:\\table2.jpg", 0 );

#include "Cryp.h"

const char *key = "1234tyuiasdf0987";

int	enc( wchar_t* file1, wchar_t* file2 )
{
	HANDLE h1 = ::CreateFile( file1, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if( INVALID_HANDLE_VALUE == h1 )
		return 1;

	HANDLE h2 = ::CreateFile( file2, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if( INVALID_HANDLE_VALUE == h2 )
		return 2;

	U32  k[4];
	memcpy( k, key, 16 );

	DWORD dwRead, dwWritten;

	DWORD dw = GetFileSize( h1, NULL );

	WriteFile( h2, &dw, 4, &dwWritten, NULL );

	while( 1 )
	{
		U32	 v[2] = {0};
		BOOL bOK = ReadFile( h1, &v, 8, &dwRead, NULL );

		CCryp::tea_enc( k, v );

		WriteFile( h2, &v, 8, &dwWritten, NULL );
		if( dwRead < 8 )
			break;
	}

	CloseHandle( h1 );
	CloseHandle( h2 );

	return 0;
}

int	dec( wchar_t* file1, wchar_t* file2 )
{
	HANDLE h1 = ::CreateFile( file1, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if( INVALID_HANDLE_VALUE == h1 )
		return 1;

	HANDLE h2 = ::CreateFile( file2, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if( INVALID_HANDLE_VALUE == h2 )
		return 2;

	U32  k[4];
	memcpy( k, key, 16 );

	DWORD	dwRead, dwWritten;

	DWORD	dwSize;
	ReadFile( h1, &dwSize, 4, &dwWritten, NULL );

	while( 1 )
	{
		U32	 v[4] = {0};
		BOOL bOK = ReadFile( h1, &v, 8, &dwRead, NULL );

		CCryp::tea_dec( k, v );

		if( dwSize < 8 )
		{
			WriteFile( h2, &v, dwSize, &dwWritten, NULL );
			break;
		}
		else
		{
			WriteFile( h2, &v, 8, &dwWritten, NULL );
			dwSize -= 8;
		}
	}

	CloseHandle( h1 );
	CloseHandle( h2 );
	return 0;
}


// curl
#define BUILDING_LIBCURL
#define CURL_STATICLIB
#include "../curl/include/curl/curl.h"

#pragma comment( lib, "ws2_32.lib" )
#pragma comment( lib, "wldap32.lib")

#ifdef _DEBUG
#pragma comment( lib, "../curl/LIB Debug/libcurld.lib" )
#else
#pragma comment( lib, "../curl/LIB Release/libcurl.lib" )
#endif

size_t writeToString(char *buffer,size_t size, size_t nitems,void *outstream)
{
	buffer[size*nitems] = 0;

	string	*strOut = (string *)outstream;
	strOut->append( buffer );

	return size*nitems;
}


int test_curl()
{
	string		ret;

	// 访问接口
	CURL* pCurl = curl_easy_init();

	string		url = "http://127.0.0.1:5000/index?name=zqh";

	curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str() );
	curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &ret );
	curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, writeToString );

	CURLcode res = curl_easy_perform(pCurl);
	//curl_easy_cleanup(pCurl);

	if( CURLE_OK!=res && CURLE_RECV_ERROR!=res )
		return -1;

	url = "http://127.0.0.1:5000/list";
	ret.clear();

	curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str() );
	curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &ret );

	CURLcode rc = curl_easy_perform(pCurl);
	curl_easy_cleanup(pCurl);

	return 0;
}

int test_xdurldll()
{
	string		ret;

	string		url = "http://127.0.0.1:5000/index?name=zqh";
	urlReturn( url, ret );

	ret.clear();
	url = "http://127.0.0.1:5000/list";
	urlReturn( url, ret );

	//string		url = "http://127.0.0.1:10008/interface/GetMeetingInfo";

	//maps2s		m;
	//urlGetInfo( url, m );
	//showMaps2s( m );


	//string		url2 = "http://127.0.0.1:10008/interface/GetFileList";
	//vector<maps2s> as;
	//urlGetArray( url2, as );

	//int n = (int)as.size();
	//for( int i = 0; i < n; i++ )
	//{
	//	printf( "%d\n", i+1 );
	//	showMaps2s( as[i] );
	//	printf( "\n" );
	//}

	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	//test_curl();

	test_xdurldll();

	//wchar_t *file1 = L"f:\\社保.xlsx";
	//wchar_t *file2 = L"f:\\123.des";
	//wchar_t *file3 = L"f:\\社保222.xlsx";

	//enc( file1, file2 );
	//dec( file2, file3 );

	//HANDLE h1 = ::CreateFile( file1, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//HANDLE h2 = ::CreateFile( file2, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//HANDLE h3 = ::CreateFile( file3, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	//char *key = "1234tyuiasdf0987";

	//U32  k[4];
	//memcpy( k, key, 16 );

	//while( 1 )
	//{
	//	DWORD dwRead = 0;

	//	U32	 v[2] = {0};
	//	BOOL bOK = ReadFile( h1, &v, 8, &dwRead, NULL );

	//	CCryp::tea_enc( k, v );

	//	DWORD dwWritten = 0;
	//	WriteFile( h2, &v, dwRead, &dwWritten, NULL );

	//	CCryp::tea_dec( k, v );

	//	DWORD dwWritten3 = 0;
	//	WriteFile( h3, &v, dwRead, &dwWritten3, NULL );

	//	if( dwRead < 8 )
	//		break;
	//}

	//CloseHandle( h1 );
	//CloseHandle( h2 );
	//CloseHandle( h3 );
	//getchar();
	return 0;
}

