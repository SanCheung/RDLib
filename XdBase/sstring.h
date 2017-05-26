#pragma once

typedef			unsigned int			u32;

namespace sstring
{

/////////////////////////////////////////////////////////////////////////////
class CStringA;
class CStringW;

class XDBASE_API CStringHelper
{
public:
	int BytesOfA2W(const char* cstr, int length);
	int BytesOfW2A(const wchar_t* wcstr, int length);

	u32 UnicodeCharToUTF8( wchar_t UniChar, char *OutUTFString );

	//编码转换函数
	//调用之前先使用NULL作为输出参数,探测需要的长度
	static u32  UnicodeStringToUTF8(const wchar_t* UnicodeString, u32 UnicodeStringLength, char *OutUTFString, u32 UTFStringBufferSize);
	static u32  UTF8StringToUnicode(const char* UTF8String, u32 UTF8StringLength, wchar_t* OutUnicodeString, u32 UnicodeStringBufferSize);

	static u32	UnicodeStringToUTF8( const CStringW& UnicodeString, CStringA& OutUTFString );
	static u32	UTF8StringToUnicode( const CStringA& UTFString, CStringW& OutUnicodeString );

};

class XDBASE_API CStringA : public CStringHelper
{
public:
	friend CStringW;

	struct CStringDataA
	{
		int nRefs;     
		int nDataLength;
		int nAllocLength;

		char* data()
		{ return (char*)(this + 1); }
	};

	CStringA();
	CStringA(const CStringA& stringSrc);	
	CStringA(char ch, int nRepeat = 1);
	CStringA(LPCSTR lpsz);
	CStringA(LPCWSTR lpsz);
	CStringA(LPCSTR lpch, int nLength);
	CStringA(LPCWSTR lpch, int nLength);
	CStringA(const unsigned char* psz);

	int GetLength() const;
	BOOL IsEmpty() const;
	void Empty(); 

	int		ToInt(){ return atoi( m_pchData ); }
	float	ToFloat() { return (float)atof( m_pchData ); }
	bool	LeftIs( LPCSTR str );

	char GetAt(int nIndex) const;      // 0 based
	char operator[](int nIndex) const; // same as GetAt
	void SetAt(int nIndex, char ch);
	operator LPCSTR() const;           // as a C string

	operator char*() const;

	// overloaded assignment
	const CStringA& operator=(const CStringA& stringSrc);
	const CStringA& operator=(char ch);

	const CStringA& operator=(wchar_t ch);

	const CStringA& operator=(LPCSTR lpsz);
	const CStringA& operator=(LPCWSTR lpsz);
	const CStringA& operator=(const unsigned char* psz);

	// string concatenation
	const CStringA& operator+=(const CStringA& string);
	const CStringA& operator+=(char ch);
	const CStringA& operator+=(LPCSTR lpsz);

	friend CStringA  __stdcall operator+(const CStringA& string1, const CStringA& string2);
	friend CStringA  __stdcall operator+(const CStringA& string, char ch);
	friend CStringA  __stdcall operator+(char ch, const CStringA& string);

	friend CStringA  __stdcall operator+(const CStringA& string, LPCSTR lpsz);
	friend CStringA  __stdcall operator+(LPCSTR lpsz, const CStringA& string);

	// string comparison
	int Compare(LPCSTR lpsz) const;         // straight character
	int CompareNoCase(LPCSTR lpsz) const;   // ignore case
	int Collate(LPCSTR lpsz) const;         // NLS aware
	int CollateNoCase(LPCSTR lpsz) const;   // ignore case

	// simple sub-string extraction
	CStringA Mid(int nFirst, int nCount) const;
	CStringA Mid(int nFirst) const;
	CStringA Left(int nCount) const;
	CStringA Right(int nCount) const;

	CStringA SpanIncluding(LPCSTR lpszCharSet) const;
	CStringA SpanExcluding(LPCSTR lpszCharSet) const;

	// upper/lower/reverse conversion
	void MakeUpper();
	void MakeLower();
	void MakeReverse();

	// trimming whitespace (either side)
	void TrimRight();
	void TrimLeft();

	// remove continuous occurrences of chTarget starting from right
	void TrimRight(char chTarget);
	// remove continuous occcurrences of characters in passed string,
	// starting from right
	void TrimRight(LPCSTR lpszTargets);
	// remove continuous occurrences of chTarget starting from left
	void TrimLeft(char chTarget);
	// remove continuous occcurrences of characters in
	// passed string, starting from left
	void TrimLeft(LPCSTR lpszTargets);

