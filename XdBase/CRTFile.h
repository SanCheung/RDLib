#pragma once

class XDBASE_API CCRTFile
{
public:
	CCRTFile( TCHAR *filename, TCHAR *mode );
	~CCRTFile(void);

	//size_t __cdecl fread(__out_bcount(_ElementSize*_Count) void * _DstBuf, __in size_t _ElementSize, __in size_t _Count, __inout FILE * _File)
	//size_t __cdecl fwrite(__in_ecount(_Size*_Count) const void * _Str, __in size_t _Size, __in size_t _Count, __inout FILE * _File);
	size_t Read( void *buf, size_t uElementSize, size_t uCount );
	size_t Write( void *buf, size_t uElementSize, size_t uCount );

	operator FILE*() const
	{
		return m_pFile;
	}

	long GetSize();

protected:
	FILE		*m_pFile;
};
