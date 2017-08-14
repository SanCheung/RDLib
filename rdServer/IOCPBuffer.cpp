#include "StdAfx.h"
#include "IOCPBuffer.h"

namespace winapi
{

CIOCPBuffer::CIOCPBuffer(void)
{
	memset(&m_OverLapped, 0, sizeof(WSAOVERLAPPED) );
	m_pchBuf		= NULL;
	m_nOperation	= IOCP_OPERATION_NONE;
	m_nBufSize		= IOCP_BUF_SIZE;
	m_nBufUsed		= 0;
	m_nAddrSize		= 0;
}


CIOCPBuffer::~CIOCPBuffer(void)
{
	Clear();
}

//=========================
// 初始化数据
//=========================
void CIOCPBuffer::Init()
{
	if( m_pchBuf == NULL )
		m_pchBuf = new char[m_nBufSize];

	ATLASSERT(m_pchBuf != NULL );
	memset(&m_RemoteAddr, 0, sizeof(m_RemoteAddr) );

	/*
	if( m_wsaBuf.buf == NULL )
	{
		m_wsaBuf.buf = new char[m_nBufSize];
		m_wsaBuf.len = m_nBufSize;
	}*/
}

//=============================
// 清空数据,Delete 申请的内存
//=============================
void CIOCPBuffer::Clear()
{
	if( m_pchBuf != NULL )
		delete []m_pchBuf;
	m_pchBuf = NULL;
}

WSABUF* CIOCPBuffer::GetWSABuf()
{
	return &m_wsaBuf;
}

//
const char* CIOCPBuffer::GetData()
{
	return m_pchBuf;
}

BOOL CIOCPBuffer::SetData( const char *pData, UINT nSize )
{
	if( nSize > m_nBufSize )
		return FALSE;

	memcpy( m_pchBuf, pData, nSize );
	m_nBufUsed = nSize;
	return TRUE;
}

short CIOCPBuffer::GetOperation()
{
	return m_nOperation;
}

void CIOCPBuffer::SetOperation(short nOperation)
{
	m_nOperation = nOperation;

	if( m_nOperation == IOCP_OPERATION_RECV )
	{
		m_wsaBuf.buf = m_pchBuf + m_nBufUsed;
		m_wsaBuf.len = m_nBufSize - m_nBufUsed;
	}
	else if( m_nOperation == IOCP_OPERATION_SEND )
	{
		m_wsaBuf.buf = m_pchBuf;
		m_wsaBuf.len = m_nBufUsed;
	}
	else if( m_nOperation == IOCP_OPERATION_RECVFROM )
	{
		m_wsaBuf.buf = m_pchBuf + m_nBufUsed;
		m_wsaBuf.len = m_nBufSize - m_nBufUsed;
	}
	else if( m_nOperation == IOCP_OPERATION_SENDTO )
	{
		m_wsaBuf.buf = m_pchBuf;
		m_wsaBuf.len = m_nBufUsed;
	}
	else
	{
		m_wsaBuf.buf = m_pchBuf;
		m_wsaBuf.len = m_nBufSize;
	}
}

void CIOCPBuffer::AddUsed(UINT dwBytes)
{
	m_nBufUsed += dwBytes;
}

void CIOCPBuffer::SetUsed(UINT dwBytes)
{
	m_nBufUsed = dwBytes;
}

UINT CIOCPBuffer::GetUsedLen()
{
	return m_nBufUsed;
}

}
