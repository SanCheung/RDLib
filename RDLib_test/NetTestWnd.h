#pragma once
class CNetTestWnd	: public CWindowWnd
					, public INotifyUI
					, public IMessageFilterUI
					, public IDialogBuilderCallback
{
public:
	CNetTestWnd(void);
	virtual ~CNetTestWnd(void);

	virtual LPCTSTR GetWindowClassName() const { return L"wcn_nettest"; }
	virtual UINT	GetClassStyle() const { return CS_HREDRAW | CS_VREDRAW | CS_IME | CS_DBLCLKS; }

	static void		Show();
	static bool		Hide();
	static void		Release();


	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	virtual void	Notify(TNotifyUI& msg);

	LRESULT	OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	HWND		CreateThis();
	void		HideThis();
	static CNetTestWnd*	s_instance;

	bool		NetOK();

	CPaintManagerUI paint_manager_;
	CWndShadow		m_shadow;
};