	// advanced manipulation
	// replace occurrences of chOld with chNew
	int Replace(char chOld, char chNew);
	// replace occurrences of substring lpszOld with lpszNew;
	// empty lpszNew removes instances of lpszOld
	int Replace(LPCSTR lpszOld, LPCSTR lpszNew);
	// remove occurrences of chRemove
	int Remove(char chRemove);
	// insert character at zero-based index; concatenates
	// if index is past end of string
	int Insert(int nIndex, char ch);
	// insert substring at zero-based index; concatenates
	// if index is past end of string
	int Insert(int nIndex, LPCSTR pstr);
	// delete nCount characters starting at zero-based index
	int Delete(int nIndex, int nCount = 1);

	int	Add( char ch );

	// searching (return starting index, or -1 if not found)
	// look for a single character match
	int Find(char ch) const;                     // like "C" strchr
	int ReverseFind(char ch) const;
	int Find(char ch, int nStart) const;         // starting at index
	int FindOneOf(LPCSTR lpszCharSet) const;

	// look for a specific sub-string
	int Find(LPCSTR lpszSub) const;        // like "C" strstr
	int Find(LPCSTR lpszSub, int nStart) const;  // starting at index

	// simple formatting
	BOOL __cdecl Format(LPCSTR lpszFormat, ...);
	BOOL __cdecl Format(UINT nFormatID, ...);
	BOOL FormatV(LPCSTR lpszFormat, va_list argList);

	// formatting for localization (uses FormatMessage API)
	BOOL __cdecl FormatMessage(LPCSTR lpszFormat, ...);
	BOOL __cdecl FormatMessage(UINT nFormatID, ...);

	// Windows support
	BOOL LoadString(UINT nID);          // load from string resource
	// 255 chars max

#ifndef _ATL_NO_COM
	// OLE BSTR support (use for OLE automation)
	BSTR AllocSysString() const;
	BSTR SetSysString(BSTR* pbstr) const;
#endif //!_ATL_NO_COM

	// Access to string implementation buffer as "C" character array
	LPSTR GetBuffer(int nMinBufLength);
	void ReleaseBuffer(int nNewLength = -1);
	LPSTR GetBufferSetLength(int nNewLength);
	void FreeExtra();

	// Use LockBuffer/UnlockBuffer to turn refcounting off
	LPSTR LockBuffer();
	void UnlockBuffer();

	// Implementation
public:
	~CStringA();
	int GetAllocLength() const;
	void ZeroAfter( int index );

	LPSTR m_pchData;   // pointer to ref counted string data

	BOOL ConcatCopy(int nSrc1Len, LPCSTR lpszSrc1Data, int nSrc2Len, LPCSTR lpszSrc2Data);
	CStringDataA* GetData() const;
protected:

	// implementation helpers
	void Init();
	void AllocCopy(CStringA& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;
	BOOL AllocBuffer(int nLen);
	void AssignCopy(int nSrcLen, LPCSTR lpszSrcData);
	void ConcatInPlace(int nSrcLen, LPCSTR lpszSrcData);
	void CopyBeforeWrite();
	BOOL AllocBeforeWrite(int nLen);
	void Release();
	static void __stdcall Release(CStringDataA* pData);
	static const CStringA& __stdcall _GetEmptyString();	
};

// Compare helpers
bool XDBASE_API __stdcall operator==(const CStringA& s1, const CStringA& s2);
bool XDBASE_API __stdcall operator==(const CStringA& s1, LPCSTR s2);
bool XDBASE_API __stdcall operator==(LPCSTR s1, const CStringA& s2);
bool XDBASE_API __stdcall operator!=(const CStringA& s1, const CStringA& s2);
bool XDBASE_API __stdcall operator!=(const CStringA& s1, LPCSTR s2);
bool XDBASE_API __stdcall operator!=(LPCSTR s1, const CStringA& s2);
bool XDBASE_API __stdcall operator<(const CStringA& s1, const CStringA& s2);
bool XDBASE_API __stdcall operator<(const CStringA& s1, LPCSTR s2);
bool XDBASE_API __stdcall operator<(LPCSTR s1, const CStringA& s2);
bool XDBASE_API __stdcall operator>(const CStringA& s1, const CStringA& s2);
bool XDBASE_API __stdcall operator>(const CStringA& s1, LPCSTR s2);
bool XDBASE_API __stdcall operator>(LPCSTR s1, const CStringA& s2);
bool XDBASE_API __stdcall operator<=(const CStringA& s1, const CStringA& s2);
bool XDBASE_API __stdcall operator<=(const CStringA& s1, LPCSTR s2);
bool XDBASE_API __stdcall operator<=(LPCSTR s1, const CStringA& s2);
bool XDBASE_API __stdcall operator>=(const CStringA& s1, const CStringA& s2);
bool XDBASE_API __stdcall operator>=(const CStringA& s1, LPCSTR s2);
bool XDBASE_API __stdcall operator>=(LPCSTR s1, const CStringA& s2);


//////////////////////////////////////////////////////////////////////////
//CStringW


class XDBASE_API CStringW : public CStringHelper
{
public:
	friend CStringA;

