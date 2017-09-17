#pragma once
class C06AdminLoginDlg  : public CDialogBase
{
public:
	C06AdminLoginDlg(void);
	virtual ~C06AdminLoginDlg(void);

	RDLIB_DEFAULT_METHOD( L"06AdminLoginDlg.xml", UI_CLASSSTYLE_FRAME, L"mgc2_dlg_windowclass" )

	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};