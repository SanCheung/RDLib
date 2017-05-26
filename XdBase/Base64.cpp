#include "StdAfx.h"
#include "Base64.h"


CBase64::CBase64(void)
{
}


CBase64::~CBase64(void)
{
}

char CBase64::LMoveBit(int base, int MoveNum) 
{ 
	char result=base; 
	if(MoveNum==0)return 1; 
	if(MoveNum==1)return MoveNum; 
	result=base<<(MoveNum-1); 
	return result; 
}

void CBase64::FromBase64( const U8 *base64code, int base64length, char *out )
{
	char base64_alphabet[]= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

	char buf[4]; 
	int i,j; 
	int k; 
	int l=0; 
	U8 temp1[4],temp2; 
	U32 base64a=(base64length/4)-1; 
	U32 base64b=0; 
	int m_padnum = 0;
	for(;base64b<base64a+1;base64b++) 
	{ 
		for(i=0;i<4;i++) 
		{ 
			buf[i]=*(base64code+(base64b*4)+i); 
			for(j=0;j<65;j++) 
			{ 
				if(buf[i]==base64_alphabet[j]) 
				{ 
					temp1[i]=j; 
					break; 
				} 
			} 
		} 
		i--; 
		for(k=1;k<4;k++) 
		{ 
			if(temp1[i-(k-1)]==64){
				m_padnum++; 
				continue;} 
			temp1[i-(k-1)]=temp1[i-(k-1)]/LMoveBit(2,(k-1)*2); 
			temp2=temp1[i-k]; 
			temp2=temp2&(LMoveBit(2,k*2)-1); 
			temp2*=LMoveBit(2,8-(2*k));//move 4 
			temp1[i-(k-1)]=temp1[i-(k-1)]+temp2; 
			out[base64b*3+(3-k)]=temp1[i-(k-1)]; 
		} 
	}
}

void CBase64::ToBase64( const U8 *in, int inlen, char *out )
{
	const char base64digits[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	for (; inlen >= 3; inlen -= 3)
	{
		*out++ = base64digits[in[0] >> 2];
		*out++ = base64digits[((in[0] << 4) & 0x30) | (in[1] >> 4)];
		*out++ = base64digits[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
		*out++ = base64digits[in[2] & 0x3f];
		in += 3;
	}

	if (inlen > 0)
	{
		U8 fragment;

		*out++ = base64digits[in[0] >> 2];
		fragment = (in[0] << 4) & 0x30;
		if (inlen > 1)
			fragment |= in[1] >> 4;
		*out++ = base64digits[fragment];
		*out++ = (inlen < 2) ? '=' : base64digits[(in[1] << 2) & 0x3c];
		*out++ = '=';
	}
	*out = '\0';
}

CStringW CBase64::ToBase64( CStringW strIn )
{
	CStringA   str = strIn;
	int			len = str.GetLength();

	char		*buf = new char[len*4];
	memset( buf, 0, len*4 );
	CBase64::ToBase64( (const U8*)str.m_pchData, len, buf );

	CStringA   strOut = buf;
	return (CStringW)strOut;
}



CStringW CBase64::FromBase64( CStringW strIn )
{
	CStringA   str = strIn;
	int			len = str.GetLength();

	char		*buf = new char[len+2];
	memset( buf, 0, len+2 );
	CBase64::FromBase64( (const U8*)str.m_pchData, len, buf );

	CStringA   strOut = buf;
	return (CStringW)strOut;
}

CStringA CBase64::FromBase64( CStringA strIn )
{
	int			len = strIn.GetLength();

	char		*buf = new char[len+2];
	memset( buf, 0, len+2 );
	CBase64::FromBase64( (const U8*)strIn.m_pchData, len, buf );

	CStringA   strOut = buf;
	return strOut;
}
