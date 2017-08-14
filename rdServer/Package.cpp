#include "StdAfx.h"
#include "Package.h"
#include <sstream>


CAntPackage::CAntPackage(void)
{
	//	m_strMethod = L"";
	m_StatBegin = 0;
	m_StatEnd = 0;

	StatBegin();
}


CAntPackage::~CAntPackage(void)
{
}

void CAntPackage::Reset()
{
//	m_strMethod = L"";
	m_asParam.clear();
	m_mapProp.clear();
	m_BodyData.EraseData();
}

void CAntPackage::SetMethod(char *pData, int nSize)
{
	m_strMethod.assign(pData, nSize);
}

string& CAntPackage::GetMethod()
{
	return m_strMethod;
}

void CAntPackage::AddParam(const string &strParam)
{
	m_asParam.push_back(strParam);
}

void CAntPackage::AddParam(int nParam)
{
	std::stringstream	convert;

	convert << nParam;

	AddParam(convert.str());
}

string CAntPackage::GetParam(UINT nIndex)
{
	if(nIndex >= m_asParam.capacity())
		return "";

	return m_asParam.at(nIndex);
}

void CAntPackage::AddProperty(const string &strKey, const string &strValue)
{
	m_mapProp[strKey] = strValue;
}

void CAntPackage::AddProperty(string strKey, int nValue)
{
//	char	chBuf[12] = {0};

//	sprintf_s(chBuf, 11, "%d", nValue);
	
//	string strValue = chBuf;
	std::stringstream	convert;

	convert << nValue;

	m_mapProp[strKey] = convert.str();

}

void CAntPackage::AddProperty(string strKey, UINT nValue)
{
//	char	chBuf[12] = {0};

//	sprintf_s(chBuf,11, "%I32u", nValue);
	
//	string strValue = chBuf;
	std::stringstream	convert;

	convert << nValue;

	m_mapProp[strKey] = convert.str();
}

void CAntPackage::GetProperty(const string &strKey, string &strValue)
{
	strValue = m_mapProp[strKey];
}

string CAntPackage::GetProperty(string strKey)
{
	return m_mapProp[strKey];
}

bool CAntPackage::GetProperty( string key, UINT& value )
{
	bool result = false;
	map<string, string>::iterator itr = m_mapProp.find(key);
	if (itr != m_mapProp.end())
	{
		result = (sscanf(itr->second.c_str(), "%u", &value) != 0);
	}
	return result;
}

void CAntPackage::AssignBodyData(char *pchData, int nSize)
{
	m_BodyData.AssignData(pchData, nSize);
}

// 生成数据流
void CAntPackage::ToBuffer(CBuffer &buffer)
{
	// Method
	buffer.Append((char*)m_strMethod.c_str(), m_strMethod.length() );
//	buffer.Append(" ", 1);

	// Param
	string	strKey("");
	string	strValue("");

	int  n = (int)m_asParam.size();
	for( int nIndex = 0; nIndex < n; nIndex ++ )
	{
		buffer.Append(" ", 1);
		strValue = m_asParam.at(nIndex);
		buffer.Append((char*)strValue.c_str(), strValue.length() );
	}
	buffer.Append('\n');

	// Properties
	map<string,string>::iterator it = m_mapProp.begin();
	while( it != m_mapProp.end() )
	{
		strKey = it->first;
		strValue = it->second;
		buffer.Append((char*)strKey.c_str(), strKey.length());
		buffer.Append(":", 1);
		buffer.Append((char*)strValue.c_str(), strValue.length());
		buffer.Append('\n');
		it++;
	}

	buffer.Append('\n');

	// Body
	if( m_BodyData.GetSize() > 0 )
	{
		buffer.Append((char*)m_BodyData.GetData(), m_BodyData.GetSize());
	//	buffer.Append('\n');
	}
}

CBuffer* CAntPackage::GetBobyData()
{
	return &m_BodyData;
}

void CAntPackage::ToProtocolBuffer( CBuffer& buffer )
{
	if (!m_strMethod.empty())
	{
		CBuffer tempBuffer;
		ToBuffer(tempBuffer);
		int		nSize = tempBuffer.GetSize();

		buffer.Append((char*)&nSize, 4);
		buffer.Append((char*)tempBuffer.GetData(), tempBuffer.GetSize());
	}
}

void CAntPackage::StatBegin()
{
	m_StatBegin = GetTickCount();
}	

void CAntPackage::StatEnd()
{
	m_StatEnd = GetTickCount();
}

DWORD CAntPackage::ProcessElapsed()
{
	return m_StatEnd - m_StatBegin;
}



//
//把Data中的数据解析成 Package 对象
//到这里的数据都是完整的包,不存在着粘包和分包的情况 
//
//  1.方法0x20参数0x20参数0x0A
//  2.属性名字:属性值0x0A
//  3.0x0A	
//  [4.内容]
//

BOOL CAntPackage::ParseData( char *pData, int nLength, CAntPackage *pPackage )
{
	if( pPackage == NULL )
		return FALSE;

	pPackage->Reset();

	int		nPos = 0;
	int		nIndex = 0;
	string	strKey("");
	string	strValue("");

	// 解析 Method和Param
	for( nIndex = 0; nIndex < nLength; nIndex ++ )
	{
		if( pData[nIndex] == ' ' )
		{
			pPackage->SetMethod(pData, nIndex);
			nPos = nIndex + 1;
			break;
		}
	}

	// 解析Param
	for( nIndex = nPos; nIndex < nLength; nIndex ++ )
	{
		if( pData[nIndex] == ' ' )
		{
			strValue.assign(pData + nPos, nIndex - nPos);
			pPackage->AddParam(strValue);
			nPos = nIndex + 1;
		}
		else if( pData[nIndex] == '\n' )
		{
			strValue.assign(pData + nPos, nIndex - nPos);
			pPackage->AddParam(strValue);
			nPos = nIndex + 1;
			break;
		}
	}

	// 解析 Property
	strKey.erase();
	strValue.erase();

	for( nIndex = nPos; nIndex < nLength; nIndex ++ )
	{
		if( pData[nIndex] == ':' )
		{
			strKey.assign(pData + nPos, nIndex - nPos);
			nPos = nIndex + 1;
		}
		else if( pData[nIndex] == '\n' )
		{
			if( strKey.empty() )
			{
				//表示解析到头了,下面是bodyData或者Package已经解析完毕
				nPos = nIndex + 1;
				break;
			}
			else
			{
				strValue.assign(pData + nPos, nIndex - nPos);
				pPackage->AddProperty( strKey, strValue );
				strKey.erase();
				strValue.erase();
				nPos = nIndex + 1;
			}
		}
	}

	// 解析 Body
	strKey = "content-length";
	pPackage->GetProperty(strKey, strValue);
	if( !strValue.empty() )
	{
		int nContentLen = atoi(strValue.c_str());
		if( nPos + nContentLen != nLength )
		{
			ATLTRACE( _T("Parse Error1\r\n") );
			return FALSE;
		}

		pPackage->AssignBodyData( pData + nPos, nContentLen );
	}
	else
	{
		if( nPos != nLength )
		{
			ATLTRACE( _T("Parse Error2\r\n") );
			return FALSE;
		}
	}
	return TRUE;
}
