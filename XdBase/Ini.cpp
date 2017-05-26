#include "StdAfx.h"

#include "sstring.h"
using namespace sstring;

#include "Ini.h"

namespace fblSystem
{

CIni::CIni( CStringW strFile, CStringW strAppName )
	: _strFile( strFile )
	, _strAppName( strAppName )
{

}


CIni::~CIni(void)
{
}

void CIni::SetAppName( CStringW strAppName )
{
	_strAppName = strAppName;
}

int CIni::GetInt( CStringW strKey )
{
	return ::GetPrivateProfileIntW( _strAppName, strKey, 0, _strFile );
}


float CIni::GetFloat( CStringW strKey )
{
	CStringW	strValue = GetString( strKey );
	return strValue.ToFloat();
}


CStringW CIni::GetString( CStringW strKey, CStringW strDefault )
{
	CStringW	strValue;
	::GetPrivateProfileStringW( _strAppName, strKey, strDefault, strValue.GetBuffer(512), 512, _strFile );
	strValue.ReleaseBuffer();

	return strValue;
}

void CIni::SetInt( CStringW strKey, int v )
{
	CStringW		strValue;
	strValue.Format( L"%d", v );
	SetString( strKey, strValue );
}

void CIni::SetFloat( CStringW strKey, float f )
{
	CStringW		strValue;
	strValue.Format( L"%f", f );
	SetString( strKey, strValue );
}

void CIni::SetString( CStringW strKey, CStringW strValue )
{
	::WritePrivateProfileStringW( _strAppName, strKey, strValue, _strFile );
}

};