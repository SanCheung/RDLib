#include "StdAfx.h"
#include "sstring.h"

// 20160523
#pragma warning(once:4996)


#ifdef _DEBUG
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// Globals

#ifndef A_ATL_NO_OLD_HEADERS_WIN64
#if !defined(_WIN64) 
typedef long ALONG_PTR;
typedef unsigned long AULONG_PTR;
typedef AULONG_PTR ADWORD_PTR;
#endif //!defined(_WIN64) && (_ATL_VER < 0x0700)
#endif //!_ATL_NO_OLD_HEADERS_WIN64


namespace	sstring
{


// For an empty string, m_pchData will point here
// (note: avoids special case of checking for NULL m_pchData) empty string data (and locked)
_declspec(selectany) int rgInitData[] = { -1, 0, 0, 0 };

_declspec(selectany) CStringA::CStringDataA* _atltmpDataNilA = (CStringA::CStringDataA*)&rgInitData;
_declspec(selectany) CStringW::CStringDataW* _atltmpDataNilW = (CStringW::CStringDataW*)&rgInitData;

_declspec(selectany) LPCSTR _atltmpPchNilA = (LPCSTR)(((BYTE*)&rgInitData) + sizeof(CStringA::CStringDataA));
_declspec(selectany) LPCWSTR _atltmpPchNilW = (LPCWSTR)(((BYTE*)&rgInitData) + sizeof(CStringW::CStringDataW));



// Helpers to avoid CRT startup code
#ifdef _ATL_MIN_CRT
static TCHAR* _cstrchr(const TCHAR* p, TCHAR ch)
{
	//strchr for '\0' should succeed
	while (*p != 0)
	{
		if (*p == ch)
			break;
		p = ::CharNext(p);
	}
	return (TCHAR*)((*p == ch) ? p : NULL);
}

static TCHAR* _cstrchr_db(const TCHAR* p, TCHAR ch1, TCHAR ch2)
{
	const TCHAR* lpsz = NULL;
	while (*p != 0)
	{
		if (*p == ch1 && *(p + 1) == ch2)
		{
			lpsz = p;
			break;
		}
		p = ::CharNext(p);
	}
	return (TCHAR*)lpsz;
}

static TCHAR* _cstrrchr(const TCHAR* p, TCHAR ch)
{
	const TCHAR* lpsz = NULL;
	while (*p != 0)
	{
		if (*p == ch)
			lpsz = p;
		p = ::CharNext(p);
	}
	return (TCHAR*)lpsz;
}

static TCHAR* _cstrrev(TCHAR* pStr)
{
	// Optimize NULL, zero-length, and single-TCHAR case.
	if ((pStr == NULL) || (pStr[0] == '\0') || (pStr[1] == '\0'))
		return pStr;
	
	TCHAR* p = pStr;
	
	while (p[1] != 0) 
	{
		TCHAR* pNext = ::CharNext(p);
		if(pNext > p + 1)
		{
			TCHAR p1 = *(TCHAR*)p;
			*(TCHAR*)p = *(TCHAR*)(p + 1);
			*(TCHAR*)(p + 1) = p1;
		}
		p = pNext;
	}
	
	TCHAR* q = pStr;
	
	while (q < p)
	{
		TCHAR t = *q;
		*q = *p;
		*p = t;
		q++;
		p--;
	}
	return (TCHAR*)pStr;
}

static TCHAR* _cstrstr(const TCHAR* pStr, const TCHAR* pCharSet)
{
	int nLen = lstrlen(pCharSet);
	if (nLen == 0)
		return (TCHAR*)pStr;
	
	const TCHAR* pRet = NULL;
	const TCHAR* pCur = pStr;
	while((pStr = _cstrchr(pCur, *pCharSet)) != NULL)
	{
		if(memcmp(pCur, pCharSet, nLen * sizeof(TCHAR)) == 0)
		{
			pRet = pCur;
			break;
		}
		pCur = ::CharNext(pCur);
	}
	return (TCHAR*) pRet;
}

static int _cstrspn(const TCHAR* pStr, const TCHAR* pCharSet)
{
	int nRet = 0;
	TCHAR* p = (TCHAR*)pStr;
	while (*p != 0)
	{
		TCHAR* pNext = ::CharNext(p);
		if(pNext > p + 1)
		{
			if(_cstrchr_db(pCharSet, *p, *(p + 1)) == NULL)
				break;
			nRet += 2;
		}
		else
		{
			if(_cstrchr(pCharSet, *p) == NULL)
				break;
			nRet++;
		}
		p = pNext;
	}
	return nRet;
}

static int _cstrcspn(const TCHAR* pStr, const TCHAR* pCharSet)
{
	int nRet = 0;
	TCHAR* p = (TCHAR*)pStr;
	while (*p != 0)
	{
		TCHAR* pNext = ::CharNext(p);
		if(pNext > p + 1)
		{
			if(_cstrchr_db(pCharSet, *p, *(p + 1)) != NULL)
				break;
			nRet += 2;
		}
		else
		{
			if(_cstrchr(pCharSet, *p) != NULL)
				break;
			nRet++;
		}
		p = pNext;
	}
	return nRet;
}

static TCHAR* _cstrpbrk(const TCHAR* p, const TCHAR* lpszCharSet)
{
	while (*p != 0)
	{
		if (_cstrchr(lpszCharSet, *p) != NULL)
		{
			return (TCHAR*)p;
			break;
		}
		p = ::CharNext(p);
	}
	return NULL;
}

static int _cstrisdigit(TCHAR ch)
{
	WORD type;
	GetStringTypeEx(GetThreadLocale(), CT_CTYPE1, &ch, 1, &type);
	return (type & C1_DIGIT) == C1_DIGIT;
}

static int _cstrisspace(TCHAR ch)
{
	WORD type;
	GetStringTypeEx(GetThreadLocale(), CT_CTYPE1, &ch, 1, &type);
	return (type & C1_SPACE) == C1_SPACE;
}

static int _cstrcmp(const TCHAR* pstrOne, const TCHAR* pstrOther)
{
	return lstrcmp(pstrOne, pstrOther);
}

static int _cstrcmpi(const TCHAR* pstrOne, const TCHAR* pstrOther)
{
	return lstrcmpi(pstrOne, pstrOther);
}

static int _cstrcoll(const TCHAR* pstrOne, const TCHAR* pstrOther)
{
	int nRet = CompareString(GetThreadLocale(), 0, pstrOne, -1, pstrOther, -1);
	ATLASSERT(nRet != 0);
	return nRet - 2;  // Convert to strcmp convention.  This really is documented.
}

static int _cstrcolli(const TCHAR* pstrOne, const TCHAR* pstrOther)
{
	int nRet = CompareString(GetThreadLocale(), NORM_IGNORECASE, pstrOne, -1, pstrOther, -1);
	ATLASSERT(nRet != 0);
	return nRet - 2;  // Convert to strcmp convention.  This really is documented.
}

static int _cstrtoi(const TCHAR* nptr)
{
	int c;              /* current TCHAR */
	int total;          /* current total */
	int sign;           /* if '-', then negative, otherwise positive */
	
	while ( _cstrisspace(*nptr) )
		++nptr;
	
	c = (int)(_TUCHAR)*nptr++;
	sign = c;           /* save sign indication */
	if (c == _T('-') || c == _T('+'))
		c = (int)(_TUCHAR)*nptr++;    /* skip sign */
	
	total = 0;
	
	while (_cstrisdigit((TCHAR)c)) {
		total = 10 * total + (c - '0');     /* accumulate digit */
		c = (int)(_TUCHAR)*nptr++;    /* get next TCHAR */
	}
	
	if (sign == '-')
		return -total;
	else
		return total;   /* return result, negated if necessary */
}

#else //!_ATL_MIN_CRT

char* _cstrchr(const char* p, char ch)
{
	return (char*)strchr(p, ch);
}

wchar_t* _cstrchr(const  wchar_t* p, wchar_t ch)
{
	return (wchar_t*)wcschr(p, ch);
}

wchar_t* _cstrrchr( const wchar_t* p, wchar_t ch)
{
	return (wchar_t*)wcsrchr(p, ch);
}

char* _cstrrchr( const char* p, char ch)
{
	return (char*)strrchr(p, ch);
}

wchar_t* _cstrrev(wchar_t* pStr)
{
	return _wcsrev(pStr);
}

char* _cstrrev(char* pStr)
{
	return _strrev(pStr);
}

wchar_t* _cstrstr( const wchar_t* pStr,  const wchar_t* pCharSet)
{
	return (wchar_t*)wcsstr(pStr, pCharSet);
}

char* _cstrstr( const char* pStr, const char* pCharSet)
{
	return (char*)strstr(pStr, pCharSet);
}

int _cstrspn( const wchar_t* pStr,  const wchar_t* pCharSet)
{
	return (int)wcsspn(pStr, pCharSet);
}

int _cstrspn( const char* pStr, const char* pCharSet)
{
	return (int)strspn(pStr, pCharSet);
}

int _cstrcspn( const wchar_t* pStr, const wchar_t* pCharSet)
{
	return (int)wcscspn(pStr, pCharSet);
}

int _cstrcspn(const char* pStr, const char* pCharSet)
{
	return (int)strspn(pStr, pCharSet);
}

wchar_t* _cstrpbrk( const wchar_t* p, const wchar_t* lpszCharSet)
{
	return (wchar_t*)wcspbrk(p, lpszCharSet);
}

char* _cstrpbrk(const char* p, const char* lpszCharSet)
{
	return (char*)strpbrk(p, lpszCharSet);
}

int _cstrisdigit(wchar_t ch)
{
	return iswdigit(ch);
}

int _cstrisdigit(char ch)
{
	return isdigit(ch);
}

int _cstrisspace(wchar_t ch)
{
	return iswspace(ch);
}

int _cstrisspace(char ch)
{
	return isspace(ch);
}

int _cstrcmp(const wchar_t* pstrOne, const wchar_t* pstrOther)
{
	return wcscmp(pstrOne, pstrOther);
}

int _cstrcmp(const char* pstrOne, const char* pstrOther)
{
	return strcmp(pstrOne, pstrOther);
}

int _cstrcmpi(const wchar_t* pstrOne, const wchar_t* pstrOther)
{
	return _wcsicmp(pstrOne, pstrOther);
}

int _cstrcmpi(const char* pstrOne, const char* pstrOther)
{
	return _stricmp(pstrOne, pstrOther);
}

int _cstrcoll(const wchar_t* pstrOne, const wchar_t* pstrOther)
{
	return wcscoll(pstrOne, pstrOther);
}

int _cstrcoll(const char* pstrOne, const char* pstrOther)
{
	return strcoll(pstrOne, pstrOther);
}

int _cstrcolli(const wchar_t* pstrOne, const wchar_t* pstrOther)
{
	return _wcsicoll(pstrOne, pstrOther);
}

int _cstrcolli(const char* pstrOne, const char* pstrOther)
{
	return _stricoll(pstrOne, pstrOther);
}

int _cstrtoi(const wchar_t* nptr)
{
	return _wtoi(nptr);
}

int _cstrtoi(const char* nptr)
{
	return atoi(nptr);
}
#endif //!_ATL_MIN_CRT

int PASCAL SafeStrlenA(LPCSTR lpsz)
{ 
	return (lpsz == NULL) ? 0 : lstrlenA(lpsz); 
}

int PASCAL SafeStrlenW(LPCWSTR lpsz)
{ 
	return (lpsz == NULL) ? 0 : lstrlenW(lpsz); 
}

// conversion helpers
int __cdecl _wcstombsz(char* mbstr, const wchar_t* wcstr, size_t count)
{
	if (count == 0 && mbstr != NULL)
		return 0;
	
	int result = ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1, mbstr, (int)count, NULL, NULL);
	ATLASSERT(mbstr == NULL || result <= (int)count);
	if (result > 0)
		mbstr[result - 1] = 0;
	return result;
}

int CStringHelper::BytesOfW2A(const wchar_t* wcstr, int length)
{
	if (wcstr == NULL || length == 0)
		return 0;
	
	int result = ::WideCharToMultiByte(CP_ACP, 0, wcstr, length, 0, 0, NULL, NULL);

	return result;
}

int __cdecl _mbstowcsz(wchar_t* wcstr, const char* mbstr, size_t count)
{
	if (count == 0 && wcstr != NULL)
		return 0;
	
	int result = ::MultiByteToWideChar(CP_ACP, 0, mbstr, -1, wcstr, (int)count);
	ATLASSERT(wcstr == NULL || result <= (int)count);
	if (result > 0)
		wcstr[result - 1] = 0;
	return result;
}
	
int CStringHelper::BytesOfA2W(const char* cstr, int length)
{
	if (cstr == NULL || length == 0)
		return 0;
	
	int result = ::MultiByteToWideChar(CP_ACP, 0, cstr, length, 0, 0);
	return result;
}

static BOOL __stdcall _IsValidString(LPCWSTR lpsz, int nLength = -1)
{
	if(lpsz == NULL)
		return FALSE;
	return !::IsBadStringPtrW(lpsz, nLength);
}

static BOOL __stdcall _IsValidString(LPCSTR lpsz, int nLength = -1)
{
	if(lpsz == NULL)
		return FALSE;
	return !::IsBadStringPtrA(lpsz, nLength);
}

static int __stdcall _LoadStringA(UINT nID, LPCSTR lpszBuf, UINT nMaxBuf)
{
	/*jonfei
	#ifdef _DEBUG
	// LoadString without annoying warning from the Debug kernel if the
	//  segment containing the string is not present
	if (::FindResource(_Module.GetResourceInstance(), MAKEINTRESOURCE((nID>>4) + 1), RT_STRING) == NULL)
	{
	lpszBuf[0] = '\0';
	return 0; // not found
	}
	#endif //_DEBUG
	int nLen = ::LoadString(_Module.GetResourceInstance(), nID, lpszBuf, nMaxBuf);
	if (nLen == 0)
	lpszBuf[0] = '\0';
	return nLen;
		*/return 0;
}

