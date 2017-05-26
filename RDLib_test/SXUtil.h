#ifndef __SX_UTIL_H__
#define __SX_UTIL_H__

class CSXUtil
{
public:
	/*
	* FUNCTION:			产生一个[min, max]之间的无重复随机序列
	* INITIALIZATION:	无
	* PARAMETER(S):
	*	@iMin			序列最小值	
	*	@max			序列最大值
	*	@pBuffer		数组数缓冲地址
	*	@size			最大缓冲大小
	* RETURN:			产生一个[min, max]之间的无重复随机序列
	*/
	static int RandSeqence(int min, int max, int* sequence, int maxsize);
};

/*
* FUNCTION:			...
* INITIALIZATION:	...
* PARAMETER(S):		...
* RETURN:			...
*/
#endif // __SX_UTIL_H__
