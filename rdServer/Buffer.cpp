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

// ��Ҫ >0
void CBuffer::SetGrowSize( int nGrowSize )
{
	if( nGrowSize > 0 )
		m_nGrowSize = nGrowSize;
}

//===========================================
// ׷������,���ص�ǰBuffer����Ч���ݵĳ���
//===========================================
int CBuffer::Append( char *pData, int nSize )
{
	if( GetFreeSize() < nSize )
	{
		// ���ӳ���
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
// �������ݣ��Ḳ��ԭ�е�����
//=====================================
int CBuffer::AssignData( char *pData, int nSize )
{
	if( GetTotalSize() < nSize )
	{
		// ���ӳ���
		delete []m_pchBuffer;
		m_nTotalSize += (nSize / m_nGrowSize + 1) * m_nGrowSize;
		m_pchBuffer = new char[m_nTotalSize];
	}

	memcpy( m_pchBuffer, pData, nSize );
	m_nSize = nSize;

	return m_nSize;
}

/*
// ����ָ�����ȵ�����
// ���Ȳ�������0,���ؿ����ĳ���
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
// ֻ֧�� �Ӻ���ǰ�ƶ�
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
// ������Ч����
// ���ǲ���ɾ��ʵ�ʵ�����
//===========================
void CBuffer::EraseData()
{
	m_nSize = 0;
}

// �ͷ�ʵ�ʵ�����
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
// nStart ��0 ��ʼ
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
