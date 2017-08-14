#pragma once

#include "Buffer.h"
#include <map>
#include <vector>
#include <string>

using namespace std;

class CAntPackage
{
public:
	CAntPackage(void);
	virtual ~CAntPackage(void);

public:
	void	Reset();
	
	void	SetMethod(char *pData, int nSize);
	string& GetMethod();
	
	void	AddParam(const string &strParam);
	void	AddParam(int nParam);
	
	void	AddProperty(const string &strKey, const string &strValue);
	void	AddProperty(string strKey, int nValue);
	void	AddProperty(string strKey, UINT nValue);
	void	GetProperty(const string &strkey, string &strValue);
	
	void	AssignBodyData(char *pchData, int nSize);
	string	GetParam(UINT nIndex);
	
	string	GetProperty(string strKey);
	bool    GetProperty(string key, UINT& value);

	void	ToBuffer(CBuffer &buffer);
	void	ToProtocolBuffer(CBuffer& buffer);

	CBuffer* GetBobyData();

	void StatBegin();
	void StatEnd();
	
	DWORD ProcessElapsed();

protected:
	// 数据都是用的UTF8编码后保存的char
	string			m_strMethod;
	vector<string>	m_asParam; // 参数
	map<string,string>	m_mapProp; //属性

	CBuffer			m_BodyData; // body内容

	//统计
	DWORD			m_StatBegin;
	DWORD			m_StatEnd;


public:
	static BOOL ParseData(char *pData, int nLength, CAntPackage *pPackage);
	
};


typedef tr1::shared_ptr<CAntPackage>	CAntPackagePtr;	

