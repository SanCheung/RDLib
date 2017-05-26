// Cryp.cpp: implementation of the CCryp class.
//
//	加密算法杂烩
//	包含MD5，Gost，Tea加密算法。
//
//
//
//
//
//
//	
//	日期：2004.11.18
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Cryp.h"
#include <stdlib.h>
#include <string.h>

#define SUCCESS		0 
#define FAIL		-1
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*Gost的s-盒*/
static U8 wz_sp[8][16] =
{
	{ 0x4,0xa,0x9,0x2,0xd,0x8,0x0,0xe,0x6,0xb,0x1,0xc,0x7,0xf,0x5,0x3},
	{ 0xe,0xb,0x4,0xc,0x6,0xd,0xf,0xa,0x2,0x3,0x8,0x1,0x0,0x7,0x5,0x9},
	{ 0x5,0x8,0x1,0xd,0xa,0x3,0x4,0x2,0xe,0xf,0xc,0x7,0x6,0x0,0x9,0xb},
	{ 0x7,0xd,0xa,0x1,0x0,0x8,0x9,0xf,0xe,0x4,0x6,0xc,0xb,0x2,0x5,0x3},
	{ 0x6,0xc,0x7,0x1,0x5,0xf,0xd,0x8,0x4,0xa,0x9,0xe,0x0,0x3,0xb,0x2},
	{ 0x4,0xb,0xa,0x0,0x7,0x2,0x1,0xd,0x3,0x6,0x8,0x5,0x9,0xc,0xf,0xe},
	{ 0xd,0xb,0x4,0x1,0x3,0xf,0x5,0x9,0x0,0xa,0xe,0x7,0x6,0x8,0x2,0xc},
	{ 0x1,0xf,0xd,0x0,0x5,0x7,0xa,0x4,0x9,0x2,0x3,0xe,0x6,0xb,0x8,0xc}
};

/*加密密钥使用顺序表*/
static U32 wz_spkey[32] = 
{
	0,1,2,3,4,5,6,7,
	0,1,2,3,4,5,6,7,
	0,1,2,3,4,5,6,7,
	7,6,5,4,3,2,1,0
};

/*s-盒替换、循环左移11位操作*/
int CCryp::f (U32 x) 
{
	x = wz_sp[7][(x>>28)&0xf]<<28 | wz_sp[6][(x>>24)&0xf]<<24
		| wz_sp[5][(x>>20)&0xf]<<20 | wz_sp[4][(x>>16)&0xf]<<16 
		| wz_sp[3][(x>>12)&0xf]<<12 | wz_sp[2][(x>>8)&0xf]<<8 
		| wz_sp[1][(x>>4)&0xf]<< 4   |  wz_sp[0][x&0xf];

	return x <<11 | x>>21;
}
/*左右值交换*/
int CCryp::gost_swap( U32 *Ldata,U32 *Rdata)
{
	U32 tempbuf;
	tempbuf = *Rdata;
	*Rdata = *Ldata ;
	*Ldata = tempbuf;
	return SUCCESS;
}

/*32轮解密操作*/
int CCryp::dencry_data( U32 *Ldata,U32 *Rdata,U32 *key)
{
	U32 i = 0 ;
	U32 tempbuf =  0 ;
	for (i = 0 ; i < 32;i++)
	{
		*Rdata ^=f(*Ldata + key[wz_spkey[31-i]]);
		gost_swap(Ldata,Rdata); /*左右值交换*/
	}
	gost_swap(Ldata,Rdata);	/*左右值交换*/
	return SUCCESS;
}

/*解密接口函数*/
int CCryp::gost_dec( U32 *data ,/*待解密数据首地址,内容需保证是64位长*/
			  U32 *key/*用户输入密钥首地址,内容需保证是256位长*/ )
{
	U32 *Ldata  ;
	U32 *Rdata ;
	Ldata = data ;
	Rdata = &Ldata[1];/*分成左右两个部分,每部分32字节*/
	dencry_data( Ldata, Rdata,key ) ;
	/*明文可用data读出*/	
	return SUCCESS;
}

int CCryp::gost_enc( U32 *data , /*待加密数据首地址,内容需保证是64位长*/
			  U32 *key /*用户输入密钥首地址,内容需保证是256位长*/ )
{
	U32 *Ldata  ;
	U32 *Rdata ;
	Ldata = data ;
	Rdata = &Ldata[1];/*分成左右两个部分,每部分32字节*/
	encry_data( Ldata, Rdata,key ) ;
	/*密文可用data读出*/	
	return SUCCESS;
}

