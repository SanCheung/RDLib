#include "StdAfx.h"
#include "Helper.h"

// open dialoag
#include <CommDlg.h>

#include <shlobj.h>
#include <shlwapi.h>
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "Shlwapi.lib")

#include "../other/xzip_static/XUnzip.h"
#include "../other/xzip_static/XZip.h"
#pragma comment( lib, "xzip_static.lib" )

#include "Cryp.h"

namespace fblSystem
{


SECURITY_ATTRIBUTES CGlobalSecurityAttributes::s_SA;
SECURITY_DESCRIPTOR CGlobalSecurityAttributes::s_SD;
bool CGlobalSecurityAttributes::s_Initialized = false;

SECURITY_ATTRIBUTES CGlobalSecurityAttributes::instance()
{
	if (!s_Initialized)
	{
		InitializeSecurityDescriptor(&s_SD, SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(&s_SD, TRUE, NULL, FALSE);
		s_SA.nLength = sizeof(s_SA);
		s_SA.lpSecurityDescriptor = &s_SD;

		s_Initialized = true;
	}

	return s_SA;
}


CHelper::CHelper(void)
{
}


CHelper::~CHelper(void)
{
}

CStringW CHelper::GetAppdataFolder()
{
	WCHAR szBuff[MAX_PATH] = {0};

	LPITEMIDLIST	pidl;
	HRESULT			hr = SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
	if( SUCCEEDED(hr))
	{
		SHGetPathFromIDListW(pidl, szBuff);
		CoTaskMemFree(pidl);
	}	

	return szBuff;
}

CStringW CHelper::GetPLSSFolder()
{
	CStringW	strAppDataFolder = GetAppdataFolder();
	CStringW	str = strAppDataFolder + L"\\XDPlss";

	if( !PathFileExistsW(str) )
	{
		SECURITY_ATTRIBUTES	sa = CGlobalSecurityAttributes::instance();
		::CreateDirectory( str, &sa );
	}

	str += L"\\";
	return str;
}

//CStringW CHelper::GetMeetingClientFolder()
//{
//
//	CStringW	strAppDataFolder = GetAppdataFolder();
//	CStringW	str = strAppDataFolder + L"\\XDClient";
//
//	if( !PathFileExistsW(str) )
//	{
//		SECURITY_ATTRIBUTES	sa = CGlobalSecurityAttributes::instance();
//		::CreateDirectory( str, &sa );
//	}
//
//	str += L"\\";
//	return str;
//}

CStringW CHelper::GetHandWriterFolder()
{
	CStringW	strParentFolder = GetPLSSFolder();
	CStringW	str = strParentFolder + L"\\HandWriter";

	if( !PathFileExistsW(str) )
	{
		SECURITY_ATTRIBUTES	sa = CGlobalSecurityAttributes::instance();
		::CreateDirectory( str, &sa );
	}

	str += L"\\";
	return str;
}



CStringA CHelper::Md5( const CStringW &str )
{
	CStringA	strIn = str;

	CCryp	c;
	c.MD5Update( (unsigned char *)(char *)strIn, strIn.GetLength() );

	BYTE	out[32] = {0};
	c.MD5Final( out );

	CStringA strOut = ToAsciiString(out, 16);
	return strOut;
}

CStringA CHelper::ToAsciiString( unsigned char *in, int len )
{
	const char stable[] = "0123456789abcdef";

	CStringA	strOut = "";
	for( int i = 0; i < len; i++ )
	{
		U8 b = in[i];
		strOut += stable[b/16];
		strOut += stable[b%16];
	}

	return strOut;
}

CStringW CHelper::EncrypTea( CStringW str, CStringW strKey )
{
	CStringA	strA = str;
	CStringA	strA2 = strKey;
	char *key = (char *)strA2;

	int		nByteSize = str.GetLength();
	if( nByteSize % 8 )
		nByteSize = (nByteSize/8 + 1 )*8;

	char	*value = new char[nByteSize];
	memset( value, 0, nByteSize );
	strcpy( value, (char *)strA );

	U8		*buffer = new U8[nByteSize+8];
	memset( buffer, 0, nByteSize+8 );

	// 64bit 数据
	// 256bit Key
	U32		v[2] = {0};
	U32		k[8] = {0};
	memcpy( k, key, min(32, strlen(key)) );
	
	const int step = 8;

	int		valueLen = 0;
	int		bufferLen = 0;
	int		n = nByteSize;
	while( n > 0 )
	{
		memcpy( v, value+valueLen, step );
		CCryp::tea_enc( k, v );

		memcpy( buffer+bufferLen, v, step );
		bufferLen += step;
		valueLen += step;

		n -= step;
	}

	char *strBase64 = new char[nByteSize*2];
	CCryp::ToBase64( buffer, bufferLen, strBase64 );
	delete[] buffer;

	CStringW strOut = strBase64;
	delete[] strBase64;
	
	return strOut;
}

CStringW CHelper::DecrypTea( CStringW str, CStringW strKey )
{
	int nBase64Length = str.GetLength();
	CStringA	strA = str;

	int nInLength = nBase64Length*2/3;
	if( nInLength % 8 )
		return L"";

	char *value = new char[nInLength];
	//memset( value, 0, nInLength );
	CCryp::FromBase64( (U8 *)(char *)strA, nBase64Length, value );

	U8	*buffer = new U8[nInLength+8];
	memset( buffer, 0, nInLength+8 );

	CStringA	strA2 = strKey;
	char *key = (char *)strA2;

	// 64bit 数据
	// 256bit Key
	U32		v[2] = {0};
	U32		k[8] = {0};
	memcpy( k, key, min(32, strlen(key)) );

	const int step = 8;
	int		bufferLen = 0;
	int		decrpyLen = 0;
	while( nInLength > 0 )
	{
		memcpy( v, value+decrpyLen, step );
		CCryp::tea_dec( k, v );

		memcpy( buffer+bufferLen, v, step );
		bufferLen += step;
		decrpyLen += step;
		nInLength -= step;
	}
	delete[] value;
	*(buffer+bufferLen) = 0;

	CStringW strOut = (char *)buffer;
	delete[] buffer;
	return strOut;
}


int CHelper::string2Array( const CStringW &strIn, vector<int> &an )
{
	an.clear();
	if( strIn.IsEmpty() )
		return 0;

	CStringW	str = strIn;
	if( str.Right(1) != L";" )
		str += L";";

	int nPos = str.Find( L";" );
	CStringW strItem = str;
	while( nPos != -1 )
	{
		strItem = str.Left(nPos);
		an.push_back( strItem.ToInt() );

		str.Delete( 0, nPos+1 );
		nPos = str.Find( L";" );
	}

	return an.size();
}

int CHelper::string2Array( const CStringW &strIn, vector<CStringW> &as, const CStringW &sp )
{
	as.clear();
	if( strIn.IsEmpty() )
		return 0;

	CStringW	str = strIn;
	//if( str.Right(1) != L";" )
	//	str += L";";
	TCHAR ch = str[str.GetLength()-1];
	if( sp.Find(ch) == -1 )
		str += sp[0];


	//int nPos = str.Find( L";" );
	int nPos = str.FindOneOf( sp );
	CStringW strItem = str;
	while( nPos != -1 )
	{
		strItem = str.Left(nPos);
		as.push_back( strItem );

		str.Delete( 0, nPos+1 );
		nPos = str.FindOneOf( sp );
	}

	return as.size();
}

CStringW CHelper::array2String( const vector<int> &an, wchar_t sp )
{
	CStringW		strOut;

	int i;
	int n = (int)an.size();
	for( i = 0; i < n; i++ )
	{
		CStringW	strItem;
		strItem.Format( L"%d%c", an[i], sp );
		strOut += strItem;
	}

	return strOut;
}

CStringW CHelper::array2String( const vector<CStringW> &as, wchar_t sp )
{
	CStringW		strOut;

	int i;
	int n = (int)as.size();
	for( i = 0; i < n; i++ )
	{
		CStringW	strItem;
		strItem.Format( L"%s%c", as[i], sp );
		strOut += strItem;
	}

	return strOut;
}

bool CHelper::GetFileNameFromDialog( bool bSave, HWND hWnd, wchar_t *strFilter, CStringW &strFile )
{
	//HWND		hWnd = ::GetForegroundWindow();

	// 初始化OFN
	OPENFILENAME ofn = {0};

	//	static wchar_t szFilter[] = L"所有图片文件\0*.bmp;*.dib;*.jpg;*.jpeg;*.jpe;*.gif;*.tiff;*.png;*.ico\0 \
	//JPEG文件 (*.jpg;*.jpeg;*.jpe)\0*.jpg;*.jpeg;*.jpe\0 \
	//位图文件 (*.bmp;*.dib)\0*.bmp;*.dib\0 \
	//GIF (*.gif)\0*.gif\0 \
	//TIFF (*.tiff)\0*.tiff \
	//PNG (*.png)\0*.png \
	//ICO (*.ico)\0*.ico\0\0";


	ofn.lStructSize       = sizeof (OPENFILENAME) ;  
	ofn.hwndOwner         = hWnd;   
	ofn.lpstrFilter       = strFilter;  
	ofn.nMaxFile          = MAX_PATH;    
	ofn.nMaxFileTitle     = MAX_PATH;
	ofn.lpstrDefExt		  = L"*.*";

	static wchar_t		buf[MAX_PATH] = {0};
	memset( buf, 0, sizeof(wchar_t)*MAX_PATH );
	ofn.lpstrFile		= buf;

	BOOL bRet;

	if( bSave )
		bRet = GetSaveFileName( &ofn );
	else
		bRet = GetOpenFileName( &ofn );

	if( bRet )
	{
		CStringW	str = buf;
		strFile = str;
		return true;
	}

	return false;
}


int CHelper::dlgOpenMultipleFile( HWND hWnd, wchar_t *strFilter, vector<CStringW> &as )
{
	//	static wchar_t szFilter[] = L"所有图片文件\0*.bmp;*.dib;*.jpg;*.jpeg;*.jpe;*.gif;*.tiff;*.png;*.ico\0 \
	//JPEG文件 (*.jpg;*.jpeg;*.jpe)\0*.jpg;*.jpeg;*.jpe\0 \
	//位图文件 (*.bmp;*.dib)\0*.bmp;*.dib\0 \
	//GIF (*.gif)\0*.gif\0 \
	//TIFF (*.tiff)\0*.tiff \
	//PNG (*.png)\0*.png \
	//ICO (*.ico)\0*.ico\0\0";

	// 初始化OFN
	OPENFILENAME ofn = {0};
	ofn.lStructSize       = sizeof (OPENFILENAME) ;  
	ofn.hwndOwner         = hWnd;   
	ofn.lpstrFilter       = strFilter;  
	ofn.nMaxFile          = MAX_PATH;    
	ofn.nMaxFileTitle     = MAX_PATH;
	ofn.lpstrDefExt		  = L"*.*";

	static wchar_t		buf[MAX_PATH] = {0};
	memset( buf, 0, sizeof(wchar_t)*MAX_PATH );
	ofn.lpstrFile		= buf;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST  | OFN_ALLOWMULTISELECT;

	BOOL bRet = GetOpenFileName( &ofn );
	if( bRet )
	{
		wchar_t		*pstr = buf;
		if( 0 == *pstr )
			return 0;

		CStringW	strFolder = pstr;
		pstr += strFolder.GetLength()+1;

		if( 0 == *pstr )
		{
			as.push_back( strFolder );
			return 1;
		}

		while( *pstr )
		{
			CStringW str = pstr;
			as.push_back( strFolder + L"\\" + str );
			pstr += str.GetLength()+1;
		}

		return (int)as.size();
	}

	return 0;
}

bool CHelper::DoModelFontDialog( HWND hWnd, CStringW &strFontName, COLORREF &cr, int &fontSize, int &bUnderline, int &bItalic, int &bBold )
{
	CHOOSEFONT		cf = {0};

	cf.lStructSize = sizeof(cf);
	cf.hwndOwner = hWnd;
	cf.rgbColors = cr;
	//cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_EFFECTS | CF_SCREENFONTS;
	cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;

	LOGFONT		lf = {0};
	lf.lfUnderline = bUnderline;
	lf.lfItalic = bItalic;
	lf.lfWeight = bBold?700:300;

	lf.lfHeight = fontSize;
	_tcscpy( lf.lfFaceName, strFontName );

	cf.lpLogFont = &lf;

	BOOL bRet = ::ChooseFont( &cf );
	if( bRet )
	{
		strFontName = cf.lpLogFont->lfFaceName;
		cr = cf.rgbColors;
		fontSize = cf.lpLogFont->lfHeight;
		bUnderline = cf.lpLogFont->lfUnderline;
		bItalic = cf.lpLogFont->lfItalic;
		bBold = cf.lpLogFont->lfWeight==700?1:0;
	}

	return TRUE==bRet;
}

bool CHelper::DoModelFontDialog( HWND hWnd, LOGFONT &lf, COLORREF &cr )
{
	CHOOSEFONT		cf = {0};

	cf.lStructSize = sizeof(cf);
	cf.hwndOwner = hWnd;
	cf.rgbColors = cr;
	//cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_EFFECTS | CF_SCREENFONTS;
	cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;

	LOGFONT		lfNew = {0};
	memcpy( &lfNew, &lf, sizeof(LOGFONT));
	cf.lpLogFont = &lf;

	BOOL bRet = ::ChooseFont( &cf );
	if( bRet )
	{
		cr = cf.rgbColors;
		memcpy( &lf, cf.lpLogFont, sizeof(LOGFONT) );
	}

	return TRUE==bRet;
}

bool CHelper::string2Time( CStringW strTime, SYSTEMTIME &st, int &nHour, int &nMinute )
{
	if( strTime.IsEmpty() || strTime.GetLength() < 16 )
	{
		GetLocalTime( &st );
		nHour = st.wHour;
		nMinute = st.wMinute;
		return true;
	}

	CStringW	str = strTime;
	st.wYear = str.Left(4).ToInt();
	str.Delete(0, 5);

	st.wMonth = str.Left(2).ToInt();
	str.Delete(0, 3);

	st.wDay = str.Left(2).ToInt();
	str.Delete(0, 3);

	nHour = str.Left(2).ToInt();
	str.Delete(0, 3);

	nMinute = str.Left(2).ToInt();
	return true;
}

bool CHelper::string2Time( CStringW strTime, SYSTEMTIME &st )
{
	memset( &st, 0, sizeof(st) );

	CStringW		strTrim = L"\\/:*?\"<>|- ";
	for( int i = 0; i < strTrim.GetLength(); i++ )
		strTime.Remove( strTrim[i] );

	if( strTime.GetLength() != 14 )
		return false;

	CStringW	str = strTime;
	st.wYear = str.Left(4).ToInt();
	str.Delete(0, 4);

	st.wMonth = str.Left(2).ToInt();
	str.Delete(0, 2);

	st.wDay = str.Left(2).ToInt();
	str.Delete(0, 2);

	st.wHour = str.Left(2).ToInt();
	str.Delete(0, 2);

	st.wMinute = str.Left(2).ToInt();
	str.Delete(0, 2);

	st.wSecond = str.ToInt();
	return true;
}

CStringW CHelper::time2String( const SYSTEMTIME &st, int nHour, int nMinute )
{
	if( nHour < 0 )	nHour = 0;
	if( nHour >23 ) nHour = 23;

	if( nMinute < 0 ) nMinute = 0;
	if( nMinute > 59) nMinute = 59;

	CStringW		strTime;
	strTime.Format( L"%04d-%02d-%02d %02d:%02d", st.wYear, st.wMonth, st.wDay, nHour, nMinute );
	return strTime;
}


CStringW CHelper::nowString()
{
	SYSTEMTIME		st;
	GetLocalTime( &st );

	CStringW		strTime;
	strTime.Format( L"%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond );
	return strTime;
}


CStringW CHelper::nowString2()
{
	SYSTEMTIME		st;
	GetLocalTime( &st );

	CStringW		strTime;
	strTime.Format( L"%04d%02d%02d%02d%02d%02d", st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond );
	return strTime;
}



CStringW CHelper::GetCurrentPath()
{
	wchar_t		buf[512] = {0};
	GetModuleFileName( NULL, buf, 512 );

	CStringW	str = buf;
	int npos = str.ReverseFind( L'\\' );
	if( npos == -1 )
		return L"";

	str.Delete( npos, str.GetLength()-npos );
	return str;
}

CStringW CHelper::getFileName( const CStringW &strFile )
{
	int npos = strFile.ReverseFind( '\\' );
	if( -1 == npos )
		return L"";

	int nLength = strFile.GetLength();
	return strFile.Right( nLength - npos - 1 );
}

UINT CHelper::getFileSize( const CStringW &strFile )
{
	HANDLE hFile = ::CreateFile( strFile, GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	
	if( INVALID_HANDLE_VALUE == hFile )
		return 0;
	
	DWORD dwSize = ::GetFileSize(hFile, NULL);
	CloseHandle( hFile );

	return dwSize;
}

CStringW CHelper::formatByteSize( UINT uSize )
{
	CStringW		str;
	if( uSize < 1024 )
		return L"<1K";

	UINT nK = uSize / 1024;
	UINT nM = nK / 1024;
	if( nM == 0 )
	{
		str.Format( L"%dK", nK );
		return str;
	}

	//nK %= 1024;
	//str.Format( L"%d.%dM", nM, int(nK*100/1024.0f) );
	str.Format( L"%.2fM", nK/1024.f );
	return str;
}

int CHelper::getFiles( vector< CStringW > &as, CStringW strFolder, const vector<CStringW> &wildcards )
{
	if( _waccess(strFolder, 0 ) == -1 )
		return 0;

	CStringW search = strFolder + L"\\*.*";

	WIN32_FIND_DATAW	findData = {0};
	HANDLE hFinder = FindFirstFileW(search, &findData);
	while ( INVALID_HANDLE_VALUE != hFinder )
	{
		CStringW name = findData.cFileName;
		for( int i = 0; i < (int)wildcards.size(); i++ )
		{
			//这里匹配
			if (WildcardMatch2(name, wildcards[i]))
			{
				CStringW file = strFolder+ L"\\" + name;
				as.push_back(file);
			}
		}

		if ( !FindNextFileW(hFinder, &findData) )
			break;
	}

	FindClose(hFinder);
	return 1;
}

//非递归版本,更快
bool CHelper::WildcardMatch2(const wchar_t *pszString, const wchar_t *pszMatch) 
	//  cf. http://www.planet-source-code.com/vb/scripts/ShowCode.asp?txtCodeId=1680&lngWId=3
{
	//	1. Assume that a string matches up to a wildcard. (Part one ofthe loop=
	//	2. If string part to the right matches it be checked by the second loop.
	//	3. All non matching characters are eaten by the last found *.

	//	So no fallback to an ealier * is needed. So recursion isn't needed.
	const wchar_t *cp;
	const wchar_t *mp;

	while ((*pszString) && (*pszMatch != L'*')) 
	{
		if ((towupper(*pszMatch) != towupper(*pszString)) && (*pszMatch != L'?')) 
		{
			return false;
		}
		pszMatch++;
		pszString++;
	}

	while (*pszString) 
	{
		if (*pszMatch == L'*') 
		{
			if (!*++pszMatch) 
			{
				return true;
			}
			mp = pszMatch;
			cp = pszString + 1;
		} 
		else if ((towupper(*pszMatch) == towupper(*pszString)) || (*pszMatch == L'?')) 
		{
			pszMatch++;
			pszString++;
		} 
		else 
		{
			pszMatch = mp;
			pszString = cp++;
		}
	}
	while (*pszMatch == L'*') 
	{
		pszMatch++;
	}
	return !*pszMatch;
}

bool CHelper::fileIsExist( LPCWSTR str )
{
	return 0 == _taccess( str, 0 );
}

void CHelper::unzipFileToFolder( LPCWSTR lpsZipFile, LPCWSTR lpsFolder )
{
	HZIP hz =  OpenZip( (void *)lpsZipFile, 0, ZIP_FILENAME );

	ZIPENTRY ze;
	GetZipItem(hz,-1,&ze); 

	int n=ze.index;
	for (int i=0; i<n; i++)
	{ 
		GetZipItem(hz,i,&ze);

		wstring wsFile = lpsFolder;
		wsFile += ze.name;
		wchar_t tail = ze.name[ wcslen(ze.name)-1 ];
		if( tail == L'\\' || tail == L'//' )

		{
			//CreateDirectory( wsFile.c_str(), NULL );
			SECURITY_ATTRIBUTES	sa = CGlobalSecurityAttributes::instance();
			::CreateDirectory( wsFile.c_str(), &sa );
		}
		else
			UnzipItem(hz, i, (void *)wsFile.c_str(), 0, ZIP_FILENAME);
	}

	CloseZip( hz );
}

int CHelper::GetSubFile(LPCTSTR path,LPCTSTR lpstrFilter )
{
	TCHAR szFind[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	TCHAR szFile[MAX_PATH];
	::ZeroMemory(szFile,MAX_PATH);
	::ZeroMemory(szFind,MAX_PATH);
	_tcscpy(szFind,path);
	_tcscat(szFind,lpstrFilter );

	HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)    return 0;

	int nCount=0;
	while(TRUE)
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(FindFileData.cFileName[0]!=_T('.') )
			{
				_tcscpy(szFile,path);
				_tcscat(szFile,_T("\\") );
				_tcscat(szFile,FindFileData.cFileName);
				GetSubFile(szFile,lpstrFilter);
			}
		}
		else
		{

			nCount++;
		}
		if(!FindNextFile(hFind,&FindFileData))    break;
	}
	FindClose(hFind);

	return nCount;
}

//查找子目录支持递归
int CHelper:: SearchSubDir(vector<wstring>& vec,LPCTSTR directory,bool recursive){
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	TCHAR pathTemp[MAX_PATH];
	ZeroMemory(pathTemp,MAX_PATH);
	wsprintf(pathTemp,_T("%s\\*.*"),directory);
	hFind=FindFirstFile(pathTemp,&fd);

	//::OutputDebugString(pathTemp);
	static int nCount=0;
	wstring str;
	if (INVALID_HANDLE_VALUE==hFind)
	{
		return 0;
	}
	do 
	{
		if(_T('.')==fd.cFileName[0])
		{
			continue;
		}
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			nCount++;
			wsprintf(pathTemp,_T("%s\\%s"),directory,fd.cFileName);
			str=wstring(pathTemp);
			vec.push_back(str);
			if(recursive)
				SearchSubDir(vec,str.c_str(),recursive);
		}
	} while (FindNextFile(hFind,&fd));

	FindClose(hFind);
	return nCount;
}

bool CHelper::UnzipToFolder( CStringW strZipFile, CStringW strOutFolder /*= L"" */ )
{
	SECURITY_ATTRIBUTES	sa = CGlobalSecurityAttributes::instance();

	//wstring	ws = L"d:\\12345\\abc.zip";
	//wstring wsFolder = L"d:\\12345\\abc_XXX";

	if( strOutFolder.IsEmpty() )
	{
		CStringW	strFolder = strZipFile;
		int npos = strFolder.ReverseFind( L'.' );
		if( npos != -1 )
			strFolder.Delete( npos, strFolder.GetLength()-npos );

		strOutFolder = strFolder;
	}

	if( !fileIsExist( strOutFolder ))
	{
		if( ::CreateDirectory(strOutFolder, &sa ) )
			return false;
	}

	HZIP hz =  OpenZip( (void *)strZipFile, 0, ZIP_FILENAME );
	if( hz == NULL )
		return false;

	ZIPENTRY ze;
	GetZipItem(hz,-1,&ze); 

	BOOL		bOK = TRUE;	
	int name=ze.index;
	for (int i=0; i<name; i++)
	{ 
		GetZipItem(hz,i,&ze);

		//wstring wsFile = wsFolder+ze.name;
		CStringW	strFile;
		strFile.Format(L"%s\\%s", strOutFolder, ze.name);

		wchar_t		tail = strFile.GetAt( strFile.GetLength()-1 );

		//wchar_t tail = wsFile.at(wsFile.size()-1);
		//if( tail == L'\\' || tail == L'//' )
		//	CreateDirectory( wsFile.c_str(), NULL );
		if( tail == L'\\' || tail == L'//' )
		{
			bOK = ::CreateDirectory( strFile, &sa );
		}
		else
			UnzipItem(hz, i, (void *)strFile, 0, ZIP_FILENAME);

		//wprintf( L"%s\n", ze.name );
	}

	CloseZip( hz );
	return bOK;
}


BOOL CHelper::AddFolderToZip( void* hZip, TCHAR* szMainPath, TCHAR* szSubPath )
{
	//wprintf( L"%s --- %s\n", szMainPath, szSubPath );


	HANDLE				hFind; // file handle
	WIN32_FIND_DATA		fd = {0};
	TCHAR				szTempPath[MAX_PATH] = {0};

	// Construct the path to search into "C:\\Windows\\System32\\*"
	_tcscpy( szTempPath, szMainPath );
	if( _tcslen( szSubPath ) > 0 )
	{
		_tcscat( szTempPath, _T("\\"));
		_tcscat( szTempPath, szSubPath );

		ZipAdd( (HZIP)hZip, szSubPath, 0, 0, ZIP_FOLDER );
	}

	_tcscat( szTempPath, _T("\\*"));

	hFind = FindFirstFile( szTempPath, &fd ); // find the first file
	if( INVALID_HANDLE_VALUE == hFind )
		return FALSE;

	bool bFound = true;

	while( bFound ) // until we finds an entry
	{
		if(FindNextFile( hFind, &fd ))
		{
			// Don't care about . and ..
			//if(IsDots(FindFileData.cFileName))
			if ((_tcscmp( fd.cFileName, _T(".")) == 0) ||
				(_tcscmp( fd.cFileName, _T("..")) == 0))
				continue;

			// We have found a directory
			if(( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				TCHAR	szNewSubPath[MAX_PATH] = {0};

				if( _tcslen( szSubPath ) > 0 )
				{
					_tcscpy( szNewSubPath, szSubPath );
					_tcscat( szNewSubPath, _T("\\") );
					_tcscat( szNewSubPath, fd.cFileName );
				}
				else
					_tcscpy( szNewSubPath, fd.cFileName );

				// Recursive call with the new directory found
				if ( !AddFolderToZip(hZip, szMainPath, szNewSubPath ) )
					return FALSE ;
			}
			// We have found a file
			else
			{
				// Add the found file to the zip file
				TCHAR	szZipName[MAX_PATH] = {0};

				if( _tcslen( szSubPath ) > 0 )
				{
					_tcscpy( szZipName, szSubPath );
					_tcscat( szZipName, _T("\\") );
					_tcscat( szZipName, fd.cFileName );
				}
				else
				{
					_tcscpy(szZipName, fd.cFileName);
				}


				TCHAR	szFullName[MAX_PATH] = {0};
				_tcscpy( szFullName, szMainPath );
				_tcscat( szFullName, _T("\\"));
				_tcscat( szFullName, szZipName );

				wprintf( L"\tAdd File %s\n", szZipName );
				if (ZipAdd( (HZIP)hZip, szZipName, szFullName, 0, ZIP_FILENAME) != ZR_OK)
					return FALSE;
			}

		}//FindNextFile
		else
		{
			if(GetLastError() == ERROR_NO_MORE_FILES) // no more files there
				bFound = false;
			else
			{
				// some error occured, close the handle and return FALSE
				FindClose(hFind);
				return FALSE;
			}
		}
	}//while

	FindClose(hFind); // closing file handle
	return true;
}

bool CHelper::ZipFolder( CStringW strZipFile, CStringW strFolder )
{
	HZIP hz = CreateZip( strZipFile, 0, ZIP_FILENAME);
	AddFolderToZip( (void *)hz, strFolder );
	CloseZip(hz);

	return true;
}

};