	struct CStringDataW
	{
		int nRefs;     
		int nDataLength;
		int nAllocLength;

		wchar_t* data()
		{ return (wchar_t*)(this + 1); }
	};
	CStringW();
	CStringW(const CStringW& stringSrc);	
	CStringW(wchar_t ch, int nRepeat = 1);
	CStringW(LPCSTR lpsz);
	CStringW(LPCWSTR lpsz);
	CStringW(LPCSTR lpch, int nLength);
	CStringW(LPCWSTR lpch, int nLength);

	int GetLength() const;
	BOOL IsEmpty() const;
	void Empty();      

	int		ToInt(){ return _wtoi( m_pchData ); }
	float	ToFloat() { return (float)_wtof( m_pchData ); }
	bool	LeftIs( LPCWSTR str );

	wchar_t GetAt(int nIndex) const;      // 0 based
	wchar_t operator[](int nIndex) const; // same as GetAt
	void SetAt(int nIndex, wchar_t ch);
	operator LPCWSTR() const;           // as a C string
	operator wchar_t*() const;

	// overloaded assignment
	const CStringW& operator=(const CStringW& stringSrc);
	const CStringW& operator=(wchar_t ch);
	const CStringW& operator=(LPCSTR lpsz);
	const CStringW& operator=(LPCWSTR lpsz);
	const CStringW& operator=(const unsigned char* psz);

	// string concatenation
	const CStringW& operator+=(const CStringW& string);
	const CStringW& operator+=(wchar_t ch);
	const CStringW& operator+=(LPCWSTR lpsz);

	friend CStringW XDBASE_API __stdcall operator+(const CStringW& string1, const CStringW& string2);
	friend CStringW XDBASE_API __stdcall operator+(const CStringW& string, wchar_t ch);
	friend CStringW XDBASE_API __stdcall operator+(wchar_t ch, const CStringW& string);

	friend CStringW XDBASE_API __stdcall operator+(const CStringW& string, LPCWSTR lpsz);
	friend CStringW XDBASE_API __stdcall operator+(LPCWSTR lpsz, const CStringW& string);

	// string comparison
	int Compare(LPCWSTR lpsz) const;         // straight character
	int CompareNoCase(LPCWSTR lpsz) const;   // ignore case
	int Collate(LPCWSTR lpsz) const;         // NLS aware
	int CollateNoCase(LPCWSTR lpsz) const;   // ignore case

	// simple sub-string extraction
	CStringW Mid(int nFirst, int nCount) const;
	CStringW Mid(int nFirst) const;
	CStringW Left(int nCount) const;
	CStringW Right(int nCount) const;

	CStringW SpanIncluding(LPCWSTR lpszCharSet) const;
	CStringW SpanExcluding(LPCWSTR lpszCharSet) const;

	// upper/lower/reverse conversion
	void MakeUpper();
	void MakeLower();
	void MakeReverse();

	// trimming whitespace (either side)
	void TrimRight();
	void TrimLeft();

	// remove continuous occurrences of chTarget starting from right
	void TrimRight(wchar_t chTarget);
	// remove continuous occcurrences of characters in passed string,
	// starting from right
	void TrimRight(LPCWSTR lpszTargets);
	// remove continuous occurrences of chTarget starting from left
	void TrimLeft(wchar_t chTarget);
	// remove continuous occcurrences of characters in
	// passed string, starting from left
	void TrimLeft(LPCWSTR lpszTargets);

	// advanced manipulation
	// replace occurrences of chOld with chNew
	int Replace(wchar_t chOld, wchar_t chNew);
	// replace occurrences of substring lpszOld with lpszNew;
	// empty lpszNew removes instances of lpszOld
	int Replace(LPCWSTR lpszOld, LPCWSTR lpszNew);
	// remove occurrences of chRemove
	int Remove(wchar_t chRemove);
	// insert character at zero-based index; concatenates
	// if index is past end of string
	int Insert(int nIndex, wchar_t ch);
	// insert substring at zero-based index; concatenates
	// if index is past end of string
	int Insert(int nIndex, LPCWSTR pstr);
	// delete nCount characters starting at zero-based index
	int Delete(int nIndex, int nCount = 1);