/*32轮加密操作*/
int CCryp::encry_data( U32 *Ldata,U32 *Rdata,U32 *key)
{
	U32 i = 0 ;
	U32 tempbuf  =  0;
	for (i = 0 ; i < 32;i++)
	{
		*Rdata ^=f(*Ldata + key[wz_spkey[i]]); 
		gost_swap(Ldata,Rdata); /*左右值交换*/
	}
	gost_swap(Ldata,Rdata);	/*左右值交换*/
	return SUCCESS;
}

/* Constants for MD5Transform routine.
*/
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

/* F, G, H and I are basic MD5 functions.
*/
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.
*/
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
Rotation is separate from addition to prevent recomputation.
*/
#define FF(a, b, c, d, x, s, ac) { \
	(a) += F ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}
#define GG(a, b, c, d, x, s, ac) { \
	(a) += G ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}
#define HH(a, b, c, d, x, s, ac) { \
	(a) += H ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}
#define II(a, b, c, d, x, s, ac) { \
	(a) += I ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}


/* MD5 initialization. Begins an MD5 operation, writing a new context.
*/

CCryp::CCryp()
{
	MD5Init ();
}

CCryp::~CCryp()
{
}

void CCryp::MD5Init ()
{
	this->count[0] = this->count[1] = 0;
	/* Load magic initialization constants.*/
	this->state[0] = 0x67452301;
	this->state[1] = 0xefcdab89;
	this->state[2] = 0x98badcfe;
	this->state[3] = 0x10325476;
	/* Add by Liguangyi */
	MD5_memset(PADDING, 0, sizeof(PADDING));
	*PADDING=0x80;
	//PADDING = {
	//	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0	};
}

/* MD5 block update operation. Continues an MD5 message-digest
operation, processing another message block, and updating the
context.
*/
void CCryp::MD5Update (unsigned char *input,unsigned int inputLen)
{
	unsigned int i, index, partLen;
	
	/* Compute number of bytes mod 64 */
	index = (unsigned int)((this->count[0] >> 3) & 0x3F);
	
	/* Update number of bits */
	if ((this->count[0] += ((unsigned long int)inputLen << 3))
		< ((unsigned long int)inputLen << 3))
		this->count[1]++;
	this->count[1] += ((unsigned long int)inputLen >> 29);
	
	partLen = 64 - index;
	
	/* Transform as many times as possible.
	*/
	if (inputLen >= partLen) {
		MD5_memcpy((unsigned char*)&this->buffer[index], 
			(unsigned char*)input, partLen);
		MD5Transform (this->state, this->buffer);
		
		for (i = partLen; i + 63 < inputLen; i += 64)
			MD5Transform (this->state, &input[i]);
		
		index = 0;
	}
	else
		i = 0;
	
	/* Buffer remaining input */
	MD5_memcpy ((unsigned char*)&this->buffer[index], (unsigned char*)&input[i], inputLen-i);
}

