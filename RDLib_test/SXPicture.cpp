#include "StdAfx.h"
#include "SXPicture.h"

CSXPicture::CSXPicture(void)
{
}

CSXPicture::~CSXPicture(void)
{
}

void CSXPicture::SetShadowRP(Rect& rcDraw, int left, int top, int right, int bottom,
	Region& shadowRgn, GraphicsPath& shadowPath, bool bRoundRect /* = false */, BYTE percent /* = 15 */)
{
	// 清空路径和Region
	shadowPath.Reset();
	shadowRgn.MakeEmpty();

	// 阴影矩形范围
	Rect rcShadow(rcDraw.X - left, rcDraw.Y - top,
		rcDraw.Width + left + right, rcDraw.Height + top + bottom);

	// 确定阴影绘图路径
	shadowPath.AddRectangle(rcShadow);

	if (bRoundRect)
	{
		// 确定圆角矩形范围
		GraphicsPath roundPath;		// 圆角路径
		roundPath.Reset();
		int offsetX = rcShadow.Width * percent / 100;
		int offsetY = rcShadow.Height * percent / 100;
		int rightX = rcShadow.GetRight() - offsetX;
		int bottomY = rcShadow.GetBottom() - offsetY;
		roundPath.StartFigure();
		roundPath.AddArc(rcShadow.X, rcShadow.Y, offsetX, offsetY, 180.0f, 90.0f);	// 左上角
		roundPath.AddArc(rightX, rcShadow.Y, offsetX, offsetY, 270.0f, 90.0f);	// 右上角
		roundPath.AddArc(rightX, bottomY, offsetX, offsetY, 0.0f, 90.0f);		// 右下角
		roundPath.AddArc(rcShadow.X, bottomY, offsetX, offsetY, 90.0f, 90.0f);	// 左下角
		roundPath.CloseFigure();
		// 确定绘制阴影范围
		shadowRgn.Union(&roundPath);
	}
	else
		shadowRgn.Union(rcShadow);

	shadowRgn.Exclude(rcDraw);
}

void CSXPicture::DrawShadow(Graphics& g, GraphicsPath& shadowPath, Region& shadowRgn, DWORD dwClr)
{
	// 初始化路径渐变画笔
	PathGradientBrush brush(&shadowPath);
	brush.SetCenterColor(dwClr);			// 阴影中心颜色
	Color clrs[] = {0};
	int count = 1;
	brush.GetSurroundColors(clrs, &count);

	brush.SetFocusScales(0.85f, 0.85f);		// 对渐变效果进行调整，使其更加自然，其实际作用是对渐变效果进行缩放
											// 参数是横纵两个坐标轴的缩放比例

	g.FillRegion(&brush, &shadowRgn);		// 绘制阴影
}

void CSXPicture::RECT2Rect(RECT& srcRECT, Rect& dstRect)
{
	dstRect.X = srcRECT.left;
	dstRect.Y = srcRECT.top;
	dstRect.Width = srcRECT.right - srcRECT.left;
	dstRect.Height = srcRECT.bottom - srcRECT.top;
}

void CSXPicture::DrawShadow(Graphics& g, RECT& rcDraw, int left, int top, int right, int bottom)
{
	Rect rcItem;
	Rect rcShadow;
	RECT2Rect(rcDraw, rcItem);
	rcShadow.X = rcItem.X - left;
	rcShadow.Y = rcItem.Y - top;
	rcShadow.Width = rcItem.Width + left + right;
	rcShadow.Height = rcItem.Height + top + bottom;

	GraphicsPath pathShadow;
	pathShadow.AddRectangle(rcShadow);
	Region rgnItem(rcItem);
	Region rgnShadow(rcShadow);
	rgnShadow.Exclude(&rgnItem);

	// 初始化渐变路径渐变画刷
	PathGradientBrush brush(&pathShadow);
	brush.SetCenterColor(Color::DimGray);

	Color clrs[] = {0};
	int count = 1;
	 brush.GetSurroundColors(clrs, &count);
	brush.SetFocusScales(0.95f, 0.95f);		// 对渐变效果进行调整，使其更加自然，其实际作用是对渐变效果进行缩放
											// 参数是横纵两个坐标轴的缩放比例

	g.FillRegion(&brush, &rgnShadow);		// 绘制阴影
}

int CSXPicture::GetEncoderCLSID(LPCWSTR mimeType, CLSID& clsID)
{
	UINT numEncoders = 0;
	UINT size = 0;
	ImageCodecInfo* pImageCodeInfo = NULL;
	
	// 获取GDI+支持的图像格式编码器种类数numEncoders
	// 获取ImageCodecInfo数组的存放大小size
	GetImageEncodersSize(&numEncoders, &size);
	if (size == 0)  return CODEINFONULL;	// 编码信息不可用
	
	// 为ImageCodecInfo数组分配足额空间
	pImageCodeInfo = (ImageCodecInfo*)malloc(size);
	if (pImageCodeInfo == NULL)  return NOMEMORY;
	
	// 获取所有图像编码器的信息
	GetImageEncoders(numEncoders, size, pImageCodeInfo);	// 获取所有图像编码器信息

	// 根据MIME类型获取其在GDI+所有支持的图像编码器CLSID
	for (UINT i = 0; i < size; i++)
	{
		if (0 == lstrcmpW(mimeType , pImageCodeInfo[i].MimeType))
		{
			clsID = pImageCodeInfo->Clsid;
			free(pImageCodeInfo);
			return SUCCESS;
		}
	}

	free(pImageCodeInfo);
	return NOENCODER;
}

int CSXPicture::TransThumbnailImage(LPCWSTR lpwcstrSrc, LPCWSTR lpwcstrDst, int dstWidth, int dstHeight)
{
	// 读取图片
	Image image(lpwcstrSrc);		// 读取文件
	// 生成缩略图
	Image* thumbnail = image.GetThumbnailImage(dstWidth, dstHeight);
	// 保存
	CLSID clsID;
	int iRet = GetEncoderCLSID(L"image/png", clsID);
	if (iRet == SUCCESS)
		thumbnail->Save(lpwcstrDst, &clsID);
	return 0;
}

HBITMAP CSXPicture::CreateBitmapFromImage(LPCTSTR lpszFileName, DWORD& width, DWORD& height, void** pData)
{
	HDC hMemDC;
	HBITMAP hBitmap;

	hMemDC = CreateCompatibleDC(NULL);	// 建立与屏幕显示兼容的内存显示设备

	Image image(lpszFileName, FALSE);
	width = image.GetWidth();
	height = image.GetHeight();

	BITMAPINFO bi;
	ZeroMemory(&bi, sizeof(BITMAPINFO));
	BITMAPINFOHEADER& bmiHeader = bi.bmiHeader;
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader.biBitCount = 32;
	bmiHeader.biWidth = width;
	bmiHeader.biHeight = height;
	bmiHeader.biCompression = BI_RGB;
	bmiHeader.biPlanes = 1;	// 目标绘图设备包含的层数，必须设置为1

	// 这时还不能绘图，因为没有地方画
	// 
	hBitmap = CreateDIBSection(hMemDC, &bi, DIB_RGB_COLORS, pData, NULL, 0); // 创建应用程序可以直接写入的、与设备无关的位图
	SelectObject(hMemDC, hBitmap);
	RECT rect = {0, 0, width, height};
	FillRect(hMemDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	Graphics graphics(hMemDC);
	graphics.DrawImage(&image, 0, 0, width, height);

	DeleteDC(hMemDC);
	return hBitmap;
}
