#include "StdAfx.h"
#include "Buffer.h"

CBuffer::CBuffer(void)
{
	m_pchBuffer		= NULL;
	m_nSize			= 0;
	m_nTotalSize	= 0;
	m_nGrowSize		= 512;
}


CBuffer::~CBuffer(void)
{
	FreeData();
}

const char* CBuffer::GetData()
{
	return m_pchBuffer;
}

int CBuffer::GetFreeSize()
{
	return m_nTotalSize - m_nSize;
}

// 需要 >0
void CBuffer::SetGrowSize( int nGrowSize )
{
	if( nGrowSize > 0 )
		m_nGrowSize = nGrowSize;
}

//===========================================
// 追加数据,返回当前Buffer中有效数据的长度
//===========================================
int CBuffer::Append( char *pData, int nSize )
{
	if( GetFreeSize() < nSize )
	{
		// 增加长度
		char *pchTemp = m_pchBuffer;
		m_nTotalSize += (nSize / m_nGrowSize + 1) * m_nGrowSize;
		m_pchBuffer = new char[m_nTotalSize];
		if( m_nSize > 0 )
			memcpy( m_pchBuffer, pchTemp, m_nSize );
		delete []pchTemp;
	}

	memcpy( m_pchBuffer + m_nSize, pData, nSize );
	m_nSize += nSize;

	return m_nSize;
}

int CBuffer::Append( char data )
{
	return Append( &data, 1 );
}

//=====================================
// 设置数据，会覆盖原有的数据
//=====================================
int CBuffer::AssignData( char *pData, int nSize )
{
	if( GetTotalSize() < nSize )
	{
		// 增加长度
		delete []m_pchBuffer;
		m_nTotalSize += (nSize / m_nGrowSize + 1) * m_nGrowSize;
		m_pchBuffer = new char[m_nTotalSize];
	}

	memcpy( m_pchBuffer, pData, nSize );
	m_nSize = nSize;

	return m_nSize;
}

/*
// 拷贝指定长度的数据
// 长度不够返回0,返回拷贝的长度
int CAntBuffer::CopyData( char *pData, int nSize )
{
	if( GetTotalSize() < nSize )
		return 0;

	memcpy( m_pchBuffer, pData, nSize );
	m_nSize = nSize;
	return m_nSize;
}
*/

//======================================
// 只支持 从后往前移动
//======================================
int CBuffer::MoveData( int nTo, int nFrom, int nSize )
{
	m_nSize = nSize;
	if( nFrom <= nTo || nSize == 0)
		return 0;

	memcpy( m_pchBuffer + nTo, m_pchBuffer + nFrom, nSize );
	return m_nSize;
}
//===========================
// 擦除有效数据
// 但是并不删除实际的内容
//===========================
void CBuffer::EraseData()
{
	m_nSize = 0;
}

// 释放实际的数据
void CBuffer::FreeData()
{
	if( m_pchBuffer == NULL )
		return;

	m_nSize = 0;
	m_nTotalSize = 0;
	delete []m_pchBuffer;
	m_pchBuffer = NULL;
}

//======================================
// nStart 从0 开始
//======================================
int CBuffer::Find( char *pchSearch, int nSize, int nFrom /*= 0*/ )
{
	if( m_nSize <= 0 )
		return -1;

	if( nFrom < 0 || (nFrom + nSize) > m_nSize || pchSearch == NULL )
		return -1;
	
	int nIndex = nFrom;

	for( nIndex; nIndex <= (m_nSize - nSize + 1); nIndex ++ )
	{
		int j = 0;
		for( j = 0; j < nSize; j ++ )
		{
			if( m_pchBuffer[nIndex+j] != pchSearch[j] )
				break;
		}

		if( j == nSize )
			return nIndex;
	}

	return -1;
}

CBuffer::operator const char*()
{
	return m_pchBuffer;
}
