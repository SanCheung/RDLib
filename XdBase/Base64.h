#pragma once

typedef		unsigned char			U8;
typedef     unsigned int			U32;

class XDBASE_API CBase64
{
public:
	CBase64(void);
	~CBase64(void);

	static char LMoveBit(int base, int MoveNum);
	static void FromBase64( const U8 *base64code, int base64length, char *out );
	static void ToBase64( const U8 *in, int inlen, char *out );

	static CStringW FromBase64( CStringW strIn );
	static CStringW ToBase64( CStringW strIn );

	static CStringA FromBase64( CStringA strIn );
};

