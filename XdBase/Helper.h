#pragma once

#include "sstring.h"
using namespace sstring;

namespace fblSystem
{

class XDBASE_API CGlobalSecurityAttributes
{
public:
	static SECURITY_ATTRIBUTES instance();

private:
	static SECURITY_ATTRIBUTES		s_SA;
	static SECURITY_DESCRIPTOR		s_SD;
	static bool						s_Initialized;
};


class XDBASE_API CHelper
{
public:
	CHelper(void);
	virtual ~CHelper(void);

	static CStringW		GetAppdataFolder();
	static CStringW		GetPLSSFolder();
	//static CStringW		GetMeetingClientFolder();
	static CStringW		GetHandWriterFolder();
	static CStringW		GetCurrentPath();

	
	static bool		GetFileNameFromDialog( bool bSave, HWND hWnd, wchar_t *strFilter, CStringW &strFile );
	static bool		DoModelFontDialog( HWND hWnd, CStringW	&strFontName, COLORREF &cr, int &fontSize, int &bUnderline, int &bItalic, int &bBold );
	static bool		DoModelFontDialog( HWND hWnd, LOGFONT &lf, COLORREF &cr );


	static int		dlgOpenMultipleFile( HWND hWnd, wchar_t *strFilter, vector<CStringW> &as );


	static CStringA		Md5( const CStringW &str );
	static CStringA		ToAsciiString( unsigned char *in, int len );

	static CStringW		EncrypTea( CStringW str, CStringW strKey );
	static CStringW		DecrypTea( CStringW str, CStringW strKey );

	static CStringW		array2String( const vector<int> &an, wchar_t sp = L';' );
	static CStringW		array2String( const vector<CStringW> &as, wchar_t sp = L';' );

	static int			string2Array( const CStringW &strIn, vector<CStringW> &as, const CStringW &sp = L";£»" );
	static int			string2Array( const CStringW &strIn, vector<int> &an );

	static bool			string2Time( CStringW strTime, SYSTEMTIME &st, int &nHour, int &nMinute );
	static CStringW		time2String( const SYSTEMTIME &st, int nHour, int nMinute);

	static bool			string2Time( CStringW strTime, SYSTEMTIME &st );

	//yyyy-mm-dd hh:mm:ss
	static CStringW		nowString();
	//yyyymmddhhmmss
	static CStringW		nowString2();

	static CStringW		getFileName( const CStringW &strFile );
	static UINT			getFileSize( const CStringW &strFile );
	static CStringW		formatByteSize( UINT uSize );

	static int			getFiles( vector< CStringW > &as, CStringW strFolder, const vector<CStringW> &wildcards );
	static bool			WildcardMatch2(const wchar_t *pszString, const wchar_t *pszMatch);

	static bool			fileIsExist( LPCWSTR str );

	static void			unzipFileToFolder( LPCWSTR lpsZipFile, LPCWSTR lpsFolder );

	static bool			UnzipToFolder( CStringW strZipFile, CStringW strOutFolder );
	static bool			ZipFolder( CStringW strZipFile, CStringW strFolder );


	static int GetSubFile(LPCTSTR path,LPCTSTR lpstrFilter=_T("\\*.png") );
	static int SearchSubDir(vector<wstring>& vec, LPCTSTR directory,bool recursive=true);

private:
	static BOOL			AddFolderToZip( void* hZip, TCHAR* szMainPath, TCHAR* szSubPath = L"" );
};

};

