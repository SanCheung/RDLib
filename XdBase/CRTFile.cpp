#include "StdAfx.h"
#include "CRTFile.h"

CCRTFile::CCRTFile( TCHAR *filename, TCHAR *mode )
: m_pFile( NULL )
{
	_tfopen_s( &m_pFile, filename, mode );
}

CCRTFile::~CCRTFile(void)
{
	if( NULL != m_pFile )
		fclose( m_pFile );
}

size_t CCRTFile::Read( void *buf, size_t uElementSize, size_t uCount )
{
	if( NULL == m_pFile )
		return 0;

	return fread( buf, uElementSize, uCount, m_pFile );
}

size_t CCRTFile::Write( void *buf, size_t uElementSize, size_t uCount )
{
	if( NULL == m_pFile )
		return 0;

	return fwrite( buf, uElementSize, uCount, m_pFile );
}


long CCRTFile::GetSize()
{
	if( NULL == m_pFile )
		return -1;
	
	fseek( m_pFile, 0, SEEK_END );
	
	long	length = ftell( m_pFile );
	rewind( m_pFile );
	return length;


}