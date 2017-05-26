#ifndef __SX_PICTURE_H__
#define __SX_PICTURE_H__

#define NOENCODER		-3		// GDI+图像编码器不支持
#define NOMEMORY		-2		// 内存分配失败
#define CODEINFONULL	-1		// 编码信息大小为0
#define SUCCESS			0		// 操作成功

class CSXPicture
{
public:
	CSXPicture(void);
	~CSXPicture(void);

	/*
	* FUNCTION:			获取圆角路径
	* INITIALIZATION:	...
	* PARAMETER(S):		...
	* RETURN:			...
	*/

	/*
	* FUNCTION:			设置阴影区域范围及其绘图路径
	* INITIALIZATION:	...
	* PARAMETER(S):		...
	* RETURN:			...
	*/
	static void SetShadowRP(Rect& rcDraw, int left, int top, int right, int bottom,
		Region& shadowRgn, GraphicsPath& shadowPath, bool bRoundRect = false, BYTE percent  = 15);

	/*
	* FUNCTION:			根据阴影区域范围及其绘图路径绘制阴影
	* INITIALIZATION:	...
	* PARAMETER(S):		...
	* RETURN:			...
	*/
	static void DrawShadow(Graphics& g, GraphicsPath& shadowPath, Region& shadowRgn, DWORD dwClr);

	/*
	* FUNCTION:			绘制阴影
	* INITIALIZATION:	...
	* PARAMETER(S):		...
	* RETURN:			...
	*/
	static void DrawShadow(Graphics& g, RECT& rcDraw, int left, int top, int right, int bottom);

	/*
	* FUNCTION:			根据mineType获取其在GDI+所有支持的图像编码器的CLSID
	* INITIALIZATION:	...
	* PARAMETER(S):		...
	* RETURN:
	*	NOENCODER		GDI+图像编码器不支持
	*	NOMEMORY		内存分配失败
	*	CODEINFONULL	编码信息大小为0
	*	SUCCESS			获取图像编码器CLSID
	*/
	static int GetEncoderCLSID(LPCWSTR mimeType, CLSID& clsID);

	/*
	* FUNCTION:			根据源图获取缩略图并保存文件
	* INITIALIZATION:	默认传入字符串非空
	* PARAMETER(S):
	*	@lpszSrc		源图目录
	*	@lpszDst		缩略图保存目录
	*	@dstWidth		目标宽度
	*	@dstHeight		目标高度
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