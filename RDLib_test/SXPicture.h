#ifndef __SX_PICTURE_H__
#define __SX_PICTURE_H__

#define NOENCODER		-3		// GDI+ͼ���������֧��
#define NOMEMORY		-2		// �ڴ����ʧ��
#define CODEINFONULL	-1		// ������Ϣ��СΪ0
#define SUCCESS			0		// �����ɹ�

class CSXPicture
{
public:
	CSXPicture(void);
	~CSXPicture(void);

	/*
	* FUNCTION:			��ȡԲ��·��
	* INITIALIZATION:	...
	* PARAMETER(S):		...
	* RETURN:			...
	*/

	/*
	* FUNCTION:			������Ӱ����Χ�����ͼ·��
	* INITIALIZATION:	...
	* PARAMETER(S):		...
	* RETURN:			...
	*/
	static void SetShadowRP(Rect& rcDraw, int left, int top, int right, int bottom,
		Region& shadowRgn, GraphicsPath& shadowPath, bool bRoundRect = false, BYTE percent  = 15);

	/*
	* FUNCTION:			������Ӱ����Χ�����ͼ·��������Ӱ
	* INITIALIZATION:	...
	* PARAMETER(S):		...
	* RETURN:			...
	*/
	static void DrawShadow(Graphics& g, GraphicsPath& shadowPath, Region& shadowRgn, DWORD dwClr);

	/*
	* FUNCTION:			������Ӱ
	* INITIALIZATION:	...
	* PARAMETER(S):		...
	* RETURN:			...
	*/
	static void DrawShadow(Graphics& g, RECT& rcDraw, int left, int top, int right, int bottom);

	/*
	* FUNCTION:			����mineType��ȡ����GDI+����֧�ֵ�ͼ���������CLSID
	* INITIALIZATION:	...
	* PARAMETER(S):		...
	* RETURN:
	*	NOENCODER		GDI+ͼ���������֧��
	*	NOMEMORY		�ڴ����ʧ��
	*	CODEINFONULL	������Ϣ��СΪ0
	*	SUCCESS			��ȡͼ�������CLSID
	*/
	static int GetEncoderCLSID(LPCWSTR mimeType, CLSID& clsID);

	/*
	* FUNCTION:			����Դͼ��ȡ����ͼ�������ļ�
	* INITIALIZATION:	Ĭ�ϴ����ַ����ǿ�
	* PARAMETER(S):
	*	@lpszSrc		ԴͼĿ¼
	*	@lpszDst		����ͼ����Ŀ¼
	*	@dstWidth		Ŀ����
	*	@dstHeight		Ŀ��߶�
	* RETURN:			...
	*/
	static int TransThumbnailImage(LPCWSTR lpwcstrSrc, LPCWSTR lpwcstrDst, int dstWidth, int dstHeight);

	static HBITMAP CreateBitmapFromImage(LPCTSTR lpszFileName, DWORD& width, DWORD& height, void** pData);

	static void		RECT2Rect(RECT& srcRECT, Rect& dstRect);
};

/*
* FUNCTION:			...
* INITIALIZATION:	...
* PARAMETER(S):		...
* RETURN:			...
*/

#endif // __SX_PICTURE_H__