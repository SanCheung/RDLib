#include "StdAfx.h"
#include "SXUtil.h"

#pragma warning(disable : 4244)

int CSXUtil::RandSeqence(int min, int max, int* sequence, int maxsize)
{
	int i = 0;
	int arrCount = max - min + 1;
	int *array = new int[arrCount];
	// 初始化数组
	for (i = 0; i < arrCount; i++)
		array[i] = min + i;

	srand(time(NULL));
	int idxBuf = 0;	// 指示缓冲区的索引值
	while (arrCount > 0)
	{
		ASSERT(idxBuf < maxsize);			// DEBUG状态下提示需要重新设置缓冲区大小，主要用于调试
		unsigned iCur = rand() % arrCount;	// 数组索引指示
		sequence[idxBuf++] = array[iCur];
		for (int j = iCur + 1; j < arrCount; j++)
		{// 原来数组iCur之后的元素前移
			array[j-1] = array[j];
		}
		arrCount--;
	}
	
	delete array;		// 释放寄存数组内存空间

	return idxBuf;		// 此时idxBuf等于max - min + 1
}

#pragma warning(default: 4244)