static int __stdcall _LoadStringW(UINT nID, LPWSTR lpszBuf, UINT nMaxBuf)
{
	/*jonfei
	#ifdef _DEBUG
	// LoadString without annoying warning from the Debug kernel if the
	//  segment containing the string is not present
	if (::FindResource(_Module.GetResourceInstance(), MAKEINTRESOURCE((nID>>4) + 1), RT_STRING) == NULL)
	{
	lpszBuf[0] = '\0';
	return 0; // not found
	}
	#endif //_DEBUG
	int nLen = ::LoadString(_Module.GetResourceInstance(), nID, lpszBuf, nMaxBuf);
	if (nLen == 0)
	lpszBuf[0] = '\0';
	return nLen;
		*/return 0;
}



//////////////////////////////////////////////////////////////////////////
//CStringA 实现
CStringA::~CStringA()
//  free any attached data
{
	if (GetData() != _atltmpDataNilA)
	{
		if (InterlockedDecrement((long*)&GetData()->nRefs) <= 0)
			delete[] (BYTE*)GetData();
	}
}

void CStringA::Init()
{ 
	m_pchData = _GetEmptyString().m_pchData; 
}

void CStringA::Release()
{
	if (GetData() != _atltmpDataNilA)
	{
		ATLASSERT(GetData()->nRefs != 0);
		if (InterlockedDecrement((long*)&GetData()->nRefs) <= 0)
			delete[] (BYTE*)GetData();
		Init();
	}
}

void __stdcall CStringA::Release( CStringDataA* pData )
{
	if (pData != _atltmpDataNilA)
	{
		ATLASSERT(pData->nRefs != 0);
		if (InterlockedDecrement((long*)&pData->nRefs) <= 0)
			delete[] (BYTE*)pData;
	}
}

const CStringA& __stdcall CStringA::_GetEmptyString()
{
	return *(CStringA*)&_atltmpPchNilA;
}

CStringA::CStringDataA* CStringA::GetData() const
{ 
	ATLASSERT(m_pchData != NULL); 
	return ((CStringDataA*)m_pchData) - 1;
}

//构造
CStringA::CStringA()
{
	Init();
}

CStringA::CStringA(const CStringA& stringSrc)
{
	ATLASSERT(stringSrc.GetData()->nRefs != 0);
	if (stringSrc.GetData()->nRefs >= 0)
	{
		ATLASSERT(stringSrc.GetData() != _atltmpDataNilA);
		m_pchData = stringSrc.m_pchData;
		InterlockedIncrement((long*)&GetData()->nRefs);
	}
	else
	{
		Init();
		*this = stringSrc.m_pchData;
	}
}

CStringA::CStringA(LPCSTR lpsz)
{
	Init();
	if (lpsz != NULL && HIWORD(lpsz) == NULL)
	{
		UINT nID = LOWORD((ADWORD_PTR)lpsz);
		LoadString(nID);
	}
	else
	{
		int nLen = SafeStrlenA(lpsz);
		if (nLen != 0)
		{
			if(AllocBuffer(nLen))
				memcpy(m_pchData, lpsz, nLen * sizeof(char));
		}
	}
}

CStringA::CStringA(LPCWSTR lpsz)
{
	Init();
	int bytes = BytesOfW2A(lpsz, SafeStrlenW(lpsz));
	if (bytes != 0)
	{
		if(AllocBuffer(bytes))
		{
			_wcstombsz(m_pchData, lpsz, bytes + 1);
			ReleaseBuffer();
		}
	}
}

CStringA::CStringA(char ch, int nLength)
{
	ATLASSERT(!_istlead(ch));    // can't create a lead byte string
	Init();
	if (nLength >= 1)
	{
		if(AllocBuffer(nLength))
		{
			for (int i = 0; i < nLength; i++)
				m_pchData[i] = ch;
		}
	}
}

CStringA::CStringA(LPCSTR lpch, int nLength)
{
	Init();
	if (lpch != NULL && nLength != 0)
	{
		if(AllocBuffer(nLength))
			memcpy(m_pchData, lpch, nLength * sizeof(char));
	}
}