	// searching (return starting index, or -1 if not found)
	// look for a single character match
	int Find(wchar_t ch) const;                     // like "C" strchr
	int ReverseFind(wchar_t ch) const;
	int Find(wchar_t ch, int nStart) const;         // starting at index
	int FindOneOf(LPCWSTR lpszCharSet) const;

	// look for a specific sub-string
	int Find(LPCWSTR lpszSub) const;        // like "C" strstr
	int Find(LPCWSTR lpszSub, int nStart) const;  // starting at index

	// simple formatting
	BOOL __cdecl Format(LPCWSTR lpszFormat, ...);
	BOOL __cdecl Format(UINT nFormatID, ...);
	BOOL FormatV(LPCWSTR lpszFormat, va_list argList);

	// formatting for localization (uses FormatMessage API)
	BOOL __cdecl FormatMessage(LPCWSTR lpszFormat, ...);
	BOOL __cdecl FormatMessage(UINT nFormatID, ...);

	// Windows support
	BOOL LoadString(UINT nID);          // load from string resource
	// 255 chars max

	// ANSI <-> OEM support (convert string in place)
	void AnsiToOem();
	void OemToAnsi();


#ifndef _ATL_NO_COM
	// OLE BSTR support (use for OLE automation)
	BSTR AllocSysString() const;
	BSTR SetSysString(BSTR* pbstr) const;
#endif //!_ATL_NO_COM

	// Access to string implementation buffer as "C" character array
	LPWSTR GetBuffer(int nMinBufLength);
	void ReleaseBuffer(int nNewLength = -1);
	LPWSTR GetBufferSetLength(int nNewLength);
	void FreeExtra();

	// Use LockBuffer/UnlockBuffer to turn refcounting off
	LPWSTR LockBuffer();
	void UnlockBuffer();

	// Implementation
public:
	~CStringW();
	int GetAllocLength() const;
	void ZeroAfter( int index );

	LPWSTR m_pchData;   // pointer to ref counted string data

	BOOL ConcatCopy(int nSrc1Len, LPCWSTR lpszSrc1Data, int nSrc2Len, LPCWSTR lpszSrc2Data);
	CStringDataW* GetData() const;
protected:

	// implementation helpers
	void Init();
	void AllocCopy(CStringW& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;
	BOOL AllocBuffer(int nLen);
	void AssignCopy(int nSrcLen, LPCWSTR lpszSrcData);
	void ConcatInPlace(int nSrcLen, LPCWSTR lpszSrcData);
	void CopyBeforeWrite();
	BOOL AllocBeforeWrite(int nLen);
	void Release();
	static void __stdcall Release(CStringDataW* pData);
	static const CStringW& __stdcall _GetEmptyString();	
};

// Compare helpers
bool XDBASE_API __stdcall operator==(const CStringW& s1, const CStringW& s2);
bool XDBASE_API __stdcall operator==(const CStringW& s1, LPCWSTR s2);
bool XDBASE_API __stdcall operator==(LPCWSTR s1, const CStringW& s2);
bool XDBASE_API __stdcall operator!=(const CStringW& s1, const CStringW& s2);
bool XDBASE_API __stdcall operator!=(const CStringW& s1, LPCWSTR s2);
bool XDBASE_API __stdcall operator!=(LPCWSTR s1, const CStringW& s2);
bool XDBASE_API __stdcall operator<(const CStringW& s1, const CStringW& s2);
bool XDBASE_API __stdcall operator<(const CStringW& s1, LPCWSTR s2);
bool XDBASE_API __stdcall operator<(LPCWSTR s1, const CStringW& s2);
bool XDBASE_API __stdcall operator>(const CStringW& s1, const CStringW& s2);
bool XDBASE_API __stdcall operator>(const CStringW& s1, LPCWSTR s2);
bool XDBASE_API __stdcall operator>(LPCWSTR s1, const CStringW& s2);
bool XDBASE_API __stdcall operator<=(const CStringW& s1, const CStringW& s2);
bool XDBASE_API __stdcall operator<=(const CStringW& s1, LPCWSTR s2);
bool XDBASE_API __stdcall operator<=(LPCWSTR s1, const CStringW& s2);
bool XDBASE_API __stdcall operator>=(const CStringW& s1, const CStringW& s2);
bool XDBASE_API __stdcall operator>=(const CStringW& s1, LPCWSTR s2);
bool XDBASE_API __stdcall operator>=(LPCWSTR s1, const CStringW& s2);

// trans
u32 XDBASE_API UnicodeStringToUTF8(const CStringW& UnicodeString, CStringA& OutUTFString);
u32 XDBASE_API UTF8StringToUnicode(const CStringA& UTFString, CStringW& OutUnicodeString);


};
