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
	// ���·����Region
	shadowPath.Reset();
	shadowRgn.MakeEmpty();

	// ��Ӱ���η�Χ
	Rect rcShadow(rcDraw.X - left, rcDraw.Y - top,
		rcDraw.Width + left + right, rcDraw.Height + top + bottom);

	// ȷ����Ӱ��ͼ·��
	shadowPath.AddRectangle(rcShadow);

	if (bRoundRect)
	{
		// ȷ��Բ�Ǿ��η�Χ
		GraphicsPath roundPath;		// Բ��·��
		roundPath.Reset();
		int offsetX = rcShadow.Width * percent / 100;
		int offsetY = rcShadow.Height * percent / 100;
		int rightX = rcShadow.GetRight() - offsetX;
		int bottomY = rcShadow.GetBottom() - offsetY;
		roundPath.StartFigure();
		roundPath.AddArc(rcShadow.X, rcShadow.Y, offsetX, offsetY, 180.0f, 90.0f);	// ���Ͻ�
		roundPath.AddArc(rightX, rcShadow.Y, offsetX, offsetY, 270.0f, 90.0f);	// ���Ͻ�
		roundPath.AddArc(rightX, bottomY, offsetX, offsetY, 0.0f, 90.0f);		// ���½�
		roundPath.AddArc(rcShadow.X, bottomY, offsetX, offsetY, 90.0f, 90.0f);	// ���½�
		roundPath.CloseFigure();
		// ȷ��������Ӱ��Χ
		shadowRgn.Union(&roundPath);
	}
	else
		shadowRgn.Union(rcShadow);

	shadowRgn.Exclude(rcDraw);
}

void CSXPicture::DrawShadow(Graphics& g, GraphicsPath& shadowPath, Region& shadowRgn, DWORD dwClr)
{
	// ��ʼ��·�����仭��
	PathGradientBrush brush(&shadowPath);
	brush.SetCenterColor(dwClr);			// ��Ӱ������ɫ
	Color clrs[] = {0};
	int count = 1;
	brush.GetSurroundColors(clrs, &count);

	brush.SetFocusScales(0.85f, 0.85f);		// �Խ���Ч�����е�����ʹ�������Ȼ����ʵ�������ǶԽ���Ч����������
											// �����Ǻ�����������������ű���

	g.FillRegion(&brush, &shadowRgn);		// ������Ӱ
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

	// ��ʼ������·�����仭ˢ
	PathGradientBrush brush(&pathShadow);
	brush.SetCenterColor(Color::DimGray);

	Color clrs[] = {0};
	int count = 1;
	 brush.GetSurroundColors(clrs, &count);
	brush.SetFocusScales(0.95f, 0.95f);		// �Խ���Ч�����е�����ʹ�������Ȼ����ʵ�������ǶԽ���Ч����������
											// �����Ǻ�����������������ű���

	g.FillRegion(&brush, &rgnShadow);		// ������Ӱ
}

int CSXPicture::GetEncoderCLSID(LPCWSTR mimeType, CLSID& clsID)
{
	UINT numEncoders = 0;
	UINT size = 0;
	ImageCodecInfo* pImageCodeInfo = NULL;
	
	// ��ȡGDI+֧�ֵ�ͼ���ʽ������������numEncoders
	// ��ȡImageCodecInfo����Ĵ�Ŵ�Сsize
	GetImageEncodersSize(&numEncoders, &size);
	if (size == 0)  return CODEINFONULL;	// ������Ϣ������
	
	// ΪImageCodecInfo����������ռ�
	pImageCodeInfo = (ImageCodecInfo*)malloc(size);
	if (pImageCodeInfo == NULL)  return NOMEMORY;
	
	// ��ȡ����ͼ�����������Ϣ
	GetImageEncoders(numEncoders, size, pImageCodeInfo);	// ��ȡ����ͼ���������Ϣ

	// ����MIME���ͻ�ȡ����GDI+����֧�ֵ�ͼ�������CLSID
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
	// ��ȡͼƬ
	Image image(lpwcstrSrc);		// ��ȡ�ļ�
	// ��������ͼ
	Image* thumbnail = image.GetThumbnailImage(dstWidth, dstHeight);
	// ����
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

	hMemDC = CreateCompatibleDC(NULL);	// ��������Ļ��ʾ���ݵ��ڴ���ʾ�豸

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
	bmiHeader.biPlanes = 1;	// Ŀ���ͼ�豸�����Ĳ�������������Ϊ1

	// ��ʱ�����ܻ�ͼ����Ϊû�еط���
	// 
	hBitmap = CreateDIBSection(hMemDC, &bi, DIB_RGB_COLORS, pData, NULL, 0); // ����Ӧ�ó������ֱ��д��ġ����豸�޹ص�λͼ
	SelectObject(hMemDC, hBitmap);
	RECT rect = {0, 0, width, height};
	FillRect(hMemDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	Graphics graphics(hMemDC);
	graphics.DrawImage(&image, 0, 0, width, height);

	DeleteDC(hMemDC);
	return hBitmap;
}
