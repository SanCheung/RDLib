#pragma once
#include "WndShadow.h"

//using namespace DuiLib;

// dialog default method
//#define DUILIB_DEFAULT_METHOD( SubFolder, XmlFile, ClassStyle, ClassName )
#define RDLIB_DEFAULT_METHOD( XmlFile, ClassStyle, ClassName )  \
public: \
	virtual CStringW GetSkinFile()		{ return XmlFile; } \
	UINT GetClassStyle() const			{ return ClassStyle; } \
	LPCTSTR GetWindowClassName() const	{ return ClassName; } \
	virtual CControlUI* CreateControl(LPCTSTR pstrClass); \
	virtual void	Notify(TNotifyUI& msg);


// virtual CStringW GetSkinFolder()	{ return CAppData::GetInstancePath() + SubFolder; } \

class RDLIB_API CDialogBase	: public CDialogWnd
							  , public INotifyUI
							  , public IMessageFilterUI
							  , public IDialogBuilderCallback
{
public:
	CDialogBase(void);
	virtual ~CDialogBase(void);

	virtual void OnFinalMessage(HWND hWnd);

	virtual UINT GetClassStyle() const;

	virtual void Init();

	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	

	virtual LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	static void Cleanup();

protected:
	//virtual CStringW	GetSkinFolder();
	virtual CStringW	GetSkinFile() = 0;
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

protected:
	CPaintManagerUI paint_manager_;

	CWndShadow		m_shadow;

public:
	template< class T>
	T* FindCtrl( wchar_t* name )
	{
		return (T *)paint_manager_.FindControl( name );
	}

	CControlUI* FindCtrl( wchar_t* name )
	{
		return paint_manager_.FindControl( name );
	}

	// nType
	int MsgBox( CStringW strInfo, CStringW strTitle = L"", int nType = 0, int nButton = MB_OK );
	int MsgBoxError( CStringW strInfo, CStringW strTitle = L"" )	{ return MsgBox( strInfo, strTitle, MB_ICONEXCLAMATION ); }

	UINT DoModal( HWND hWnd, CStringW strCaption, DWORD dwStyle = WS_POPUPWINDOW|WS_CAPTION );
	UINT DoModalNoCaption( HWND hWnd );

	void		setEditText( CStringW strCtrl, CStringW strText );
	void		getEditText( CStringW strCtrl, CStringW &strText );
	CStringW	getEditText( CStringW strCtrl );

	void		setComboValue( CStringW strCtrl, int i );
	int			getComboValue( CStringW strCtrl );
};

