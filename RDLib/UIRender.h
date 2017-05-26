#ifndef __UIRENDER_H__
#define __UIRENDER_H__

#pragma once

//namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//

class RDLIB_API CRenderClip
{
public:
    ~CRenderClip();
    RECT rcItem;
    HDC hDC;
    HRGN hRgn;
    HRGN hOldRgn;

    static void GenerateClip(HDC hDC, RECT rc, CRenderClip& clip);
    static void GenerateRoundClip(HDC hDC, RECT rc, RECT rcItem, int width, int height, CRenderClip& clip);
    static void UseOldClipBegin(HDC hDC, CRenderClip& clip);
    static void UseOldClipEnd(HDC hDC, CRenderClip& clip);
};

/////////////////////////////////////////////////////////////////////////////////////
//

class RDLIB_API WindowsGraphics
{
public:
	WindowsGraphics(){
		hDC = NULL;
	}
	~WindowsGraphics(){
		DeleteDC(hDC);
	}
	bool CreateAsMeasurementWindowsGraphics(){
		if (!hDC){
			hDC = CreateCompatibleDC(0);
		}
		return hDC != NULL;
	}
	 operator HDC () const{
		return hDC;
	}
private:
	HDC hDC;
};

class RDLIB_API TextMeasureHelper
{
public:
	static SIZE MeasureText(HDC hDC, LPCTSTR pstrText, HFONT hFont, SIZE proposedSize, DWORD format)
	{
		static SIZE MaxSize = {0x7fffffff, 0x7fffffff};

		SIZE size ={0, 0};
		if (hDC && pstrText)
		{	
			HFONT hOldFont = NULL;
			if (hFont){
				hOldFont = (HFONT)SelectObject(hDC, hFont);
			}

			if ((proposedSize.cy >= MaxSize.cy) && ((format & DT_SINGLELINE) != DT_TOP))
			{
				format &= ~(DT_BOTTOM | DT_VCENTER);
			}

			if (proposedSize.cx == MaxSize.cx)
			{
				format &= ~DT_WORDBREAK;
			}
			format |= DT_CALCRECT;

			RECT rc = {0, 0, proposedSize.cx, proposedSize.cy};
			
			DRAWTEXTPARAMS DTParams ={0};
			DTParams.cbSize = sizeof(DTParams);
			DTParams.iLeftMargin = DTParams.iRightMargin = 2;

			DrawTextEx(hDC, (LPTSTR)pstrText, -1, &rc, format, &DTParams);
			size.cx = rc.right - rc.left;
			size.cy = rc.bottom - rc.top;

			if (hOldFont){
				SelectObject(hDC, hOldFont);
			}
		}
		return size;
	}
	

private:
	//static WindowsGraphics s_MeasurementDC;
	//static CStringPtrMap s_MeasurementCache;
};

//////////////////////////////////////////////////////////////////////////

class RDLIB_API CRenderEngine
{
public:
    static DWORD AdjustColor(DWORD dwColor, short H, short S, short L);
    static TImageInfo* LoadImage(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0);

	static TImageInfo* LoadImageFromBuffer( LPBYTE pData, DWORD dwSize, DWORD mask );

	static void FreeImage(const TImageInfo* bitmap);
    static void DrawImage(HDC hDC, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint, \
        const RECT& rcBmpPart, const RECT& rcCorners, bool alphaChannel, BYTE uFade = 255, 
        bool hole = false, bool xtiled = false, bool ytiled = false);
    static bool DrawImageString(HDC hDC, CPaintManagerUI* pManager, const RECT& rcItem, const RECT& rcPaint, 
        LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);
    static void DrawColor(HDC hDC, const RECT& rc, DWORD color);
    static void DrawGradient(HDC hDC, const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps);

    // 以下函数中的颜色参数alpha值无效
    static void DrawLine(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor);
    static void DrawRect(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor);
    static void DrawRoundRect(HDC hDC, const RECT& rc, int width, int height, int nSize, DWORD dwPenColor);
    static void DrawText(HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, \
        DWORD dwTextColor, int iFont, UINT uStyle);
    static void DrawHtmlText(HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, 
        DWORD dwTextColor, int iFont, RECT* pLinks, CStringW* sLinks, int& nLinkRects, UINT uStyle);
    static HBITMAP GenerateBitmap(CPaintManagerUI* pManager, CControlUI* pControl, RECT rc);
};

//} // namespace DuiLib

#endif // __UIRENDER_H__
