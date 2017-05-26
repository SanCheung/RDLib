#pragma once

#define UI_WNDSTYLE_CONTAINER  (0)
#define UI_WNDSTYLE_FRAME      (WS_VISIBLE | WS_OVERLAPPEDWINDOW)
#define UI_WNDSTYLE_CHILD      (WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
#define UI_WNDSTYLE_DIALOG     (WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)

#define UI_WNDSTYLE_EX_FRAME   (WS_EX_WINDOWEDGE)
#define UI_WNDSTYLE_EX_DIALOG  (WS_EX_TOOLWINDOW | WS_EX_DLGMODALFRAME)

#define UI_CLASSSTYLE_CONTAINER  (0)
#define UI_CLASSSTYLE_FRAME      (CS_VREDRAW | CS_HREDRAW)
#define UI_CLASSSTYLE_CHILD      (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)
#define UI_CLASSSTYLE_DIALOG     (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)



/////////////////////////////////////////////////////////////////////////////////////
//

typedef enum EVENTTYPE_UI
{
	UIEVENT__FIRST = 1,
	UIEVENT__KEYBEGIN,
	UIEVENT_KEYDOWN,
	UIEVENT_KEYUP,
	UIEVENT_CHAR,
	UIEVENT_SYSKEY,
	UIEVENT__KEYEND,
	UIEVENT__MOUSEBEGIN,
	UIEVENT_MOUSEMOVE,
	UIEVENT_MOUSELEAVE,
	UIEVENT_MOUSEENTER,
	UIEVENT_MOUSEHOVER,
	UIEVENT_BUTTONDOWN,
	UIEVENT_BUTTONUP,
	UIEVENT_RBUTTONDOWN,
	UIEVENT_DBLCLICK,
	UIEVENT_CONTEXTMENU,
	UIEVENT_SCROLLWHEEL,
	UIEVENT__MOUSEEND,
	UIEVENT_KILLFOCUS,
	UIEVENT_SETFOCUS,
	UIEVENT_WINDOWSIZE,
	UIEVENT_SETCURSOR,
	UIEVENT_TIMER,
	UIEVENT_NOTIFY,
	UIEVENT_COMMAND,
	UIEVENT__LAST,
};

/////////////////////////////////////////////////////////////////////////////////////
//
//
// 20130307

// Styles for the DoPaintFrame() helper
#define UIFRAME_ROUND        0x00000001
#define UIFRAME_FOCUS        0x00000002

// Styles for the DoPaintArcCaption() helper
#define UIARC_GRIPPER        0x00000001
#define UIARC_EXPANDBUTTON   0x00000002
#define UIARC_COLLAPSEBUTTON 0x00000004



// System settings
typedef struct tagTSystemSettingsUI
{
	bool bShowKeyboardCues;
	bool bScrollLists;
} TSystemSettingsUI;

// Various system settings
typedef struct tagTSystemMetricsUI
{
	INT cxvscroll;
} TSystemMetricsUI;








// Flags for CControlUI::GetControlFlags()
#define UIFLAG_TABSTOP       0x00000001
#define UIFLAG_SETCURSOR     0x00000002
#define UIFLAG_WANTRETURN    0x00000004

// Flags for FindControl()
#define UIFIND_ALL           0x00000000
#define UIFIND_VISIBLE       0x00000001
#define UIFIND_ENABLED       0x00000002
#define UIFIND_HITTEST       0x00000004
#define UIFIND_TOP_FIRST     0x00000008
#define UIFIND_ME_FIRST      0x80000000

// Flags for the CDialogLayout stretching
#define UISTRETCH_NEWGROUP   0x00000001
#define UISTRETCH_NEWLINE    0x00000002
#define UISTRETCH_MOVE_X     0x00000004
#define UISTRETCH_MOVE_Y     0x00000008
#define UISTRETCH_CDUISize_X     0x00000010
#define UISTRETCH_CDUISize_Y     0x00000020

// Flags used for controlling the paint
#define UISTATE_FOCUSED      0x00000001
#define UISTATE_SELECTED     0x00000002
#define UISTATE_DISABLED     0x00000004
#define UISTATE_HOT          0x00000008
#define UISTATE_PUSHED       0x00000010
#define UISTATE_READONLY     0x00000020
#define UISTATE_CAPTURED     0x00000040
#define UISTATE_CHECKED		 0x00000080


// Flags for DoAnumateWindow()
#define UIANIM_FADE          0x00000001
#define UIANIM_HIDE          0x00000002

/////////////////////////////////////////////////////////////////////////////////////
//

typedef struct tagTFontInfo
{
	HFONT		hFont;
	TCHAR		sFontName[64];
	int			iSize;

	bool bBold;
	bool bUnderline;
	bool bItalic;
	TEXTMETRIC tm;
} TFontInfo;

typedef struct tagTImageInfo
{
	HBITMAP hBitmap;
	int nX;
	int nY;
	bool alphaChannel;
	TCHAR sResType[512];
	DWORD dwMask;
} TImageInfo;


class CControlUI;


// Structure for notifications from the system
// to the control implementation.
typedef struct tagTEventUI
{
	int Type;
	CControlUI* pSender;
	DWORD dwTimestamp;
	POINT ptMouse;
	TCHAR chKey;
	WORD wKeyState;
	WPARAM wParam;
	LPARAM lParam;
} TEventUI;

// Structure for notifications to the outside world
typedef struct tagTNotifyUI 
{
	CStringW sType;
	CControlUI* pSender;
	DWORD dwTimestamp;
	POINT ptMouse;
	WPARAM wParam;
	LPARAM lParam;
} TNotifyUI;

// Structure for relative position to the parent
typedef struct tagTRelativePosUI
{
	bool bRelative;
	SIZE szParent;
	int nMoveXPercent;
	int nMoveYPercent;
	int nZoomXPercent;
	int nZoomYPercent;
}TRelativePosUI;



// Listener interface
class INotifyUI
{
public:
	virtual void Notify(TNotifyUI& msg) = 0;
};



// MessageFilter interface
class IMessageFilterUI
{
public:
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) = 0;
};



class ITranslateAccelerator
{
public:
	virtual LRESULT TranslateAccelerator(MSG *pMsg) = 0;
};