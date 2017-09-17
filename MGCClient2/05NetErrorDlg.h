#pragma once
class C05NetErrorDlg  : public CDialogBase
{
public:
	C05NetErrorDlg(void);
	virtual ~C05NetErrorDlg(void);

	RDLIB_DEFAULT_METHOD( L"05NetErrorDlg.xml", UI_CLASSSTYLE_FRAME, L"mgc2_dlg_windowclass" )

	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	// 尝试次数 界面显示
	void	tryTimeToUI( int nTryTime );
};