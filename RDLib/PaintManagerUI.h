#pragma once

#include "StringPtrMap.h"

#define			WM_RDLIB_CLEANUP			WM_APP+1

typedef CControlUI* (*LPCREATECONTROL)(LPCTSTR pstrType);


class RDLIB_API CPaintManagerUI
{
public:
	CPaintManagerUI();
	~CPaintManagerUI();

public:
	void Init(HWND hWnd);
	void NeedUpdate();
	void Invalidate(RECT& rcItem);

	HDC		GetPaintDC() const				{ return m_hDcPaint; }
	HWND	GetPaintWindow() const			{ return m_hWndPaint; }
	HWND	GetTooltipWindow() const		{ return m_hwndTooltip; }

	CDUIPoint	GetMousePos() const			{ return m_ptLastMousePos; }
	CDUISize	GetClientSize() const;

	CDUISize	GetInitSize()				{ return m_szInitWindowSize; }
	void SetInitSize(int cx, int cy);

	CDUIRect&	GetSizeBox()				{ return m_rcSizeBox; }
	void SetSizeBox(CDUIRect& rcSizeBox)	{ m_rcSizeBox = rcSizeBox; }

	CDUIRect&	GetCaptionRect()				{ return m_rcCaption; }
	void SetCaptionRect( CDUIRect& rcCaption)	{ m_rcCaption = rcCaption; }

	CDUISize GetRoundCorner() const;
	void SetRoundCorner(int cx, int cy);

	CDUISize GetMinInfo() const	{ return m_szMinWindow; }
	void SetMinInfo(int cx, int cy);

	CDUISize GetMaxInfo() const { return m_szMinWindow; }
	void SetMaxInfo(int cx, int cy);

	int GetTransparent() const;
	void SetTransparent(int nOpacity);

	void SetBackgroundTransparent(bool bTrans);
	bool IsShowUpdateRect() const;
	void SetShowUpdateRect(bool show);

	bool UseParentResource(CPaintManagerUI* pm);
	CPaintManagerUI* GetParentResource() const;

	void SetBorderColor( DWORD clrColor );
	DWORD GetBorderColor();

	DWORD GetDefaultDisabledColor() const;
	void SetDefaultDisabledColor(DWORD dwColor);
	DWORD GetDefaultFontColor() const;
	void SetDefaultFontColor(DWORD dwColor);
	DWORD GetDefaultLinkFontColor() const;
	void SetDefaultLinkFontColor(DWORD dwColor);
	DWORD GetDefaultLinkHoverFontColor() const;
	void SetDefaultLinkHoverFontColor(DWORD dwColor);
	DWORD GetDefaultSelectedBkColor() const;
	void SetDefaultSelectedBkColor(DWORD dwColor);
	TFontInfo* GetDefaultFontInfo();
	void SetDefaultFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	DWORD GetCustomFontCount() const;
	HFONT AddFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	//HFONT AddFontAt(int index, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);

	HFONT CloneFont(HFONT hFont);

	HFONT GetFont(int index);
	HFONT GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	bool FindFont(HFONT hFont);
	bool FindFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	int GetFontIndex(HFONT hFont);
	int GetFontIndex(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	int GetFontIndexEx(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	bool RemoveFont(HFONT hFont);
	bool RemoveFontAt(int index);
	void RemoveAllFonts();
	TFontInfo* GetFontInfo(int index);
	TFontInfo* GetFontInfo(HFONT hFont);

	const TImageInfo* GetImage(LPCTSTR bitmap);
	const TImageInfo* GetImageEx(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
	const TImageInfo* AddImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
	bool AddExternalImage(LPCTSTR file);
	const TImageInfo* AddImage(LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, bool bAlpha);
	bool RemoveImage(LPCTSTR bitmap);
	void RemoveAllImages();
	void ReloadAllImages();

	void AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList);
	LPCTSTR GetDefaultAttributeList(LPCTSTR pStrControlName);
	bool RemoveDefaultAttributeList(LPCTSTR pStrControlName);
	const mapsv& GetDefaultAttribultes();
	void RemoveAllDefaultAttributeList();

	bool AttachDialog(CControlUI* pControl);
	bool InitControls(CControlUI* pControl, CControlUI* pParent = NULL);
	void ReapObjects(CControlUI* pControl);

	bool AddOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
	//CStdPtrArray* GetOptionGroup(LPCTSTR pStrGroupName);
	vectorv* GetOptionGroup(LPCTSTR pStrGroupName);
	void RemoveOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
	void RemoveAllOptionGroups();

	CControlUI* GetFocus() const;
	void SetFocus(CControlUI* pControl);
	void SetFocusNeeded(CControlUI* pControl);

	bool SetNextTabControl(bool bForward = true);

	bool SetTimer(CControlUI* pControl, UINT nTimerID, UINT uElapse);
	bool KillTimer(CControlUI* pControl, UINT nTimerID);
	void KillTimer(CControlUI* pControl);
	void RemoveAllTimers();

	void SetCapture();
	void ReleaseCapture();
	bool IsCaptured();

	bool AddNotifier(INotifyUI* pControl);
	bool RemoveNotifier(INotifyUI* pControl);   
	void SendNotify(TNotifyUI& Msg, bool bAsync = false);
	void SendNotify(CControlUI* pControl, LPCTSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false);

	bool AddPreMessageFilter(IMessageFilterUI* pFilter);
	bool RemovePreMessageFilter(IMessageFilterUI* pFilter);

	bool AddMessageFilter(IMessageFilterUI* pFilter);
	bool RemoveMessageFilter(IMessageFilterUI* pFilter);

	int GetPostPaintCount() const;
	bool AddPostPaint(CControlUI* pControl);
	bool RemovePostPaint(CControlUI* pControl);
	//bool SetPostPaintIndex(CControlUI* pControl, int iIndex);

	void AddDelayedCleanup(CControlUI* pControl);

	bool AddTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);
	bool RemoveTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);
	bool TranslateAccelerator(LPMSG pMsg);

