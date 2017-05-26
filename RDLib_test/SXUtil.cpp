#include "StdAfx.h"
#include "SXUtil.h"

#pragma warning(disable : 4244)

int CSXUtil::RandSeqence(int min, int max, int* sequence, int maxsize)
{
	int i = 0;
	int arrCount = max - min + 1;
	int *array = new int[arrCount];
	// ��ʼ������
	for (i = 0; i < arrCount; i++)
		array[i] = min + i;

	srand(time(NULL));
	int idxBuf = 0;	// ָʾ������������ֵ
	while (arrCount > 0)
	{
		ASSERT(idxBuf < maxsize);			// DEBUG״̬����ʾ��Ҫ�������û�������С����Ҫ���ڵ���
		unsigned iCur = rand() % arrCount;	// ��������ָʾ
		sequence[idxBuf++] = array[iCur];
		for (int j = iCur + 1; j < arrCount; j++)
		{// ԭ������iCur֮���Ԫ��ǰ��
			array[j-1] = array[j];
		}
		arrCount--;
	}
	
	delete array;		// �ͷżĴ������ڴ�ռ�

	return idxBuf;		// ��ʱidxBuf����max - min + 1
}

#pragma warning(default: 4244)