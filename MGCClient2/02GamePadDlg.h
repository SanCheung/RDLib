#pragma once
class C02GamePadDlg : public CDialogBase
{
public:
	C02GamePadDlg(void);
	virtual ~C02GamePadDlg(void);

	RDLIB_DEFAULT_METHOD( L"02GamePadDlg.xml", UI_CLASSSTYLE_FRAME, L"02GamePad_windowclass" )

	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

