#pragma once

namespace Json
{
	class Value;
}

class CHelper
{
public:
	CHelper(void);
	virtual ~CHelper(void);

public:
	static int urlGetInfo( string strURL, map<string, string> &m );
	static int urlGetArray( string strURL, vector<maps2s> &as );

	static int urlReturn( string strURL, string &strValue );

private:
	static string	unicode2Mbs( wstring ws );
	static wstring	utf2Unicode( string str );
	static size_t	writeToString(char *buffer,size_t size, size_t nitems,void *outstream);


	static int		jsValue2Map( Json::Value jv, maps2s &m );
};

