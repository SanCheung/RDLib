#pragma once

namespace fblSystem
{

class XDBASE_API CIni
{
public:
	CIni( CStringW strFile, CStringW strAppName = L"" );
	virtual ~CIni(void);

private:
	CStringW		_strFile;
	CStringW		_strAppName;

public:
	void		SetAppName( CStringW strAppName );
	// read
	int			GetInt( CStringW strKey );
	float		GetFloat( CStringW strKey );
	CStringW	GetString( CStringW strKey, CStringW strDefault = L"" );

	// write
	void		SetInt( CStringW strKey, int v );
	void		SetFloat( CStringW strKey, float f );
	void		SetString( CStringW strKey, CStringW strValue );

};

};