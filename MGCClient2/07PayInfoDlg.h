#pragma once
class C07PayInfoDlg  : public CDialogBase
{
public:
	C07PayInfoDlg(void);
	virtual ~C07PayInfoDlg(void);

	RDLIB_DEFAULT_METHOD( L"07PayInfoDlg.xml", UI_CLASSSTYLE_FRAME, L"mgc2_dlg_windowclass" )

	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	// 倒计时的界面显示
	void	secondToUI( int nSecond );
};