	CControlUI* GetRoot() const;
	CControlUI* FindControl(POINT pt) const;
	CControlUI* FindControl(LPCTSTR pstrName) const;
	CControlUI* FindSubControlByPoint(CControlUI* pParent, POINT pt) const;
	CControlUI* FindSubControlByName(CControlUI* pParent, LPCTSTR pstrName) const;
	CControlUI* FindSubControlByClass(CControlUI* pParent, LPCTSTR pstrClass, int iIndex = 0);
	vectorv* FindSubControlsByClass(CControlUI* pParent, LPCTSTR pstrClass);
	vectorv* GetSubControlsByClass();

	//static void MessageLoop();
	//static bool TranslateMessage(const LPMSG pMsg);
	//static void Term();

	bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
	bool PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);

	bool WindowAttached();

	void OnPaint();


public:
	static CControlUI* CALLBACK __FindControlFromNameHash(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlFromCount(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlFromPoint(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlFromTab(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlFromShortcut(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlFromUpdate(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlFromName(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlFromClass(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlsFromClass(CControlUI* pThis, LPVOID pData);


private:
	HWND m_hWndPaint;
	int m_nOpacity;
	HDC m_hDcPaint;
	HDC m_hDcOffscreen;
	HDC m_hDcBackground;
	HBITMAP m_hbmpOffscreen;
	HBITMAP m_hbmpBackground;
	HWND m_hwndTooltip;
	TOOLINFO m_ToolTip;
	bool m_bShowUpdateRect;
	//
	CControlUI* m_pRoot;
	CControlUI* m_pFocus;
	CControlUI* m_pEventHover;
	CControlUI* m_pEventClick;
	CControlUI* m_pEventKey;

	//
	CDUIPoint	m_ptLastMousePos;

	CDUISize	m_szMinWindow;
	CDUISize	m_szMaxWindow;
	CDUISize	m_szInitWindowSize;
	CDUISize	m_szRoundCorner;

	CDUIRect	m_rcSizeBox;
	CDUIRect	m_rcCaption;

	UINT m_uTimerID;
	bool m_bFirstLayout;
	bool m_bUpdateNeeded;
	bool m_bFocusNeeded;
	bool m_bOffscreenPaint;
	bool m_bAlphaBackground;
	bool m_bMouseTracking;
	bool m_bMouseCapture;
	bool m_bWindowAttached;


	// 通知者
	vectorv		m_aNotifiers;
	
	// 计时器
	vectorv		m_aTimers;
	
	// 预处理消息
	vectorv		m_aPreMessageFilters;

	// 消息
	vectorv		m_aMessageFilters;

	// 预绘制控件
	vectorv		m_aPostPaintControls;

	// 延迟删除
	vectorv		m_aDelayedCleanup;

	// 消息缓冲
	dequev		m_aAsyncNotify;

	// 控件查找缓冲
	vectorv		m_aFoundControls;

	// 名字映射表
	mapsv		m_mNameHash;

	// 选项卡分组表
	mapsv		m_mOptionGroup;


	//CStdPtrArray m_aNotifiers;
	//CStdPtrArray m_aTimers;
	//CStdPtrArray m_aPreMessageFilters;
	//CStdPtrArray m_aMessageFilters;
	//CStdPtrArray m_aPostPaintControls;
	//CStdPtrArray m_aDelayedCleanup;
	//CStdPtrArray m_aAsyncNotify;
	//CStdPtrArray m_aFoundControls;
	//CStringPtrMap m_mNameHash;
	//CStringPtrMap m_mOptionGroup;

	//
	CPaintManagerUI* m_pParentResourcePM;
	DWORD m_dwDefaultDisabledColor;
	DWORD m_dwDefaultFontColor;
	DWORD m_dwDefaultLinkFontColor;
	DWORD m_dwDefaultLinkHoverFontColor;
	DWORD m_dwDefaultSelectedBkColor;
	DWORD m_dwBorderColor;
	TFontInfo m_DefaultFontInfo;

	vectorv		m_aCustomFonts;
	mapsv		m_mImageHash;
	mapsv		m_DefaultAttrHash;

	//CStdPtrArray m_aCustomFonts;
	//CStringPtrMap m_mImageHash;
	//CStringPtrMap m_DefaultAttrHash;

	//////////////////////////////////////////////////////////////////////////
public:
	setv	m_aTranslateAccelerator;

	//bool			m_IsDoDragDroping;
	//CDragEventArgs* m_DragEventArgs;
	//CControlUI*		m_DropTarget;
	//CControlUI*		m_DragSource;


	bool		m_bExiting;
	CStringW	m_strScript;


private:
	void*	mapsv_find( const mapsv &m, LPCTSTR str );
};

