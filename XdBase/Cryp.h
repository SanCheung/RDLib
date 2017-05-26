// Cryp.h: interface for the CCryp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRYP_H__2EF93F1B_8A64_40C8_B12D_6BD8BE2C4117__INCLUDED_)
#define AFX_CRYP_H__2EF93F1B_8A64_40C8_B12D_6BD8BE2C4117__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef unsigned int	U32;
typedef unsigned char	U8;


class CCryp  
{
public:
	CCryp();
	virtual ~CCryp();

	void MD5Update ( unsigned char *input, unsigned int inputLen);
	void MD5Final (unsigned char digest[16]);

	static void tea_enc(U32 *k, U32 *v);
	static void tea_dec(U32 *k, U32 *v);

	static char LMoveBit(int base, int MoveNum);

	/*加密接口函数*/
	static int gost_enc(U32 *data , U32 *key);

	/*解密接口函数*/
	static int gost_dec(U32 *data, U32 *key);

	static	void	ToBase64( const unsigned char *in, int len, char *out );
	static	void	FromBase64( const unsigned char *in, int len, char *out );

	static	void	ToBase30( U32 in, char *out );
	static  U32		FromBase30( char *out );


private:
	static void tean(U32 *k, U32 *v, long N);

	static int encry_data( U32 *Ldata,U32 *Rdata,U32 *key);
	static int dencry_data( U32 *Ldata,U32 *Rdata,U32 *key);

	/*s-盒替换、循环左移11位操作*/
	static int f(U32 x);

	/*左右值交换*/
	static int gost_swap( U32 *Ldata,U32 *Rdata);


private:
	unsigned long int	state[4];				/* state (ABCD) */
	unsigned long int	count[2];				/* number of bits, modulo 2^64 (lsb first) */
	unsigned char		buffer[64];				/* input buffer */
	unsigned char		PADDING[64];			/* What? */

private:
	void MD5Init ();
	void MD5Transform (unsigned long int state[4], unsigned char block[64]);
	void MD5_memcpy (unsigned char* output, unsigned char* input,unsigned int len);
	void Encode (unsigned char *output, unsigned long int *input,unsigned int len);
	void Decode (unsigned long int *output, unsigned char *input, unsigned int len);
	void MD5_memset (unsigned char* output,int value,unsigned int len);

};

#endif // !defined(AFX_CRYP_H__2EF93F1B_8A64_40C8_B12D_6BD8BE2C4117__INCLUDED_)