CStringA::CStringA(LPCWSTR lpsz, int nLength)
{
	Init();
	int bytes = BytesOfW2A(lpsz, nLength);
	if (nLength != 0 && bytes != 0)
	{
		if(AllocBuffer(bytes))
		{
			_wcstombsz(m_pchData, lpsz, bytes + 1);
			ReleaseBuffer();		
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//赋值运算符
CStringA::CStringA(const unsigned char* lpsz)
{ 
	Init(); 
	*this = (LPCSTR)lpsz; 
}

const CStringA& CStringA::operator=(const unsigned char* lpsz)
{ 
	*this = (LPCSTR)lpsz; 
	return *this; 
}

const CStringA& CStringA::operator=(wchar_t ch)
{ 
	*this = (char)ch;
	return *this; 
}

const CStringA& CStringA::operator=(char ch)
{
	ATLASSERT(!_istlead(ch));    // can't set single lead byte
	AssignCopy(1, &ch);
	return *this;
}

const CStringA& CStringA::operator=(const CStringA& stringSrc)
{
	if (m_pchData != stringSrc.m_pchData)
	{
		if ((GetData()->nRefs < 0 && GetData() != _atltmpDataNilA) || stringSrc.GetData()->nRefs < 0)
		{
			// actual copy necessary since one of the strings is locked
			AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
		}
		else
		{
			// can just copy references around
			Release();
			ATLASSERT(stringSrc.GetData() != _atltmpDataNilA);
			m_pchData = stringSrc.m_pchData;
			InterlockedIncrement((long*)&GetData()->nRefs);
		}
	}
	return *this;
}

const CStringA& CStringA::operator=(LPCSTR lpsz)
{
	ATLASSERT(lpsz == NULL || _IsValidString(lpsz));
	AssignCopy(SafeStrlenA(lpsz), lpsz);
	return *this;
}

const CStringA& CStringA::operator=(LPCWSTR lpsz)
{
	int nSrcLen = BytesOfW2A(lpsz, SafeStrlenW(lpsz));
	if(AllocBeforeWrite(nSrcLen))
	{
		_wcstombsz(m_pchData, lpsz, nSrcLen + 1);
		ZeroAfter(nSrcLen);
		ReleaseBuffer();
	}
	return *this;
}

const CStringA& CStringA::operator+=(char ch)
{ 
	ConcatInPlace(1, &ch);
	return *this; 
}

const CStringA& CStringA::operator+=(LPCSTR lpsz)
{
	ATLASSERT(lpsz == NULL || _IsValidString(lpsz));
	ConcatInPlace(SafeStrlenA(lpsz), lpsz);
	return *this;
}


const CStringA& CStringA::operator+=(const CStringA& string)
{
	ConcatInPlace(string.GetData()->nDataLength, string.m_pchData);
	return *this;
}


//////////////////////////////////////////////////////////////////////////

int CStringA::GetLength() const
{ 
	return GetData()->nDataLength; 
}

int CStringA::GetAllocLength() const
{ 
	return GetData()->nAllocLength; 
}

BOOL CStringA::IsEmpty() const
{
	return GetData()->nDataLength == 0; 
}

CStringA::operator LPCSTR() const
{ 
	return m_pchData; 
}

CStringA::operator char*() const
{ 
	return (char *)m_pchData; 
}


int CStringA::Compare(LPCSTR lpsz) const
{ 
	return _cstrcmp(m_pchData, lpsz); // MBCS/Unicode aware
}   

int CStringA::CompareNoCase(LPCSTR lpsz) const
{ 
	return _cstrcmpi(m_pchData, lpsz);  // MBCS/Unicode aware
}  

// Collate is often slower than Compare but is MBSC/Unicode
//  aware as well as locale-sensitive with respect to sort order.
int CStringA::Collate(LPCSTR lpsz) const
{ 
	return _cstrcoll(m_pchData, lpsz);  // locale sensitive
} 
 
int CStringA::CollateNoCase(LPCSTR lpsz) const
{ 
	return _cstrcolli(m_pchData, lpsz);// locale sensitive
 }   

char CStringA::GetAt(int nIndex) const
{
	ATLASSERT(nIndex >= 0);
	ATLASSERT(nIndex < GetData()->nDataLength);
	return m_pchData[nIndex];
}

char CStringA::operator[](int nIndex) const
{
	// same as GetAt
	ATLASSERT(nIndex >= 0);
	ATLASSERT(nIndex < GetData()->nDataLength);
	return m_pchData[nIndex];
}


BOOL CStringA::AllocBuffer(int nLen)
// always allocate one extra character for '\0' termination
// assumes [optimistically] that data length will equal allocation length
{
	ATLASSERT(nLen >= 0);
	ATLASSERT(nLen <= INT_MAX - 1);    // max size (enough room for 1 extra)
	
	if (nLen == 0)
	{
		Init();
	}
	else
	{
		CStringDataA* pData = NULL;
		ATLTRY(pData = (CStringDataA*)new BYTE[sizeof(CStringDataA) + (nLen + 1) * sizeof(char)]);
		if(pData == NULL)
			return FALSE;
		
		pData->nRefs = 1;
		pData->data()[nLen] = '\0';
		pData->nDataLength = nLen;
		pData->nAllocLength = nLen;
		m_pchData = pData->data();
	}
	
	return TRUE;
}

void CStringA::Empty()
{
	if (GetData()->nDataLength == 0)
		return;
	
	if (GetData()->nRefs >= 0)
		Release();
	else
		*this = _T("");
	
	ATLASSERT(GetData()->nDataLength == 0);
	ATLASSERT(GetData()->nRefs < 0 || GetData()->nAllocLength == 0);
}

void CStringA::ZeroAfter( int index )
{
	CStringDataA* data = GetData();
	if (data->data() && index < data->nAllocLength)
	{
		memset(data->data() + index, 0, data->nAllocLength - index);
	}
}

void CStringA::CopyBeforeWrite()
{
	if (GetData()->nRefs > 1)
	{
		CStringDataA* pData = GetData();
		Release();
		if(AllocBuffer(pData->nDataLength)){
			memcpy(m_pchData, pData->data(), (pData->nDataLength + 1) * sizeof(char));
		}
	}
	ATLASSERT(GetData()->nRefs <= 1);
}

BOOL CStringA::AllocBeforeWrite(int nLen)
{
	BOOL bRet = TRUE;
	if (GetData()->nRefs > 1 || nLen > GetData()->nAllocLength)
	{
		Release();
		bRet = AllocBuffer(nLen);
	}
	ATLASSERT(GetData()->nRefs <= 1);
	return bRet;
}

void CStringA::AllocCopy(CStringA& dest, int nCopyLen, int nCopyIndex,
								 int nExtraLen) const
{
	// will clone the data attached to this string
	// allocating 'nExtraLen' characters
	// Places results in uninitialized string 'dest'
	// Will copy the part or all of original data to start of new string
	
	int nNewLen = nCopyLen + nExtraLen;
	if (nNewLen == 0)
	{
		dest.Init();
	}
	else
	{
		if(dest.AllocBuffer(nNewLen))
			memcpy(dest.m_pchData, m_pchData + nCopyIndex, nCopyLen * sizeof(char));
	}
}


// Assignment operators
//  All assign a new value to the string
//      (a) first see if the buffer is big enough
//      (b) if enough room, copy on top of old buffer, set size and type
//      (c) otherwise free old string data, and create a new one
//
//  All routines return the new string (but as a 'const CStringA&' so that
//      assigning it again will cause a copy, eg: s1 = s2 = "hi there".
//
void CStringA::AssignCopy(int nSrcLen, LPCSTR lpszSrcData)
{
	if(AllocBeforeWrite(nSrcLen))
	{
		CStringDataA* data = GetData();
		if (nSrcLen == 0){
			memset(data->data(), 0, data->nDataLength);
		}
		else{
			memcpy(m_pchData, lpszSrcData, nSrcLen * sizeof(char));
			ZeroAfter(nSrcLen);
		}
		GetData()->nDataLength = nSrcLen;
		m_pchData[nSrcLen] = '\0';
	}
}

// Concatenation
// NOTE: "operator+" is done as friend functions for simplicity
//      There are three variants:
//          CStringA + CStringA
// and for ? = char, LPCSTR
//          CStringA + ?
//          ? + CStringA

BOOL CStringA::ConcatCopy(int nSrc1Len, LPCSTR lpszSrc1Data,
								  int nSrc2Len, LPCSTR lpszSrc2Data)
{
	// -- master concatenation routine
	// Concatenate two sources
	// -- assume that 'this' is a new CStringA object
	
	BOOL bRet = TRUE;
	int nNewLen = nSrc1Len + nSrc2Len;
	if (nNewLen != 0)
	{
		bRet = AllocBuffer(nNewLen);
		if (bRet)
		{
			memcpy(m_pchData, lpszSrc1Data, nSrc1Len * sizeof(char));
			memcpy(m_pchData + nSrc1Len, lpszSrc2Data, nSrc2Len * sizeof(char));
		}
	}
	return bRet;
}


void CStringA::ConcatInPlace(int nSrcLen, LPCSTR lpszSrcData)
{
	//  -- the main routine for += operators
	
	// concatenating an empty string is a no-op!
	if (nSrcLen == 0)
		return;
	
	// if the buffer is too small, or we have a width mis-match, just
	//   allocate a new buffer (slow but sure)
	if (GetData()->nRefs > 1 || GetData()->nDataLength + nSrcLen > GetData()->nAllocLength)
	{
		// we have to grow the buffer, use the ConcatCopy routine
		CStringDataA* pOldData = GetData();
		if (ConcatCopy(GetData()->nDataLength, m_pchData, nSrcLen, lpszSrcData))
		{
			ATLASSERT(pOldData != NULL);
			CStringA::Release(pOldData);
		}
	}
	else
	{
		// fast concatenation when buffer big enough
		memcpy(m_pchData + GetData()->nDataLength, lpszSrcData, nSrcLen * sizeof(char));
		GetData()->nDataLength += nSrcLen;
		ATLASSERT(GetData()->nDataLength <= GetData()->nAllocLength);
		m_pchData[GetData()->nDataLength] = '\0';
	}
}

LPSTR CStringA::GetBuffer(int nMinBufLength)
{
	ATLASSERT(nMinBufLength >= 0);
	
	if (GetData()->nRefs > 1 || nMinBufLength > GetData()->nAllocLength)
	{
		// we have to grow the buffer
		CStringDataA* pOldData = GetData();
		int nOldLen = GetData()->nDataLength;   // AllocBuffer will tromp it
		if (nMinBufLength < nOldLen)
			nMinBufLength = nOldLen;
		
		if(!AllocBuffer(nMinBufLength))
			return NULL;
		
		memcpy(m_pchData, pOldData->data(), (nOldLen + 1) * sizeof(char));
		GetData()->nDataLength = nOldLen;
		CStringA::Release(pOldData);
	}
	ATLASSERT(GetData()->nRefs <= 1);
	
	// return a pointer to the character storage for this string
	ATLASSERT(m_pchData != NULL);
	return m_pchData;
}

void CStringA::ReleaseBuffer(int nNewLength)
{
	CopyBeforeWrite();  // just in case GetBuffer was not called
	
	if (nNewLength == -1)
		nNewLength = SafeStrlenA(m_pchData); // zero terminated
	
	ATLASSERT(nNewLength <= GetData()->nAllocLength);
	GetData()->nDataLength = nNewLength;
	m_pchData[nNewLength] = '\0';
}

LPSTR CStringA::GetBufferSetLength(int nNewLength)
{
	ATLASSERT(nNewLength >= 0);
	
	if(GetBuffer(nNewLength) == NULL)
		return NULL;
	
	GetData()->nDataLength = nNewLength;
	m_pchData[nNewLength] = '\0';
	return m_pchData;
}

void CStringA::FreeExtra()
{
	ATLASSERT(GetData()->nDataLength <= GetData()->nAllocLength);
	if (GetData()->nDataLength != GetData()->nAllocLength)
	{
		CStringDataA* pOldData = GetData();
		if(AllocBuffer(GetData()->nDataLength))
		{
			memcpy(m_pchData, pOldData->data(), pOldData->nDataLength * sizeof(char));
			ATLASSERT(m_pchData[GetData()->nDataLength] == '\0');
			CStringA::Release(pOldData);
		}
	}
	ATLASSERT(GetData() != NULL);
}

LPSTR CStringA::LockBuffer()
{
	LPSTR lpsz = GetBuffer(0);
	if(lpsz != NULL)
		GetData()->nRefs = -1;
	return lpsz;
}

void CStringA::UnlockBuffer()
{
	ATLASSERT(GetData()->nRefs == -1);
	if (GetData() != _atltmpDataNilA)
		GetData()->nRefs = 1;
}

int CStringA::Find(char ch) const
{
	return Find(ch, 0);
}

int CStringA::Find(char ch, int nStart) const
{
	int nLength = GetData()->nDataLength;
	if (nStart >= nLength)
		return -1;
	
	// find first single character
	LPCSTR lpsz = _cstrchr(m_pchData + nStart, ch);
	
	// return -1 if not found and index otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

int CStringA::FindOneOf(LPCSTR lpszCharSet) const
{
	ATLASSERT(_IsValidString(lpszCharSet));
	LPCSTR lpsz = _cstrpbrk(m_pchData, lpszCharSet);
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

void CStringA::MakeUpper()
{
	CopyBeforeWrite();
	CharUpperA(m_pchData);
}

void CStringA::MakeLower()
{
	CopyBeforeWrite();
	CharLowerA(m_pchData);
}

void CStringA::MakeReverse()
{
	CopyBeforeWrite();
	_cstrrev(m_pchData);
}

void CStringA::SetAt(int nIndex, char ch)
{
	ATLASSERT(nIndex >= 0);
	ATLASSERT(nIndex < GetData()->nDataLength);
	
	CopyBeforeWrite();
	m_pchData[nIndex] = ch;
}

CStringA CStringA::Mid(int nFirst) const
{
	return Mid(nFirst, GetData()->nDataLength - nFirst);
}

CStringA CStringA::Mid(int nFirst, int nCount) const
{
	// out-of-bounds requests return sensible things
	if (nFirst < 0)
		nFirst = 0;
	if (nCount < 0)
		nCount = 0;
	
	if (nFirst + nCount > GetData()->nDataLength)
		nCount = GetData()->nDataLength - nFirst;
	if (nFirst > GetData()->nDataLength)
		nCount = 0;
	
	CStringA dest;
	AllocCopy(dest, nCount, nFirst, 0);
	return dest;
}

CStringA CStringA::Right(int nCount) const
{
	if (nCount < 0)
		nCount = 0;
	else if (nCount > GetData()->nDataLength)
		nCount = GetData()->nDataLength;
	
	CStringA dest;
	AllocCopy(dest, nCount, GetData()->nDataLength-nCount, 0);
	return dest;
}

CStringA CStringA::Left(int nCount) const
{
	if (nCount < 0)
		nCount = 0;
	else if (nCount > GetData()->nDataLength)
		nCount = GetData()->nDataLength;
	
	CStringA dest;
	AllocCopy(dest, nCount, 0, 0);
	return dest;
}

bool CStringA::LeftIs( LPCSTR str )
{
	int nLen = SafeStrlenA( str );
	return (0 == Left(nLen).Compare( str));
}

// strspn equivalent
CStringA CStringA::SpanIncluding(LPCSTR lpszCharSet) const
{
	ATLASSERT(_IsValidString(lpszCharSet));
	return Left(_cstrspn(m_pchData, lpszCharSet));
}

// strcspn equivalent
CStringA CStringA::SpanExcluding(LPCSTR lpszCharSet) const
{
	ATLASSERT(_IsValidString(lpszCharSet));
	return Left(_cstrcspn(m_pchData, lpszCharSet));
}

int CStringA::ReverseFind(char ch) const
{
	// find last single character
	LPCSTR lpsz = _cstrrchr(m_pchData, ch);
	
	// return -1 if not found, distance from beginning otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

// find a sub-string (like strstr)
int CStringA::Find(LPCSTR lpszSub) const
{
	return Find(lpszSub, 0);
}

int CStringA::Find(LPCSTR lpszSub, int nStart) const
{
	ATLASSERT(_IsValidString(lpszSub));
	
	int nLength = GetData()->nDataLength;
	if (nStart > nLength)
		return -1;
	
	// find first matching substring
	LPCSTR lpsz = _cstrstr(m_pchData + nStart, lpszSub);
	
	// return -1 for not found, distance from beginning otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

BOOL CStringA::FormatV(LPCSTR lpszFormat, va_list argList)
{
	ATLASSERT(_IsValidString(lpszFormat));
	
	enum _FormatModifiers
	{
		FORCE_ANSI =	0x10000,
			FORCE_UNICODE =	0x20000,
			FORCE_INT64 =	0x40000
	};
	
	va_list argListSave = argList;
	
	// make a guess at the maximum length of the resulting string
	int nMaxLen = 0;
	for (LPCSTR lpsz = lpszFormat; *lpsz != '\0'; lpsz = ::CharNextA(lpsz))
	{
		// handle '%' character, but watch out for '%%'
		if (*lpsz != '%' || *(lpsz = ::CharNextA(lpsz)) == '%')
		{
			nMaxLen += (int)(::CharNextA(lpsz) - lpsz);
			continue;
		}
		
		int nItemLen = 0;
		
		// handle '%' character with format
		int nWidth = 0;
		for (; *lpsz != '\0'; lpsz = ::CharNextA(lpsz))
		{
			// check for valid flags
			if (*lpsz == '#')
				nMaxLen += 2;   // for '0x'
			else if (*lpsz == '*')
				nWidth = va_arg(argList, int);
			else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' || *lpsz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = _cstrtoi(lpsz);
			for (; *lpsz != '\0' && _cstrisdigit(*lpsz); lpsz = ::CharNextA(lpsz))
				;
		}
		ATLASSERT(nWidth >= 0);
		
		int nPrecision = 0;
		if (*lpsz == '.')
		{
			// skip past '.' separator (width.precision)
			lpsz = ::CharNextA(lpsz);
			
			// get precision and skip it
			if (*lpsz == '*')
			{
				nPrecision = va_arg(argList, int);
				lpsz = ::CharNextA(lpsz);
			}
			else
			{
				nPrecision = _cstrtoi(lpsz);
				for (; *lpsz != '\0' && _cstrisdigit(*lpsz); lpsz = ::CharNextA(lpsz))
					;
			}
			ATLASSERT(nPrecision >= 0);
		}
		
		// should be on type modifier or specifier
		int nModifier = 0;
		if(lpsz[0] == _T('I') && lpsz[1] == _T('6') && lpsz[2] == _T('4'))
		{
			lpsz += 3;
			nModifier = FORCE_INT64;
		}
		else
		{
			switch (*lpsz)
			{
				// modifiers that affect size
			case 'h':
				nModifier = FORCE_ANSI;
				lpsz = ::CharNextA(lpsz);
				break;
			case 'l':
				nModifier = FORCE_UNICODE;
				lpsz = ::CharNextA(lpsz);
				break;
				
				// modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				lpsz = ::CharNextA(lpsz);
				break;
			}
		}
		
		// now should be on specifier
		switch (*lpsz | nModifier)
		{
			// single characters
		case 'c':
		case 'C':
			nItemLen = 2;
			va_arg(argList, char);
			break;
		case 'c' | FORCE_ANSI:
		case 'C' | FORCE_ANSI:
			nItemLen = 2;
			va_arg(argList, char);
			break;
		case 'c' | FORCE_UNICODE:
		case 'C' | FORCE_UNICODE:
			nItemLen = 2;
			va_arg(argList, WCHAR);
			break;
			
			// strings
		case 's':
			{
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6;  // "(null)"
				}
				else
				{
					nItemLen = SafeStrlenA(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
				break;
			}
			
		case 'S':
			{
				LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6;  // "(null)"
				}
				else
				{
					nItemLen = (int)wcslen(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
				break;
			}
			
		case 's' | FORCE_ANSI:
		case 'S' | FORCE_ANSI:
			{
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6; // "(null)"
				}
				else
				{
					nItemLen = lstrlenA(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
				break;
			}
			
		case 's' | FORCE_UNICODE:
		case 'S' | FORCE_UNICODE:
			{
				LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6; // "(null)"
				}
				else
				{
					nItemLen = (int)wcslen(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
				break;
			}
		}
		
		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
			nItemLen = max(nItemLen, nWidth);
			if (nPrecision != 0)
				nItemLen = min(nItemLen, nPrecision);
		}
		else
		{
			switch (*lpsz)
			{
				// integers
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				if (nModifier & FORCE_INT64)
					va_arg(argList, __int64);
				else
					va_arg(argList, int);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth + nPrecision);
				break;

			case 'e':
			case 'g':
			case 'G':
				va_arg(argList, double);
				nItemLen = 128;
				nItemLen = max(nItemLen, nWidth + nPrecision);
				break;
			case 'f':
				{
					double f;
					LPTSTR pszTemp;
					
					// 312 == strlen("-1+(309 zeroes).")
					// 309 zeroes == max precision of a double
					// 6 == adjustment in case precision is not specified,
					//   which means that the precision defaults to 6
					pszTemp = (LPTSTR)_alloca(max(nWidth, 312 + nPrecision + 6));
					
					f = va_arg(argList, double);
					_stprintf(pszTemp, _T( "%*.*f" ), nWidth, nPrecision + 6, f);
					nItemLen = _tcslen(pszTemp);
				}
				break;
				
			case 'p':
				va_arg(argList, void*);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth + nPrecision);
				break;
				
				// no output
			case 'n':
				va_arg(argList, int*);
				break;
				
			default:
				ATLASSERT(FALSE);  // unknown formatting option
			}
		}
		
		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}
	
	if(GetBuffer(nMaxLen) == NULL)
		return FALSE;

	int nRet = vsprintf(m_pchData, lpszFormat, argListSave);

	nRet;	// ref
	ATLASSERT(nRet <= GetAllocLength());
	ReleaseBuffer();
	
	va_end(argListSave);
	return TRUE;
}

// formatting (using wsprintf style formatting)
BOOL __cdecl CStringA::Format(LPCSTR lpszFormat, ...)
{
	ATLASSERT(_IsValidString(lpszFormat));
	
	va_list argList;
	va_start(argList, lpszFormat);
	BOOL bRet = FormatV(lpszFormat, argList);
	va_end(argList);
	return bRet;
}

BOOL __cdecl CStringA::Format(UINT nFormatID, ...)
{
	CStringA strFormat;
	BOOL bRet = strFormat.LoadString(nFormatID);
	ATLASSERT(bRet != 0);
	
	va_list argList;
	va_start(argList, nFormatID);
	bRet = FormatV(strFormat, argList);
	va_end(argList);
	return bRet;
}

// formatting (using FormatMessage style formatting)
BOOL __cdecl CStringA::FormatMessage(LPCSTR lpszFormat, ...)
{
	// format message into temporary buffer lpszTemp
	va_list argList;
	va_start(argList, lpszFormat);
	LPSTR lpszTemp;
	BOOL bRet = TRUE;
	
	if (::FormatMessageA(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		lpszFormat, 0, 0, (LPSTR)&lpszTemp, 0, &argList) == 0 || lpszTemp == NULL)
		bRet = FALSE;
	
	// assign lpszTemp into the resulting string and free the temporary
	*this = lpszTemp;
	LocalFree(lpszTemp);
	va_end(argList);
	return bRet;
}

BOOL __cdecl CStringA::FormatMessage(UINT nFormatID, ...)
{
	// get format string from string table
	CStringA strFormat;
	BOOL bRetTmp = strFormat.LoadString(nFormatID);
	bRetTmp;	// ref
	ATLASSERT(bRetTmp != 0);
	
	// format message into temporary buffer lpszTemp
	va_list argList;
	va_start(argList, nFormatID);
	LPSTR lpszTemp;
	BOOL bRet = TRUE;
	
	if (::FormatMessageA(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		strFormat, 0, 0, (LPSTR)&lpszTemp, 0, &argList) == 0 || lpszTemp == NULL)
		bRet = FALSE;
	
	// assign lpszTemp into the resulting string and free lpszTemp
	*this = lpszTemp;
	LocalFree(lpszTemp);
	va_end(argList);
	return bRet;
}

void CStringA::TrimRight()
{
	CopyBeforeWrite();
	
	// find beginning of trailing spaces by starting at beginning (DBCS aware)
	LPSTR lpsz = m_pchData;
	LPSTR lpszLast = NULL;
	while (*lpsz != '\0')
	{
		if (_cstrisspace(*lpsz))
		{
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
		{
			lpszLast = NULL;
		}
		lpsz = ::CharNextA(lpsz);
	}
	
	if (lpszLast != NULL)
	{
		// truncate at trailing space start
		*lpszLast = '\0';
		GetData()->nDataLength = (int)(ADWORD_PTR)(lpszLast - m_pchData);
	}
}

void CStringA::TrimLeft()
{
	CopyBeforeWrite();
	
	// find first non-space character
	LPCSTR lpsz = m_pchData;
	while (_cstrisspace(*lpsz))
		lpsz = ::CharNextA(lpsz);
	
	// fix up data and length
	int nDataLength = GetData()->nDataLength - (int)(ADWORD_PTR)(lpsz - m_pchData);
	memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(char));
	GetData()->nDataLength = nDataLength;
}

void CStringA::TrimRight(LPCSTR lpszTargetList)
{
	// find beginning of trailing matches
	// by starting at beginning (DBCS aware)
	
	CopyBeforeWrite();
	LPSTR lpsz = m_pchData;
	LPSTR lpszLast = NULL;
	
	while (*lpsz != '\0')
	{
		if (_cstrchr(lpszTargetList, *lpsz) != NULL)
		{
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
			lpszLast = NULL;
		lpsz = ::CharNextA(lpsz);
	}
	
	if (lpszLast != NULL)
	{
		// truncate at left-most matching character
		*lpszLast = '\0';
		GetData()->nDataLength = (int)(ADWORD_PTR)(lpszLast - m_pchData);
	}
}

void CStringA::TrimRight(char chTarget)
{
	// find beginning of trailing matches
	// by starting at beginning (DBCS aware)
	
	CopyBeforeWrite();
	LPSTR lpsz = m_pchData;
	LPSTR lpszLast = NULL;
	
	while (*lpsz != '\0')
	{
		if (*lpsz == chTarget)
		{
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
			lpszLast = NULL;
		lpsz = ::CharNextA(lpsz);
	}
	
	if (lpszLast != NULL)
	{
		// truncate at left-most matching character
		*lpszLast = '\0';
		GetData()->nDataLength = (int)(ADWORD_PTR)(lpszLast - m_pchData);
	}
}

void CStringA::TrimLeft(LPCSTR lpszTargets)
{
	// if we're not trimming anything, we're not doing any work
	if (SafeStrlenA(lpszTargets) == 0)
		return;
	
	CopyBeforeWrite();
	LPCSTR lpsz = m_pchData;
	
	while (*lpsz != '\0')
	{
		if (_cstrchr(lpszTargets, *lpsz) == NULL)
			break;
		lpsz = ::CharNextA(lpsz);
	}
	
	if (lpsz != m_pchData)
	{
		// fix up data and length
		int nDataLength = GetData()->nDataLength - (int)(ADWORD_PTR)(lpsz - m_pchData);
		memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(char));
		GetData()->nDataLength = nDataLength;
	}
}

void CStringA::TrimLeft(char chTarget)
{
	// find first non-matching character
	
	CopyBeforeWrite();
	LPCSTR lpsz = m_pchData;
	
	while (chTarget == *lpsz)
		lpsz = ::CharNextA(lpsz);
	
	if (lpsz != m_pchData)
	{
		// fix up data and length
		int nDataLength = GetData()->nDataLength - (int)(ADWORD_PTR)(lpsz - m_pchData);
		memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(char));
		GetData()->nDataLength = nDataLength;
	}
}

int CStringA::Delete(int nIndex, int nCount /* = 1 */)
{
	if (nIndex < 0)
		nIndex = 0;
	int nNewLength = GetData()->nDataLength;
	if (nCount > 0 && nIndex < nNewLength)
	{
		CopyBeforeWrite();
		int nBytesToCopy = nNewLength - (nIndex + nCount) + 1;
		
		memmove(m_pchData + nIndex, m_pchData + nIndex + nCount, nBytesToCopy * sizeof(char));
		GetData()->nDataLength = nNewLength - nCount;
	}
	
	return nNewLength;
}

int CStringA::Add( char ch )
{
	return Insert( GetLength(), ch );
}

int CStringA::Insert(int nIndex, char ch)
{
	CopyBeforeWrite();
	
	if (nIndex < 0)
		nIndex = 0;
	
	int nNewLength = GetData()->nDataLength;
	if (nIndex > nNewLength)
		nIndex = nNewLength;
	nNewLength++;
	
	if (GetData()->nAllocLength < nNewLength)
	{
		CStringDataA* pOldData = GetData();
		LPCSTR pstr = m_pchData;
		if(!AllocBuffer(nNewLength))
			return -1;
		memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(char));
		CStringA::Release(pOldData);
	}
	
	// move existing bytes down
	memmove(m_pchData + nIndex + 1, m_pchData + nIndex, (nNewLength - nIndex) * sizeof(char));
	m_pchData[nIndex] = ch;
	GetData()->nDataLength = nNewLength;
	
	return nNewLength;
}

int CStringA::Insert(int nIndex, LPCSTR pstr)
{
	if (nIndex < 0)
		nIndex = 0;
	
	int nInsertLength = SafeStrlenA(pstr);
	int nNewLength = GetData()->nDataLength;
	if (nInsertLength > 0)
	{
		CopyBeforeWrite();
		if (nIndex > nNewLength)
			nIndex = nNewLength;
		nNewLength += nInsertLength;
		
		if (GetData()->nAllocLength < nNewLength)
		{
			CStringDataA* pOldData = GetData();
			LPCSTR pstr = m_pchData;
			if(!AllocBuffer(nNewLength))
				return -1;
			memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(char));
			CStringA::Release(pOldData);
		}
		
		// move existing bytes down
		memmove(m_pchData + nIndex + nInsertLength, m_pchData + nIndex, (nNewLength - nIndex - nInsertLength + 1) * sizeof(char));
		memcpy(m_pchData + nIndex, pstr, nInsertLength * sizeof(char));
		GetData()->nDataLength = nNewLength;
	}
	
	return nNewLength;
}

int CStringA::Replace(char chOld, char chNew)
{
	int nCount = 0;
	
	// short-circuit the nop case
	if (chOld != chNew)
	{
		// otherwise modify each character that matches in the string
		CopyBeforeWrite();
		LPSTR psz = m_pchData;
		LPSTR pszEnd = psz + GetData()->nDataLength;
		while (psz < pszEnd)
		{
			// replace instances of the specified character only
			if (*psz == chOld)
			{
				*psz = chNew;
				nCount++;
			}
			psz = ::CharNextA(psz);
		}
	}
	return nCount;
}

int CStringA::Replace(LPCSTR lpszOld, LPCSTR lpszNew)
{
	// can't have empty or NULL lpszOld
	
	int nSourceLen = SafeStrlenA(lpszOld);
	if (nSourceLen == 0)
		return 0;
	int nReplacementLen = SafeStrlenA(lpszNew);
	
	// loop once to figure out the size of the result string
	int nCount = 0;
	LPSTR lpszStart = m_pchData;
	LPSTR lpszEnd = m_pchData + GetData()->nDataLength;
	LPSTR lpszTarget;
	while (lpszStart < lpszEnd)
	{
		while ((lpszTarget = _cstrstr(lpszStart, lpszOld)) != NULL)
		{
			nCount++;
			lpszStart = lpszTarget + nSourceLen;
		}
		lpszStart += SafeStrlenA(lpszStart) + 1;
	}
	
	// if any changes were made, make them
	if (nCount > 0)
	{
		CopyBeforeWrite();
		
		// if the buffer is too small, just
		//   allocate a new buffer (slow but sure)
		int nOldLength = GetData()->nDataLength;
		int nNewLength =  nOldLength + (nReplacementLen - nSourceLen) * nCount;
		if (GetData()->nAllocLength < nNewLength || GetData()->nRefs > 1)
		{
			CStringDataA* pOldData = GetData();
			LPCSTR pstr = m_pchData;
			if(!AllocBuffer(nNewLength))
				return -1;
			memcpy(m_pchData, pstr, pOldData->nDataLength * sizeof(char));
			CStringA::Release(pOldData);
		}
		// else, we just do it in-place
		lpszStart = m_pchData;
		lpszEnd = m_pchData + GetData()->nDataLength;
		
		// loop again to actually do the work
		while (lpszStart < lpszEnd)
		{
			while ( (lpszTarget = _cstrstr(lpszStart, lpszOld)) != NULL)
			{
				int nBalance = nOldLength - ((int)(ADWORD_PTR)(lpszTarget - m_pchData) + nSourceLen);
				memmove((void*)(lpszTarget + nReplacementLen), lpszTarget + nSourceLen, nBalance * sizeof(char));
				memcpy((void*)lpszTarget, lpszNew, nReplacementLen * sizeof(char));
				lpszStart = lpszTarget + nReplacementLen;
				lpszStart[nBalance] = '\0';
				nOldLength += (nReplacementLen - nSourceLen);
			}
			lpszStart += SafeStrlenA(lpszStart) + 1;
		}
		ATLASSERT(m_pchData[nNewLength] == '\0');
		GetData()->nDataLength = nNewLength;
	}
	
	return nCount;
}

int CStringA::Remove(char chRemove)
{
	CopyBeforeWrite();
	
	LPSTR pstrSource = m_pchData;
	LPSTR pstrDest = m_pchData;
	LPSTR pstrEnd = m_pchData + GetData()->nDataLength;
	
	while (pstrSource < pstrEnd)
	{
		if (*pstrSource != chRemove)
		{
			*pstrDest = *pstrSource;
			pstrDest = ::CharNextA(pstrDest);
		}
		pstrSource = ::CharNextA(pstrSource);
	}
	*pstrDest = '\0';
	int nCount = (int)(ADWORD_PTR)(pstrSource - pstrDest);
	GetData()->nDataLength -= nCount;
	
	return nCount;
}

// #ifdef _UNICODE
// #define CHAR_FUDGE 1    // one char unused is good enough
// #else
// #define CHAR_FUDGE 2    // two BYTES unused for case of DBC last char
// #endif

BOOL CStringA::LoadString(UINT nID)
{
	// try fixed buffer first (to avoid wasting space in the heap)
	char szTemp[256];
	int nCount =  sizeof(szTemp) / sizeof(szTemp[0]);
	int nLen = _LoadStringA(nID, szTemp, nCount);
	if (nCount - nLen > 1)
	{
		*this = szTemp;
		return (nLen > 0);
	}
	
	// try buffer size of 512, then larger size until entire string is retrieved
	int nSize = 256;
	do
	{
		nSize += 256;
		LPCSTR lpstr = GetBuffer(nSize - 1);
		if(lpstr == NULL)
		{
			nLen = 0;
			break;
		}
		nLen = _LoadStringA(nID, lpstr, nSize);
	} while (nSize - nLen <= 1);
	ReleaseBuffer();
	
	return (nLen > 0);
}

#ifndef _ATL_NO_COM
BSTR CStringA::AllocSysString() const
{
	//#if defined(_UNICODE) || defined(OLE2ANSI)
	//	BSTR bstr = ::SysAlloCStringALen(m_pchData, GetData()->nDataLength);
	//#else
	int nLen = MultiByteToWideChar(CP_ACP, 0, m_pchData,
		GetData()->nDataLength, NULL, NULL);
	BSTR bstr = ::SysAllocStringLen(NULL, nLen);
	if(bstr != NULL)
		MultiByteToWideChar(CP_ACP, 0, m_pchData, GetData()->nDataLength, bstr, nLen);
	// #endif
	return bstr;
}

BSTR CStringA::SetSysString(BSTR* pbstr) const
{
	//#if defined(_UNICODE) || defined(OLE2ANSI)
	//	::SysReAlloCStringALen(pbstr, m_pchData, GetData()->nDataLength);
	//#else
	int nLen = MultiByteToWideChar(CP_ACP, 0, m_pchData,
		GetData()->nDataLength, NULL, NULL);
	if(::SysReAllocStringLen(pbstr, NULL, nLen))
		MultiByteToWideChar(CP_ACP, 0, m_pchData, GetData()->nDataLength, *pbstr, nLen);
	//#endif
	ATLASSERT(*pbstr != NULL);
	return *pbstr;
}

#endif //!_ATL_NO_COM


//////////////////////////////////////////////////////////////////////////
//友元 字符串连接
CStringA __stdcall operator+(const CStringA& string, char ch)
{ 
	CStringA s = string;
	s += ch;
	return s;
}

CStringA __stdcall operator+(char ch, const CStringA& string)
{ 
	CStringA s(ch);
	return s + string; 
}

CStringA __stdcall operator+(const CStringA& string1, const CStringA& string2)
{
	CStringA s;
	s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData, string2.GetData()->nDataLength, string2.m_pchData);
	return s;
}

CStringA __stdcall operator+(const CStringA& string, LPCSTR lpsz)
{
	ATLASSERT(lpsz == NULL || _IsValidString(lpsz));
	CStringA s;
	s.ConcatCopy(string.GetData()->nDataLength, string.m_pchData, SafeStrlenA(lpsz), lpsz);
	return s;
}

CStringA __stdcall operator+(LPCSTR lpsz, const CStringA& string)
{
	ATLASSERT(lpsz == NULL || _IsValidString(lpsz));
	CStringA s;
	s.ConcatCopy(SafeStrlenA(lpsz), lpsz, string.GetData()->nDataLength, string.m_pchData);
	return s;
}

//////////////////////////////////////////////////////////////////////////
//字符串比较
bool __stdcall operator==(const CStringA& s1, const CStringA& s2)
{ return s1.Compare(s2) == 0; }
bool __stdcall operator==(const CStringA& s1, LPCSTR s2)
{ return s1.Compare(s2) == 0; }
bool __stdcall operator==(LPCSTR s1, const CStringA& s2)
{ return s2.Compare(s1) == 0; }
bool __stdcall operator!=(const CStringA& s1, const CStringA& s2)
{ return s1.Compare(s2) != 0; }
bool __stdcall operator!=(const CStringA& s1, LPCSTR s2)
{ return s1.Compare(s2) != 0; }
bool __stdcall operator!=(LPCSTR s1, const CStringA& s2)
{ return s2.Compare(s1) != 0; }
bool __stdcall operator<(const CStringA& s1, const CStringA& s2)
{ return s1.Compare(s2) < 0; }
bool __stdcall operator<(const CStringA& s1, LPCSTR s2)
{ return s1.Compare(s2) < 0; }
bool __stdcall operator<(LPCSTR s1, const CStringA& s2)
{ return s2.Compare(s1) > 0; }
bool __stdcall operator>(const CStringA& s1, const CStringA& s2)
{ return s1.Compare(s2) > 0; }
bool __stdcall operator>(const CStringA& s1, LPCSTR s2)
{ return s1.Compare(s2) > 0; }
bool __stdcall operator>(LPCSTR s1, const CStringA& s2)
{ return s2.Compare(s1) < 0; }
bool __stdcall operator<=(const CStringA& s1, const CStringA& s2)
{ return s1.Compare(s2) <= 0; }
bool __stdcall operator<=(const CStringA& s1, LPCSTR s2)
{ return s1.Compare(s2) <= 0; }
bool __stdcall operator<=(LPCSTR s1, const CStringA& s2)
{ return s2.Compare(s1) >= 0; }
bool __stdcall operator>=(const CStringA& s1, const CStringA& s2)
{ return s1.Compare(s2) >= 0; }
bool __stdcall operator>=(const CStringA& s1, LPCSTR s2)
{ return s1.Compare(s2) >= 0; }
bool __stdcall operator>=(LPCSTR s1, const CStringA& s2)
{ return s2.Compare(s1) <= 0; }


//////////////////////////////////////////////////////////////////////////
//CStringW 实现

CStringW::~CStringW()
//  free any attached data
{
	if (GetData() != _atltmpDataNilW)
	{
		if (InterlockedDecrement((long*)&GetData()->nRefs) <= 0)
			delete[] (BYTE*)GetData();
	}
}

void CStringW::Init()
{ 
	m_pchData = _GetEmptyString().m_pchData; 
}

void CStringW::Release()
{
	if (GetData() != _atltmpDataNilW)
	{
		ATLASSERT(GetData()->nRefs != 0);
		if (InterlockedDecrement((long*)&GetData()->nRefs) <= 0)
			delete[] (BYTE*)GetData();
		Init();
	}
}

void __stdcall CStringW::Release( CStringDataW* pData )
{
	if (pData != _atltmpDataNilW)
	{
		ATLASSERT(pData->nRefs != 0);
		if (InterlockedDecrement((long*)&pData->nRefs) <= 0)
			delete[] (BYTE*)pData;
	}
}

const CStringW& __stdcall CStringW::_GetEmptyString()
{
	return *(CStringW*)&_atltmpPchNilW;
}

CStringW::CStringDataW* CStringW::GetData() const
{ 
	ATLASSERT(m_pchData != NULL); 
	return ((CStringW::CStringDataW*)m_pchData) - 1;
}

//构造
CStringW::CStringW()
{
	Init();
}

CStringW::CStringW(const CStringW& stringSrc)
{
	ATLASSERT(stringSrc.GetData()->nRefs != 0);
	if (stringSrc.GetData()->nRefs >= 0)
	{
		ATLASSERT(stringSrc.GetData() != _atltmpDataNilW);
		m_pchData = stringSrc.m_pchData;
		InterlockedIncrement((long*)&GetData()->nRefs);
	}
	else
	{
		Init();
		*this = stringSrc.m_pchData;
	}
}

CStringW::CStringW(LPCSTR lpsz)
{
	Init();
	int nSrcLen = SafeStrlenA(lpsz);
	if (nSrcLen != 0)
	{
		if(AllocBuffer(nSrcLen))
		{			
			_mbstowcsz(m_pchData, lpsz, nSrcLen+1);
			ReleaseBuffer();
		}
	}
}

CStringW::CStringW(LPCWSTR lpsz)
{
	Init();
	if (lpsz != NULL && HIWORD(lpsz) == NULL)
	{
		UINT nID = LOWORD((ADWORD_PTR)lpsz);
		LoadString(nID);
	}
	else
	{
		int nLen = SafeStrlenW(lpsz);
		if (nLen != 0)
		{
			if(AllocBuffer(nLen))
				memcpy(m_pchData, lpsz, nLen * sizeof(wchar_t));
		}
	}
}

CStringW::CStringW(wchar_t ch, int nLength)
{
	ATLASSERT(!_istlead(ch));    // can't create a lead byte string
	Init();
	if (nLength >= 1)
	{
		if(AllocBuffer(nLength))
		{
			for (int i = 0; i < nLength; i++)
				m_pchData[i] = ch;
		}
	}
}

CStringW::CStringW(LPCSTR lpch, int nLength)
{
	Init();
	if (nLength != 0)
	{
		if(AllocBuffer(nLength))
		{
			_mbstowcsz(m_pchData, lpch, nLength+1);
		}
	}
}

CStringW::CStringW(LPCWSTR lpsz, int nLength)
{
	Init();
	if (nLength != 0)
	{
		if(AllocBuffer(nLength))
			memcpy(m_pchData, lpsz, nLength * sizeof(wchar_t));
	}
}

//////////////////////////////////////////////////////////////////////////
//赋值运算符

const CStringW& CStringW::operator=(const unsigned char* lpsz)
{ 
	*this = (LPCSTR)lpsz; 
	return *this; 
}


const CStringW& CStringW::operator+=(wchar_t ch)
{ 
	ConcatInPlace(1, &ch);
	return *this; 
}

const CStringW& CStringW::operator=(wchar_t ch)
{ 
	AssignCopy(1, &ch);
	return *this; 
}

const CStringW& CStringW::operator=(const CStringW& stringSrc)
{
	if (m_pchData != stringSrc.m_pchData)
	{
		if ((GetData()->nRefs < 0 && GetData() != _atltmpDataNilW) || stringSrc.GetData()->nRefs < 0)
		{
			// actual copy necessary since one of the strings is locked
			AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
		}
		else
		{
			// can just copy references around
			Release();
			ATLASSERT(stringSrc.GetData() != _atltmpDataNilW);
			m_pchData = stringSrc.m_pchData;
			InterlockedIncrement((long*)&GetData()->nRefs);
		}
	}
	return *this;
}

const CStringW& CStringW::operator=(LPCSTR lpsz)
{
	int nSrcLen = SafeStrlenA(lpsz);
	int bytes =	BytesOfA2W(lpsz, nSrcLen);
	if(AllocBeforeWrite(bytes))
	{
		_mbstowcsz(m_pchData, lpsz, nSrcLen + 1);

		ReleaseBuffer();

		ZeroAfter(nSrcLen);
	}
	return *this;	
}

const CStringW& CStringW::operator=(LPCWSTR lpsz)
{

	ATLASSERT(lpsz == NULL || _IsValidString(lpsz));
	AssignCopy(SafeStrlenW(lpsz), lpsz);
	return *this;
}

const CStringW& CStringW::operator+=(LPCWSTR lpsz)
{
	ATLASSERT(lpsz == NULL || _IsValidString(lpsz));
	ConcatInPlace(SafeStrlenW(lpsz), lpsz);
	return *this;
}


const CStringW& CStringW::operator+=(const CStringW& string)
{
	ConcatInPlace(string.GetData()->nDataLength, string.m_pchData);
	return *this;
}

// const CStringW& CStringW::operator=(wchar_t ch)
// {
// 	AssignCopy(sizeof(wchar_t), &ch);
// 	return *this;
// }

//////////////////////////////////////////////////////////////////////////

int CStringW::GetLength() const
{ 
	return GetData()->nDataLength; 
}

int CStringW::GetAllocLength() const
{ 
	return GetData()->nAllocLength; 
}

BOOL CStringW::IsEmpty() const
{
	return GetData()->nDataLength == 0; 
}

CStringW::operator LPCWSTR() const
{ 
	return m_pchData; 
}

CStringW::operator wchar_t*() const
{ 
	return (wchar_t *)m_pchData; 
}

int CStringW::Compare(LPCWSTR lpsz) const
{ 
	return _cstrcmp(m_pchData, lpsz); // MBCS/Unicode aware
}   

int CStringW::CompareNoCase(LPCWSTR lpsz) const
{ 
	return _cstrcmpi(m_pchData, lpsz);  // MBCS/Unicode aware
}  

// Collate is often slower than Compare but is MBSC/Unicode
//  aware as well as locale-sensitive with respect to sort order.
int CStringW::Collate(LPCWSTR lpsz) const
{ 
	return _cstrcoll(m_pchData, lpsz);  // locale sensitive
} 
 
int CStringW::CollateNoCase(LPCWSTR lpsz) const
{ 
	return _cstrcolli(m_pchData, lpsz);// locale sensitive
 }   

wchar_t CStringW::GetAt(int nIndex) const
{
	ATLASSERT(nIndex >= 0);
	ATLASSERT(nIndex < GetData()->nDataLength);
	return m_pchData[nIndex];
}

wchar_t CStringW::operator[](int nIndex) const
{
	// same as GetAt
	ATLASSERT(nIndex >= 0);
	ATLASSERT(nIndex < GetData()->nDataLength);
	return m_pchData[nIndex];
}


BOOL CStringW::AllocBuffer(int nLen)
// always allocate one extra character for '\0' termination
// assumes [optimistically] that data length will equal allocation length
{
	ATLASSERT(nLen >= 0);
	ATLASSERT(nLen <= INT_MAX - 1);    // max size (enough room for 1 extra)
	
	if (nLen == 0)
	{
		Init();
	}
	else
	{
		CStringDataW* pData = NULL;
		ATLTRY(pData = (CStringDataW*)new BYTE[sizeof(CStringDataW) + (nLen + 1) * sizeof(wchar_t)]);
		if(pData == NULL)
			return FALSE;
		
		pData->nRefs = 1;
		pData->data()[nLen] = '\0';
		pData->nDataLength = nLen;
		pData->nAllocLength = nLen;
		m_pchData = pData->data();
	}
	
	return TRUE;
}

void CStringW::Empty()
{
	if (GetData()->nDataLength == 0)
		return;
	
	if (GetData()->nRefs >= 0)
		Release();
	else
		*this = _T("");
	
	ATLASSERT(GetData()->nDataLength == 0);
	ATLASSERT(GetData()->nRefs < 0 || GetData()->nAllocLength == 0);
}

void CStringW::CopyBeforeWrite()
{
	if (GetData()->nRefs > 1)
	{
		CStringDataW* pData = GetData();
		Release();
		if(AllocBuffer(pData->nDataLength)){
			memcpy(m_pchData, pData->data(), (pData->nDataLength + 1) * sizeof(wchar_t));	
		}
	}
	ATLASSERT(GetData()->nRefs <= 1);
}

BOOL CStringW::AllocBeforeWrite(int nLen)
{
	BOOL bRet = TRUE;
	if (GetData()->nRefs > 1 || nLen > GetData()->nAllocLength)
	{
		Release();
		bRet = AllocBuffer(nLen);
	}
	ATLASSERT(GetData()->nRefs <= 1);
	return bRet;
}

void CStringW::AllocCopy(CStringW& dest, int nCopyLen, int nCopyIndex,
								 int nExtraLen) const
{
	// will clone the data attached to this string
	// allocating 'nExtraLen' characters
	// Places results in uninitialized string 'dest'
	// Will copy the part or all of original data to start of new string
	
	int nNewLen = nCopyLen + nExtraLen;
	if (nNewLen == 0)
	{
		dest.Init();
	}
	else
	{
		if(dest.AllocBuffer(nNewLen))
			memcpy(dest.m_pchData, m_pchData + nCopyIndex, nCopyLen * sizeof(wchar_t));
	}
}


// Assignment operators
//  All assign a new value to the string
//      (a) first see if the buffer is big enough
//      (b) if enough room, copy on top of old buffer, set size and type
//      (c) otherwise free old string data, and create a new one
//
//  All routines return the new string (but as a 'const CStringW&' so that
//      assigning it again will cause a copy, eg: s1 = s2 = "hi there".
//
void CStringW::AssignCopy(int nSrcLen, LPCWSTR lpszSrcData)
{
	if(AllocBeforeWrite(nSrcLen))
	{
		CStringDataW* data = GetData();
		if (nSrcLen == 0){
			memset(m_pchData, 0, data->nDataLength);
		}
		else{
			memcpy(m_pchData, lpszSrcData, nSrcLen * sizeof(wchar_t));

			ZeroAfter(nSrcLen);
		}
		GetData()->nDataLength = nSrcLen;
		m_pchData[nSrcLen] = '\0';
	}
}

// Concatenation
// NOTE: "operator+" is done as friend functions for simplicity
//      There are three variants:
//          CStringW + CStringW
// and for ? = wchar_t, LPCWSTR
//          CStringW + ?
//          ? + CStringW

BOOL CStringW::ConcatCopy(int nSrc1Len, LPCWSTR lpszSrc1Data,
								  int nSrc2Len, LPCWSTR lpszSrc2Data)
{
	// -- master concatenation routine
	// Concatenate two sources
	// -- assume that 'this' is a new CStringW object
	
	BOOL bRet = TRUE;
	int nNewLen = nSrc1Len + nSrc2Len;
	if (nNewLen != 0)
	{
		bRet = AllocBuffer(nNewLen);
		if (bRet)
		{
			memcpy(m_pchData, lpszSrc1Data, nSrc1Len * sizeof(wchar_t));
			memcpy(m_pchData + nSrc1Len, lpszSrc2Data, nSrc2Len * sizeof(wchar_t));
		}
	}
	return bRet;
}


void CStringW::ConcatInPlace(int nSrcLen, LPCWSTR lpszSrcData)
{
	//  -- the main routine for += operators
	
	// concatenating an empty string is a no-op!
	if (nSrcLen == 0)
		return;
	
	// if the buffer is too small, or we have a width mis-match, just
	//   allocate a new buffer (slow but sure)
	if (GetData()->nRefs > 1 || GetData()->nDataLength + nSrcLen > GetData()->nAllocLength)
	{
		// we have to grow the buffer, use the ConcatCopy routine
		CStringDataW* pOldData = GetData();
		if (ConcatCopy(GetData()->nDataLength, m_pchData, nSrcLen, lpszSrcData))
		{
			ATLASSERT(pOldData != NULL);
			CStringW::Release(pOldData);
		}
	}
	else
	{
		// fast concatenation when buffer big enough
		memcpy(m_pchData + GetData()->nDataLength, lpszSrcData, nSrcLen * sizeof(wchar_t));
		GetData()->nDataLength += nSrcLen;
		ATLASSERT(GetData()->nDataLength <= GetData()->nAllocLength);
		m_pchData[GetData()->nDataLength] = '\0';
	}
}

LPWSTR CStringW::GetBuffer(int nMinBufLength)
{
	ATLASSERT(nMinBufLength >= 0);
	
	if (GetData()->nRefs > 1 || nMinBufLength > GetData()->nAllocLength)
	{
		// we have to grow the buffer
		CStringDataW* pOldData = GetData();
		int nOldLen = GetData()->nDataLength;   // AllocBuffer will tromp it
		if (nMinBufLength < nOldLen)
			nMinBufLength = nOldLen;
		
		if(!AllocBuffer(nMinBufLength))
			return NULL;
		
		memcpy(m_pchData, pOldData->data(), (nOldLen + 1) * sizeof(wchar_t));
		GetData()->nDataLength = nOldLen;
		CStringW::Release(pOldData);
	}
	ATLASSERT(GetData()->nRefs <= 1);
	
	// return a pointer to the character storage for this string
	ATLASSERT(m_pchData != NULL);
	return m_pchData;
}

void CStringW::ReleaseBuffer(int nNewLength)
{
	CopyBeforeWrite();  // just in case GetBuffer was not called
	
	if (nNewLength == -1)
		nNewLength = SafeStrlenW(m_pchData); // zero terminated
	
	ATLASSERT(nNewLength <= GetData()->nAllocLength);
	GetData()->nDataLength = nNewLength;
	m_pchData[nNewLength] = '\0';
}

LPWSTR CStringW::GetBufferSetLength(int nNewLength)
{
	ATLASSERT(nNewLength >= 0);
	
	if(GetBuffer(nNewLength) == NULL)
		return NULL;
	
	GetData()->nDataLength = nNewLength;
	m_pchData[nNewLength] = '\0';
	return m_pchData;
}

void CStringW::FreeExtra()
{
	ATLASSERT(GetData()->nDataLength <= GetData()->nAllocLength);
	if (GetData()->nDataLength != GetData()->nAllocLength)
	{
		CStringDataW* pOldData = GetData();
		if(AllocBuffer(GetData()->nDataLength))
		{
			memcpy(m_pchData, pOldData->data(), pOldData->nDataLength * sizeof(wchar_t));
			ATLASSERT(m_pchData[GetData()->nDataLength] == '\0');
			CStringW::Release(pOldData);
		}
	}
	ATLASSERT(GetData() != NULL);
}

LPWSTR CStringW::LockBuffer()
{
	LPWSTR lpsz = GetBuffer(0);
	if(lpsz != NULL)
		GetData()->nRefs = -1;
	return lpsz;
}

void CStringW::UnlockBuffer()
{
	ATLASSERT(GetData()->nRefs == -1);
	if (GetData() != _atltmpDataNilW)
		GetData()->nRefs = 1;
}

int CStringW::Find(wchar_t ch) const
{
	return Find(ch, 0);
}

int CStringW::Find(wchar_t ch, int nStart) const
{
	int nLength = GetData()->nDataLength;
	if (nStart >= nLength)
		return -1;
	
	// find first single character
	LPCWSTR lpsz = _cstrchr(m_pchData + nStart, ch);
	
	// return -1 if not found and index otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

int CStringW::FindOneOf(LPCWSTR lpszCharSet) const
{
	ATLASSERT(_IsValidString(lpszCharSet));
	LPCWSTR lpsz = _cstrpbrk(m_pchData, lpszCharSet);
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

void CStringW::MakeUpper()
{
	CopyBeforeWrite();
	CharUpperW(m_pchData);
}

void CStringW::MakeLower()
{
	CopyBeforeWrite();
	CharLowerW(m_pchData);
}

void CStringW::MakeReverse()
{
	CopyBeforeWrite();
	_cstrrev(m_pchData);
}

void CStringW::SetAt(int nIndex, wchar_t ch)
{
	ATLASSERT(nIndex >= 0);
	ATLASSERT(nIndex < GetData()->nDataLength);
	
	CopyBeforeWrite();
	m_pchData[nIndex] = ch;
}

CStringW CStringW::Mid(int nFirst) const
{
	return Mid(nFirst, GetData()->nDataLength - nFirst);
}

CStringW CStringW::Mid(int nFirst, int nCount) const
{
	// out-of-bounds requests return sensible things
	if (nFirst < 0)
		nFirst = 0;
	if (nCount < 0)
		nCount = 0;
	
	if (nFirst + nCount > GetData()->nDataLength)
		nCount = GetData()->nDataLength - nFirst;
	if (nFirst > GetData()->nDataLength)
		nCount = 0;
	
	CStringW dest;
	AllocCopy(dest, nCount, nFirst, 0);
	return dest;
}

CStringW CStringW::Right(int nCount) const
{
	if (nCount < 0)
		nCount = 0;
	else if (nCount > GetData()->nDataLength)
		nCount = GetData()->nDataLength;
	
	CStringW dest;
	AllocCopy(dest, nCount, GetData()->nDataLength-nCount, 0);
	return dest;
}

CStringW CStringW::Left(int nCount) const
{
	if (nCount < 0)
		nCount = 0;
	else if (nCount > GetData()->nDataLength)
		nCount = GetData()->nDataLength;
	
	CStringW dest;
	AllocCopy(dest, nCount, 0, 0);
	return dest;
}

bool CStringW::LeftIs( LPCWSTR str )
{
	int nLen = SafeStrlenW( str );
	return (0 == Left(nLen).Compare( str));
}

// strspn equivalent
CStringW CStringW::SpanIncluding(LPCWSTR lpszCharSet) const
{
	ATLASSERT(_IsValidString(lpszCharSet));
	return Left(_cstrspn(m_pchData, lpszCharSet));
}

// strcspn equivalent
CStringW CStringW::SpanExcluding(LPCWSTR lpszCharSet) const
{
	ATLASSERT(_IsValidString(lpszCharSet));
	return Left(_cstrcspn(m_pchData, lpszCharSet));
}

int CStringW::ReverseFind(wchar_t ch) const
{
	// find last single character
	LPCWSTR lpsz = _cstrrchr(m_pchData, ch);
	
	// return -1 if not found, distance from beginning otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

// find a sub-string (like strstr)
int CStringW::Find(LPCWSTR lpszSub) const
{
	return Find(lpszSub, 0);
}

int CStringW::Find(LPCWSTR lpszSub, int nStart) const
{
	ATLASSERT(_IsValidString(lpszSub));
	
	int nLength = GetData()->nDataLength;
	if (nStart > nLength)
		return -1;
	
	// find first matching substring
	LPCWSTR lpsz = _cstrstr(m_pchData + nStart, lpszSub);
	
	// return -1 for not found, distance from beginning otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

BOOL CStringW::FormatV(LPCWSTR lpszFormat, va_list argList)
{
	ATLASSERT(_IsValidString(lpszFormat));
	
	enum _FormatModifiers
	{
		FORCE_ANSI =	0x10000,
			FORCE_UNICODE =	0x20000,
			FORCE_INT64 =	0x40000
	};
	
	va_list argListSave = argList;
	
	// make a guess at the maximum length of the resulting string
	int nMaxLen = 0;
	for (LPWSTR lpsz = (LPWSTR)lpszFormat; *lpsz != '\0'; lpsz = ::CharNextW(lpsz))
	{
		// handle '%' character, but watch out for '%%'
		if (*lpsz != '%' || *(lpsz = ::CharNextW(lpsz)) == '%')
		{			
			nMaxLen++;		
			continue;
		}
		
		int nItemLen = 0;
		
		// handle '%' character with format
		int nWidth = 0;
		for (; *lpsz != '\0'; lpsz = ::CharNextW(lpsz))
		{
			// check for valid flags
			if (*lpsz == '#')
				nMaxLen += 2;   // for '0x'
			else if (*lpsz == '*')
				nWidth = va_arg(argList, int);
			else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' || *lpsz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = _cstrtoi(lpsz);
			for (; *lpsz != '\0' && _cstrisdigit(*lpsz); lpsz = ::CharNextW(lpsz))
				;
		}
		ATLASSERT(nWidth >= 0);
		
		int nPrecision = 0;
		if (*lpsz == '.')
		{
			// skip past '.' separator (width.precision)
			lpsz = ::CharNextW(lpsz);
			
			// get precision and skip it
			if (*lpsz == '*')
			{
				nPrecision = va_arg(argList, int);
				lpsz = ::CharNextW(lpsz);
			}
			else
			{
				nPrecision = _cstrtoi(lpsz);
				for (; *lpsz != '\0' && _cstrisdigit(*lpsz); lpsz = ::CharNextW(lpsz))
					;
			}
			ATLASSERT(nPrecision >= 0);
		}
		
		// should be on type modifier or specifier
		int nModifier = 0;
		if(lpsz[0] == _T('I') && lpsz[1] == _T('6') && lpsz[2] == _T('4'))
		{
			lpsz += 3;
			nModifier = FORCE_INT64;
		}
		else
		{
			switch (*lpsz)
			{
				// modifiers that affect size
			case 'h':
				nModifier = FORCE_ANSI;
				lpsz = ::CharNextW(lpsz);
				break;
			case 'l':
				nModifier = FORCE_UNICODE;
				lpsz = ::CharNextW(lpsz);
				break;
				
				// modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				lpsz = ::CharNextW(lpsz);
				break;
			}
		}
		
		// now should be on specifier
		switch (*lpsz | nModifier)
		{
			// single characters
		case 'c':
		case 'C':
			nItemLen = 2;
			va_arg(argList, wchar_t);
			break;
		case 'c' | FORCE_ANSI:
		case 'C' | FORCE_ANSI:
			nItemLen = 2;
			va_arg(argList, char);
			break;
		case 'c' | FORCE_UNICODE:
		case 'C' | FORCE_UNICODE:
			nItemLen = 2;
			va_arg(argList, WCHAR);
			break;
			
			// strings
		case 's':
			{
				LPCWSTR pstrNextArg = va_arg(argList, LPCWSTR);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6;  // "(null)"
				}
				else
				{
					nItemLen = SafeStrlenW(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
				break;
			}
			
		case 'S':
			{
				
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6; // "(null)"
				}
				else
				{
					nItemLen = SafeStrlenA(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
			
				break;
			}
			
		case 's' | FORCE_ANSI:
		case 'S' | FORCE_ANSI:
			{
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6; // "(null)"
				}
				else
				{
					nItemLen = SafeStrlenA(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
				break;
			}
			
		case 's' | FORCE_UNICODE:
		case 'S' | FORCE_UNICODE:
			{
				LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
				if (pstrNextArg == NULL)
				{
					nItemLen = 6; // "(null)"
				}
				else
				{
					nItemLen = (int)wcslen(pstrNextArg);
					nItemLen = max(1, nItemLen);
				}
				break;
			}
		}
		
		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
			nItemLen = max(nItemLen, nWidth);
			if (nPrecision != 0)
				nItemLen = min(nItemLen, nPrecision);
		}
		else
		{
			switch (*lpsz)
			{
				// integers
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				if (nModifier & FORCE_INT64)
					va_arg(argList, __int64);
				else
					va_arg(argList, int);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth + nPrecision);
				break;
				
			case 'e':
			case 'g':
			case 'G':
				va_arg(argList, double);
				nItemLen = 128;
				nItemLen = max(nItemLen, nWidth + nPrecision);
				break;
			case 'f':
				{
					double f;
					LPTSTR pszTemp;
					
					// 312 == strlen("-1+(309 zeroes).")
					// 309 zeroes == max precision of a double
					// 6 == adjustment in case precision is not specified,
					//   which means that the precision defaults to 6
					pszTemp = (LPTSTR)_alloca(max(nWidth, 312 + nPrecision + 6));
					
					f = va_arg(argList, double);
					_stprintf(pszTemp, _T( "%*.*f" ), nWidth, nPrecision + 6, f);
					nItemLen = _tcslen(pszTemp);
				}
				break;
				
			case 'p':
				va_arg(argList, void*);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth + nPrecision);
				break;
				
				// no output
			case 'n':
				va_arg(argList, int*);
				break;
				
			default:
				ATLASSERT(FALSE);  // unknown formatting option
			}
		}
		
		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}
	
	if(GetBuffer(nMaxLen) == NULL)
		return FALSE;

	int nRet = _vswprintf(m_pchData, lpszFormat, argListSave);
	nRet;	// ref
	ATLASSERT(nRet <= GetAllocLength());
	ReleaseBuffer();
	
	va_end(argListSave);
	return TRUE;
}

// formatting (using wsprintf style formatting)
BOOL __cdecl CStringW::Format(LPCWSTR lpszFormat, ...)
{
	ATLASSERT(_IsValidString(lpszFormat));
	
	va_list argList;
	va_start(argList, lpszFormat);
	BOOL bRet = FormatV(lpszFormat, argList);
	va_end(argList);
	return bRet;
}

BOOL __cdecl CStringW::Format(UINT nFormatID, ...)
{
	CStringW strFormat;
	BOOL bRet = strFormat.LoadString(nFormatID);
	ATLASSERT(bRet != 0);
	
	va_list argList;
	va_start(argList, nFormatID);
	bRet = FormatV(strFormat, argList);
	va_end(argList);
	return bRet;
}

// formatting (using FormatMessage style formatting)
BOOL __cdecl CStringW::FormatMessage(LPCWSTR lpszFormat, ...)
{
	// format message into temporary buffer lpszTemp
	va_list argList;
	va_start(argList, lpszFormat);
	LPWSTR lpszTemp;
	BOOL bRet = TRUE;
	
	if (::FormatMessageW(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		lpszFormat, 0, 0, (LPWSTR)&lpszTemp, 0, &argList) == 0 || lpszTemp == NULL)
		bRet = FALSE;
	
	// assign lpszTemp into the resulting string and free the temporary
	*this = lpszTemp;
	LocalFree(lpszTemp);
	va_end(argList);
	return bRet;
}

BOOL __cdecl CStringW::FormatMessage(UINT nFormatID, ...)
{
	// get format string from string table
	CStringW strFormat;
	BOOL bRetTmp = strFormat.LoadString(nFormatID);
	bRetTmp;	// ref
	ATLASSERT(bRetTmp != 0);
	
	// format message into temporary buffer lpszTemp
	va_list argList;
	va_start(argList, nFormatID);
	LPWSTR lpszTemp;
	BOOL bRet = TRUE;
	
	if (::FormatMessageW(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		strFormat, 0, 0, (LPWSTR)&lpszTemp, 0, &argList) == 0 || lpszTemp == NULL)
		bRet = FALSE;
	
	// assign lpszTemp into the resulting string and free lpszTemp
	*this = lpszTemp;
	LocalFree(lpszTemp);
	va_end(argList);
	return bRet;
}

void CStringW::TrimRight()
{
	CopyBeforeWrite();
	
	// find beginning of trailing spaces by starting at beginning (DBCS aware)
	LPWSTR lpsz = m_pchData;
	LPWSTR lpszLast = NULL;
	while (*lpsz != '\0')
	{
		if (_cstrisspace(*lpsz))
		{
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
		{
			lpszLast = NULL;
		}
		lpsz = ::CharNextW(lpsz);
	}
	
	if (lpszLast != NULL)
	{
		// truncate at trailing space start
		*lpszLast = '\0';
		GetData()->nDataLength = (int)(ADWORD_PTR)(lpszLast - m_pchData);
	}
}

void CStringW::TrimLeft()
{
	CopyBeforeWrite();
	
	// find first non-space character
	LPCWSTR lpsz = m_pchData;
	while (_cstrisspace(*lpsz))
		lpsz = ::CharNextW(lpsz);
	
	// fix up data and length
	int nDataLength = GetData()->nDataLength - (int)(ADWORD_PTR)(lpsz - m_pchData);
	memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(wchar_t));
	GetData()->nDataLength = nDataLength;
}

void CStringW::TrimRight(LPCWSTR lpszTargetList)
{
	// find beginning of trailing matches
	// by starting at beginning (DBCS aware)
	
	CopyBeforeWrite();
	LPWSTR lpsz = m_pchData;
	LPWSTR lpszLast = NULL;
	
	while (*lpsz != '\0')
	{
		if (_cstrchr(lpszTargetList, *lpsz) != NULL)
		{
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
			lpszLast = NULL;
		lpsz = ::CharNextW(lpsz);
	}
	
	if (lpszLast != NULL)
	{
		// truncate at left-most matching character
		*lpszLast = '\0';
		GetData()->nDataLength = (int)(ADWORD_PTR)(lpszLast - m_pchData);
	}
}

void CStringW::TrimRight(wchar_t chTarget)
{
	// find beginning of trailing matches
	// by starting at beginning (DBCS aware)
	
	CopyBeforeWrite();
	LPWSTR lpsz = m_pchData;
	LPWSTR lpszLast = NULL;
	
	while (*lpsz != '\0')
	{
		if (*lpsz == chTarget)
		{
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
			lpszLast = NULL;
		lpsz = ::CharNextW(lpsz);
	}
	
	if (lpszLast != NULL)
	{
		// truncate at left-most matching character
		*lpszLast = '\0';
		GetData()->nDataLength = (int)(ADWORD_PTR)(lpszLast - m_pchData);
	}
}

void CStringW::TrimLeft(LPCWSTR lpszTargets)
{
	// if we're not trimming anything, we're not doing any work
	if (SafeStrlenW(lpszTargets) == 0)
		return;
	
	CopyBeforeWrite();
	LPCWSTR lpsz = m_pchData;
	
	while (*lpsz != '\0')
	{
		if (_cstrchr(lpszTargets, *lpsz) == NULL)
			break;
		lpsz = ::CharNextW(lpsz);
	}
	
	if (lpsz != m_pchData)
	{
		// fix up data and length
		int nDataLength = GetData()->nDataLength - (int)(ADWORD_PTR)(lpsz - m_pchData);
		memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(wchar_t));
		GetData()->nDataLength = nDataLength;
	}
}

void CStringW::TrimLeft(wchar_t chTarget)
{
	// find first non-matching character
	
	CopyBeforeWrite();
	LPCWSTR lpsz = m_pchData;
	
	while (chTarget == *lpsz)
		lpsz = ::CharNextW(lpsz);
	
	if (lpsz != m_pchData)
	{
		// fix up data and length
		int nDataLength = GetData()->nDataLength - (int)(ADWORD_PTR)(lpsz - m_pchData);
		memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(wchar_t));
		GetData()->nDataLength = nDataLength;
	}
}

int CStringW::Delete(int nIndex, int nCount /* = 1 */)
{
	if (nIndex < 0)
		nIndex = 0;
	int nNewLength = GetData()->nDataLength;
	if (nCount > 0 && nIndex < nNewLength)
	{
		CopyBeforeWrite();
		int nBytesToCopy = nNewLength - (nIndex + nCount) + 1;
		
		memmove(m_pchData + nIndex, m_pchData + nIndex + nCount, nBytesToCopy * sizeof(wchar_t));
		GetData()->nDataLength = nNewLength - nCount;
	}
	
	return nNewLength;
}

int CStringW::Insert(int nIndex, wchar_t ch)
{
	CopyBeforeWrite();
	
	if (nIndex < 0)
		nIndex = 0;
	
	int nNewLength = GetData()->nDataLength;
	if (nIndex > nNewLength)
		nIndex = nNewLength;
	nNewLength++;
	
	if (GetData()->nAllocLength < nNewLength)
	{
		CStringDataW* pOldData = GetData();
		LPCWSTR pstr = m_pchData;
		if(!AllocBuffer(nNewLength))
			return -1;
		memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(wchar_t));
		CStringW::Release(pOldData);
	}
	
	// move existing bytes down
	memmove(m_pchData + nIndex + 1, m_pchData + nIndex, (nNewLength - nIndex) * sizeof(wchar_t));
	m_pchData[nIndex] = ch;
	GetData()->nDataLength = nNewLength;
	
	return nNewLength;
}

int CStringW::Insert(int nIndex, LPCWSTR pstr)
{
	if (nIndex < 0)
		nIndex = 0;
	
	int nInsertLength = SafeStrlenW(pstr);
	int nNewLength = GetData()->nDataLength;
	if (nInsertLength > 0)
	{
		CopyBeforeWrite();
		if (nIndex > nNewLength)
			nIndex = nNewLength;
		nNewLength += nInsertLength;
		
		if (GetData()->nAllocLength < nNewLength)
		{
			CStringDataW* pOldData = GetData();
			LPCWSTR pstr = m_pchData;
			if(!AllocBuffer(nNewLength))
				return -1;
			memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(wchar_t));
			CStringW::Release(pOldData);
		}
		
		// move existing bytes down
		memmove(m_pchData + nIndex + nInsertLength, m_pchData + nIndex, (nNewLength - nIndex - nInsertLength + 1) * sizeof(wchar_t));
		memcpy(m_pchData + nIndex, pstr, nInsertLength * sizeof(wchar_t));
		GetData()->nDataLength = nNewLength;
	}
	
	return nNewLength;
}

int CStringW::Replace(wchar_t chOld, wchar_t chNew)
{
	int nCount = 0;
	
	// short-circuit the nop case
	if (chOld != chNew)
	{
		// otherwise modify each character that matches in the string
		CopyBeforeWrite();
		LPWSTR psz = m_pchData;
		LPWSTR pszEnd = psz + GetData()->nDataLength;
		while (psz < pszEnd)
		{
			// replace instances of the specified character only
			if (*psz == chOld)
			{
				*psz = chNew;
				nCount++;
			}
			psz = ::CharNextW(psz);
		}
	}
	return nCount;
}

int CStringW::Replace(LPCWSTR lpszOld, LPCWSTR lpszNew)
{
	// can't have empty or NULL lpszOld
	
	int nSourceLen = SafeStrlenW(lpszOld);
	if (nSourceLen == 0)
		return 0;
	int nReplacementLen = SafeStrlenW(lpszNew);
	
	// loop once to figure out the size of the result string
	int nCount = 0;
	LPWSTR lpszStart = m_pchData;
	LPWSTR lpszEnd = m_pchData + GetData()->nDataLength;
	LPWSTR lpszTarget;
	while (lpszStart < lpszEnd)
	{
		while ((lpszTarget = _cstrstr(lpszStart, lpszOld)) != NULL)
		{
			nCount++;
			lpszStart = lpszTarget + nSourceLen;
		}
		lpszStart += SafeStrlenW(lpszStart) + 1;
	}
	
	// if any changes were made, make them
	if (nCount > 0)
	{
		CopyBeforeWrite();
		
		// if the buffer is too small, just
		//   allocate a new buffer (slow but sure)
		int nOldLength = GetData()->nDataLength;
		int nNewLength =  nOldLength + (nReplacementLen - nSourceLen) * nCount;
		if (GetData()->nAllocLength < nNewLength || GetData()->nRefs > 1)
		{
			CStringDataW* pOldData = GetData();
			LPCWSTR pstr = m_pchData;
			if(!AllocBuffer(nNewLength))
				return -1;
			memcpy(m_pchData, pstr, pOldData->nDataLength * sizeof(wchar_t));
			CStringW::Release(pOldData);
		}
		// else, we just do it in-place
		lpszStart = m_pchData;
		lpszEnd = m_pchData + GetData()->nDataLength;
		
		// loop again to actually do the work
		while (lpszStart < lpszEnd)
		{
			while ( (lpszTarget = _cstrstr(lpszStart, lpszOld)) != NULL)
			{
				int nBalance = nOldLength - ((int)(ADWORD_PTR)(lpszTarget - m_pchData) + nSourceLen);
				memmove((void*)(lpszTarget + nReplacementLen), lpszTarget + nSourceLen, nBalance * sizeof(wchar_t));
				memcpy((void*)lpszTarget, lpszNew, nReplacementLen * sizeof(wchar_t));
				lpszStart = lpszTarget + nReplacementLen;
				lpszStart[nBalance] = '\0';
				nOldLength += (nReplacementLen - nSourceLen);
			}
			lpszStart += SafeStrlenW(lpszStart) + 1;
		}
		ATLASSERT(m_pchData[nNewLength] == '\0');
		GetData()->nDataLength = nNewLength;
	}
	
	return nCount;
}

int CStringW::Remove(wchar_t chRemove)
{
	CopyBeforeWrite();
	
	LPWSTR pstrSource = m_pchData;
	LPWSTR pstrDest = m_pchData;
	LPWSTR pstrEnd = m_pchData + GetData()->nDataLength;
	
	while (pstrSource < pstrEnd)
	{
		if (*pstrSource != chRemove)
		{
			*pstrDest = *pstrSource;
			pstrDest = ::CharNextW(pstrDest);
		}
		pstrSource = ::CharNextW(pstrSource);
	}
	*pstrDest = '\0';
	int nCount = (int)(ADWORD_PTR)(pstrSource - pstrDest);
	GetData()->nDataLength -= nCount;
	
	return nCount;
}

// #ifdef _UNICODE
// #define CHAR_FUDGE 1    // one wchar_t unused is good enough
// #else
// #define CHAR_FUDGE 2    // two BYTES unused for case of DBC last wchar_t
// #endif

BOOL CStringW::LoadString(UINT nID)
{
	// try fixed buffer first (to avoid wasting space in the heap)
	wchar_t szTemp[256];
	int nCount =  sizeof(szTemp) / sizeof(szTemp[0]);
	int nLen = _LoadStringW(nID, szTemp, nCount);
	if (nCount - nLen > 1)
	{
		*this = szTemp;
		return (nLen > 0);
	}
	
	// try buffer size of 512, then larger size until entire string is retrieved
	int nSize = 256;
	do
	{
		nSize += 256;
		LPWSTR lpstr = GetBuffer(nSize - 1);
		if(lpstr == NULL)
		{
			nLen = 0;
			break;
		}
		nLen = _LoadStringW(nID, lpstr, nSize);
	} while (nSize - nLen <= 1);
	ReleaseBuffer();
	
	return (nLen > 0);
}

#ifndef _ATL_NO_COM
BSTR CStringW::AllocSysString() const
{
	BSTR bstr = ::SysAllocStringLen(m_pchData, GetData()->nDataLength);
	return bstr;
}

BSTR CStringW::SetSysString(BSTR* pbstr) const
{
	//::SysReAlloCStringWLen(pbstr, m_pchData, GetData()->nDataLength);
	return *pbstr;
}

void CStringW::ZeroAfter( int index )
{
	CStringDataW* data = GetData();
	if (data->data() && index < data->nAllocLength)
	{
		memset(data->data() + index, 0, data->nAllocLength - index);
	}
}

#endif //!_ATL_NO_COM


//////////////////////////////////////////////////////////////////////////
//友元 字符串连接
CStringW __stdcall operator+(const CStringW& string, wchar_t ch)
{ 
	CStringW s;
	s += ch;
	return s;
}

CStringW __stdcall operator+(wchar_t ch, const CStringW& string)
{ 
	CStringW s(ch);
	return s + string; 
}

CStringW __stdcall operator+(const CStringW& string1, const CStringW& string2)
{
	CStringW s;
	s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData, string2.GetData()->nDataLength, string2.m_pchData);
	return s;
}

CStringW __stdcall operator+(const CStringW& string, LPCWSTR lpsz)
{
	ATLASSERT(lpsz == NULL || _IsValidString(lpsz));
	CStringW s;
	s.ConcatCopy(string.GetData()->nDataLength, string.m_pchData, SafeStrlenW(lpsz), lpsz);
	return s;
}

CStringW __stdcall operator+(LPCWSTR lpsz, const CStringW& string)
{
	ATLASSERT(lpsz == NULL || _IsValidString(lpsz));
	CStringW s;
	s.ConcatCopy(SafeStrlenW(lpsz), lpsz, string.GetData()->nDataLength, string.m_pchData);
	return s;
}

//////////////////////////////////////////////////////////////////////////
//字符串比较
bool __stdcall operator==(const CStringW& s1, const CStringW& s2)
{ return s1.Compare(s2) == 0; }
bool __stdcall operator==(const CStringW& s1, LPCWSTR s2)
{ return s1.Compare(s2) == 0; }
bool __stdcall operator==(LPCWSTR s1, const CStringW& s2)
{ return s2.Compare(s1) == 0; }
bool __stdcall operator!=(const CStringW& s1, const CStringW& s2)
{ return s1.Compare(s2) != 0; }
bool __stdcall operator!=(const CStringW& s1, LPCWSTR s2)
{ return s1.Compare(s2) != 0; }
bool __stdcall operator!=(LPCWSTR s1, const CStringW& s2)
{ return s2.Compare(s1) != 0; }
bool __stdcall operator<(const CStringW& s1, const CStringW& s2)
{ return s1.Compare(s2) < 0; }
bool __stdcall operator<(const CStringW& s1, LPCWSTR s2)
{ return s1.Compare(s2) < 0; }
bool __stdcall operator<(LPCWSTR s1, const CStringW& s2)
{ return s2.Compare(s1) > 0; }
bool __stdcall operator>(const CStringW& s1, const CStringW& s2)
{ return s1.Compare(s2) > 0; }
bool __stdcall operator>(const CStringW& s1, LPCWSTR s2)
{ return s1.Compare(s2) > 0; }
bool __stdcall operator>(LPCWSTR s1, const CStringW& s2)
{ return s2.Compare(s1) < 0; }
bool __stdcall operator<=(const CStringW& s1, const CStringW& s2)
{ return s1.Compare(s2) <= 0; }
bool __stdcall operator<=(const CStringW& s1, LPCWSTR s2)
{ return s1.Compare(s2) <= 0; }
bool __stdcall operator<=(LPCWSTR s1, const CStringW& s2)
{ return s2.Compare(s1) >= 0; }
bool __stdcall operator>=(const CStringW& s1, const CStringW& s2)
{ return s1.Compare(s2) >= 0; }
bool __stdcall operator>=(const CStringW& s1, LPCWSTR s2)
{ return s1.Compare(s2) >= 0; }
bool __stdcall operator>=(LPCWSTR s1, const CStringW& s2)
{ return s2.Compare(s1) <= 0; }





//辅助函数
unsigned int CStringHelper::UnicodeCharToUTF8( wchar_t UniChar, char *OutUTFString )
{
	unsigned int UTF8CharLength = 0;
	if (UniChar < 0x80)
	{  
		if ( OutUTFString )
			OutUTFString[UTF8CharLength++] = (char)UniChar;
		else
			UTF8CharLength++;
	}
	else if(UniChar < 0x800)
	{
		if ( OutUTFString )
		{
			OutUTFString[UTF8CharLength++] = 0xc0 | ( UniChar >> 6 );
			OutUTFString[UTF8CharLength++] = 0x80 | ( UniChar & 0x3f );
		}
		else
		{
			UTF8CharLength += 2;
		}
	}
	else if(UniChar < 0x10000 )
	{
		if ( OutUTFString )
		{
			OutUTFString[UTF8CharLength++] = 0xe0 | ( UniChar >> 12 );
			OutUTFString[UTF8CharLength++] = 0x80 | ( (UniChar >> 6) & 0x3f );
			OutUTFString[UTF8CharLength++] = 0x80 | ( UniChar & 0x3f );
		}
		else
		{
			UTF8CharLength += 3;
		}
	}
	else if( UniChar < 0x200000 ) 
	{
		if ( OutUTFString )
		{
			OutUTFString[UTF8CharLength++] = 0xf0 | ( (int)UniChar >> 18 );
			OutUTFString[UTF8CharLength++] = 0x80 | ( (UniChar >> 12) & 0x3f );
			OutUTFString[UTF8CharLength++] = 0x80 | ( (UniChar >> 6) & 0x3f );
			OutUTFString[UTF8CharLength++] = 0x80 | ( UniChar & 0x3f );
		}
		else
		{
			UTF8CharLength += 4;
		}
	}

	return UTF8CharLength;
}

unsigned int CStringHelper::UnicodeStringToUTF8( const wchar_t* UnicodeString, unsigned int UnicodeStringLength, char *OutUTFString, unsigned int UTFStringBufferSize )
{
	unsigned int UTF8CharLength = 0;
	unsigned int UnicodeStringIndex = 0;
	
	while (UnicodeStringIndex < UnicodeStringLength)
	{
		wchar_t UniChar = UnicodeString[UnicodeStringIndex];

		if ( UTFStringBufferSize != 0 && UTF8CharLength >= UTFStringBufferSize )
			break;

		//这里没有做边界判断,调用之前先使用NULL作为输出参数,探测需要的长度
		if (UniChar < 0x80)
		{  
			if ( OutUTFString )
				OutUTFString[UTF8CharLength++] = (char)UniChar;
			else
				UTF8CharLength++;
		}
		else if(UniChar < 0x800)
		{
			if ( OutUTFString )
			{
				OutUTFString[UTF8CharLength++] = 0xc0 | ( UniChar >> 6 );
				OutUTFString[UTF8CharLength++] = 0x80 | ( UniChar & 0x3f );
			}
			else
			{
				UTF8CharLength += 2;
			}
		}
		else if(UniChar < 0x10000 )
		{
			if ( OutUTFString )
			{
				OutUTFString[UTF8CharLength++] = 0xe0 | ( UniChar >> 12 );
				OutUTFString[UTF8CharLength++] = 0x80 | ( (UniChar >> 6) & 0x3f );
				OutUTFString[UTF8CharLength++] = 0x80 | ( UniChar & 0x3f );
			}
			else
			{
				UTF8CharLength += 3;
			}
		}
		else if( UniChar < 0x200000 ) 
		{
			if ( OutUTFString )
			{
				OutUTFString[UTF8CharLength++] = 0xf0 | ( (int)UniChar >> 18 );
				OutUTFString[UTF8CharLength++] = 0x80 | ( (UniChar >> 12) & 0x3f );
				OutUTFString[UTF8CharLength++] = 0x80 | ( (UniChar >> 6) & 0x3f );
				OutUTFString[UTF8CharLength++] = 0x80 | ( UniChar & 0x3f );
			}
			else
			{
				UTF8CharLength += 4;
			}
		}

		UnicodeStringIndex++;
	}

	return UTF8CharLength;
}

unsigned int CStringHelper::UnicodeStringToUTF8( const CStringW& UnicodeString, CStringA& OutUTFString )
{
	if (!OutUTFString.IsEmpty())
	{
		OutUTFString = "";
	}
	unsigned int result = 0;	
	unsigned int ret = CStringHelper::UnicodeStringToUTF8((LPCWSTR)UnicodeString, UnicodeString.GetLength(), NULL, 0);
	result = CStringHelper::UnicodeStringToUTF8((LPCWSTR)UnicodeString, UnicodeString.GetLength(), OutUTFString.GetBuffer(ret), ret);
	OutUTFString.ReleaseBuffer(result);
	return result;
}

unsigned int CStringHelper::UTF8StringToUnicode( const char* UTF8String, unsigned int UTF8StringLength, wchar_t* OutUnicodeString, unsigned int UnicodeStringBufferSize )
{
	unsigned int UTF8Index = 0;
	unsigned int UniIndex = 0;

	while ( UTF8Index < UTF8StringLength )
	{
		unsigned char UTF8Char = UTF8String[UTF8Index];

		if ( UnicodeStringBufferSize != 0 && UniIndex >= UnicodeStringBufferSize )
			break;

		if ((UTF8Char & 0x80) == 0) 
		{
			const unsigned int cUTF8CharRequire = 1;

			// UTF8字码不足
			if ( UTF8Index + cUTF8CharRequire > UTF8StringLength )
				break;

			if ( OutUnicodeString )
			{
				wchar_t& WideChar = OutUnicodeString[UniIndex]; 

				WideChar = UTF8Char;
			}

			UTF8Index += cUTF8CharRequire;
		}
		else if((UTF8Char & 0xE0) == 0xC0)  ///< 110x-xxxx 10xx-xxxx
		{
			const unsigned int cUTF8CharRequire = 2;

			// UTF8字码不足
			if ( UTF8Index + cUTF8CharRequire > UTF8StringLength )
				break;

			if ( OutUnicodeString )
			{
				wchar_t& WideChar = OutUnicodeString[UniIndex]; 
				WideChar  = (UTF8String[UTF8Index + 0] & 0x3F) << 6;
				WideChar |= (UTF8String[UTF8Index + 1] & 0x3F);
			}

			UTF8Index += cUTF8CharRequire;
		}
		else if((UTF8Char & 0xF0) == 0xE0)  ///< 1110-xxxx 10xx-xxxx 10xx-xxxx
		{
			const unsigned int cUTF8CharRequire = 3;

			// UTF8字码不足
			if ( UTF8Index + cUTF8CharRequire > UTF8StringLength )
				break;

			if ( OutUnicodeString )
			{
				wchar_t& WideChar = OutUnicodeString[UniIndex]; 

				WideChar  = (UTF8String[UTF8Index + 0] & 0x1F) << 12;
				WideChar |= (UTF8String[UTF8Index + 1] & 0x3F) << 6;
				WideChar |= (UTF8String[UTF8Index + 2] & 0x3F);
			}

			UTF8Index += cUTF8CharRequire;
		} 
		else if((UTF8Char & 0xF8) == 0xF0)  ///< 1111-0xxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 
		{
			const unsigned int cUTF8CharRequire = 4;

			// UTF8字码不足
			if ( UTF8Index + cUTF8CharRequire > UTF8StringLength )
				break;

			if ( OutUnicodeString )
			{
				wchar_t& WideChar = OutUnicodeString[UniIndex]; 

				WideChar  = (UTF8String[UTF8Index + 0] & 0x0F) << 18;
				WideChar  = (UTF8String[UTF8Index + 1] & 0x3F) << 12;
				WideChar |= (UTF8String[UTF8Index + 2] & 0x3F) << 6;
				WideChar |= (UTF8String[UTF8Index + 3] & 0x3F);
			}

			UTF8Index += cUTF8CharRequire;
		} 
		else ///< 1111-10xx 10xx-xxxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 
		{
			const unsigned int cUTF8CharRequire = 5;

			// UTF8字码不足
			if ( UTF8Index + cUTF8CharRequire > UTF8StringLength )
				break;

			if ( OutUnicodeString )
			{
				wchar_t& WideChar = OutUnicodeString[UniIndex]; 

				WideChar  = (UTF8String[UTF8Index + 0] & 0x07) << 24;
				WideChar  = (UTF8String[UTF8Index + 1] & 0x3F) << 18;
				WideChar  = (UTF8String[UTF8Index + 2] & 0x3F) << 12;
				WideChar |= (UTF8String[UTF8Index + 3] & 0x3F) << 6;
				WideChar |= (UTF8String[UTF8Index + 4] & 0x3F);
			}

			UTF8Index += cUTF8CharRequire;
		}

		UniIndex++;
	}
	return UniIndex;
}

unsigned int CStringHelper::UTF8StringToUnicode( const CStringA& UTFString, CStringW& OutUnicodeString )
{
	if (!OutUnicodeString.IsEmpty())
	{
		OutUnicodeString = "";
	}
	unsigned int result = 0;	
	unsigned int ret = UTF8StringToUnicode((LPCSTR)UTFString, UTFString.GetLength(), NULL, 0);
	result = UTF8StringToUnicode((LPCSTR)UTFString, UTFString.GetLength(), OutUnicodeString.GetBuffer(ret), ret);
	OutUnicodeString.ReleaseBuffer(result);
	return result;
}



};