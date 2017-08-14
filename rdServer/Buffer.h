#pragma once


class CBuffer
{
public:
	CBuffer(void);
	virtual ~CBuffer(void);

	NOCOPY_CLASS( CBuffer )

public:
	operator const char* ();
	const char* GetData();

	int		GetSize()			{ return m_nSize; }
	void	SetSize(int size)	{ m_nSize = size; }

	int		GetTotalSize()		{ return m_nTotalSize; }
	int		GetFreeSize();

	void	SetGrowSize( int nGrowSize );

	int		Append( char *pData, int nSize );
	int		Append( char data );

	int		AssignData( char *pData, int nSize );
	int		MoveData( int nTo, int nFrom, int nSize );
	void	EraseData();
	void	FreeData();
	int		Find( char *pchSearch, int nSize, int nFrom = 0 );

protected:
	char	*m_pchBuffer;
	int		m_nSize;		// buffer中有效数据长度
	int		m_nTotalSize;	// buffer总的长度
	int		m_nGrowSize;	// 每次增加的长度
};

