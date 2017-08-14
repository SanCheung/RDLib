#pragma once
class CDlgConfirm : public CDialogBase
{
public:
	CDlgConfirm(void);
	virtual ~CDlgConfirm(void);

	RDLIB_DEFAULT_METHOD( L"dlgConfirm.xml", UI_CLASSSTYLE_FRAME, L"mgc_ws_confirm" )

	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