/* MD5 finalization. Ends an MD5 message-digest operation, writing the
the message digest and zeroizing the context.
*/
void CCryp::MD5Final (unsigned char digest[16])
{
	unsigned char bits[8];
	unsigned int index, padLen;
	
	/* Save number of bits */
	Encode (bits, this->count, 8);
	
	/* Pad out to 56 mod 64.
	*/
	index = (unsigned int)((this->count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	MD5Update ( PADDING, padLen);
	
	/* Append length (before padding) */
	MD5Update (bits, 8);
	/* Store state in digest */
	Encode (digest, this->state, 16);
	
	/* Zeroize sensitive information.
	*/
	MD5_memset ((unsigned char*)this, 0, sizeof (*this));
	this->MD5Init();
}

/* MD5 basic transformation. Transforms state based on block.
*/
void CCryp::MD5Transform (unsigned long int state[4], unsigned char block[64])
{
	unsigned long int a = state[0], b = state[1], c = state[2], d = state[3], x[16];
	
	Decode (x, block, 64);
	
	/* Round 1 */
	FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
	FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
	FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
	FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
	FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
	FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
	FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
	FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
	FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
	FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
	FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */
	
	/* Round 2 */
	GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
	GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
	GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
	GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
	GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
	GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
	GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
	GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
	GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
	GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
	GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
	GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */
	
	/* Round 3 */
	HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
	HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
	HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
	HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
	HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
	HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
	HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
	HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
	HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
	HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */
	
	/* Round 4 */
	II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
	II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
	II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
	II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
	II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
	II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
	II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
	II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
	II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
	II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */
	
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	
	/* Zeroize sensitive information.
	*/
	MD5_memset ((unsigned char*)x, 0, sizeof (x));
}

/* Encodes input (unsigned long int) into output (unsigned char). Assumes len is
a multiple of 4.
*/
void CCryp::Encode (unsigned char *output, unsigned long int *input,unsigned int len)
{
	unsigned int i, j;
	
	for (i = 0, j = 0; j < len; i++, j += 4) {
		output[j] = (unsigned char)(input[i] & 0xff);
		output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
		output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
		output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
	}
}

/* Decodes input (unsigned char) into output (unsigned long int). Assumes len is
a multiple of 4.
*/
void CCryp::Decode (unsigned long int *output, unsigned char *input, unsigned int len)
{
	unsigned int i, j;
	
	for (i = 0, j = 0; j < len; i++, j += 4)
		output[i] = ((unsigned long int)input[j]) | (((unsigned long int)input[j+1]) << 8) |
		(((unsigned long int)input[j+2]) << 16) | (((unsigned long int)input[j+3]) << 24);
}

/* Note: Replace "for loop" with standard memcpy if possible.
*/

void CCryp::MD5_memcpy (unsigned char* output, unsigned char* input,unsigned int len)
{
	unsigned int i;
	
	for (i = 0; i < len; i++)
		output[i] = input[i];
}

/* Note: Replace "for loop" with standard memset if possible.
*/
void CCryp::MD5_memset (unsigned char* output,int value,unsigned int len)
{
	unsigned int i;
	
	for (i = 0; i < len; i++)
		((char *)output)[i] = (char)value;
}




/**********************************************************
   TEA - Tiny Encryption Algorithm
   Feistel cipher by David Wheeler & Roger M. Needham
   (extended version)
 **********************************************************/
#define ROUNDS		32
#define DELTA		0x9e3779b9 
/* sqr(5)-1 * 2^31 */

/**********************************************************
   Input values: 	k[4]	128-bit key
			v[2]    64-bit plaintext block
   Output values:	v[2]    64-bit ciphertext block 
 **********************************************************/
void CCryp::tean(U32 *k, U32 *v, long N) 
{
  U32 y=v[0], z=v[1];
  U32 limit,sum=0;
  if(N>0) 
  { /* ENCRYPT */
    limit=DELTA*N;
    while(sum!=limit) 
	{
      y+=((z<<4)^(z>>5)) + (z^sum) + k[sum&3];
      sum+=DELTA;
      z+=((y<<4)^(y>>5)) + (y^sum) + k[(sum>>11)&3];
    }
  } 
  else 
  { /* DECRYPT */
    sum=DELTA*(-N);
    while(sum) 
	{
      z-=((y<<4)^(y>>5)) + (y^sum) + k[(sum>>11)&3];
      sum-=DELTA;
      y-=((z<<4)^(z>>5)) + (z^sum) + k[sum&3];
    }
  }
  v[0]=y; v[1]=z;
}

void CCryp::tea_enc(U32 *k, U32 *v) 
{
	tean(k,v,ROUNDS);
}

void CCryp::tea_dec(U32 *k, U32 *v) 
{
	tean(k,v,-ROUNDS);
}

char CCryp::LMoveBit(int base, int MoveNum) 
{ 
	char result=base; 
	if(MoveNum==0)return 1; 
	if(MoveNum==1)return MoveNum; 
	result=base<<(MoveNum-1); 
	return result; 
}

void CCryp::ToBase64( const unsigned char *in, int inlen, char *out )
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
};

//===========================================================
// base64 解码
//===========================================================
void CCryp::FromBase64( const unsigned char *base64code, int base64length, char *out )
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

//			      1234567890123456789012345678901234567890			
//	char	*table = "ABCEFGHJKLMNPQRSTWXY3456789";
//	#define	LEN			27
//	0xFFFFFFFF;	
//	R65N26F7
//	0x0;		
//	A******1
#define	LEN			27

void CCryp::ToBase30( U32 in, char *out )
{
	char	*table = "ABCEFGHJKLMNPQRSTWXY3456789";

	int		a;
	int		i(0);
	while( in > LEN-1 )
	{
		a = in % LEN;
		out[i] = table[ a ];

		in /= LEN;
		i++;
	}

	out[i] = table[ in ];

	out[7] = table[i*2];
	while( ++i < 7 )
		out[i] = table[rand()%LEN];
}

U32 CCryp::FromBase30( char *out )
{
	char	*table = "ABCEFGHJKLMNPQRSTWXY3456789";
	int		an[32] = {0};

	int		index,
			count = int( strchr( table, out[7] ) - table ) / 2 +1;
	
	for( index = 0; index < count; index++ )
	{
		an[index] = int( strchr( table, out[index] ) - table );
	}

	U32		i = count-1;
	U32 	x = an[i];
	while( i > 0 )
	{
		i--;
		x = x*LEN + an[i];
	}

	return x;
}