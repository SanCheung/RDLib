#pragma once

class CDemoDlg  : public CDialogBase
				, xdmp::ThreadWrap
{
public:
	CDemoDlg(void);
	virtual ~CDemoDlg(void);

	RDLIB_DEFAULT_METHOD( L"dlgDemo.xml", UI_CLASSSTYLE_FRAME, L"mgc_windowclass" )

	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void	ShowInfo( CStringW strInfo );

	virtual void	thread_main();
};

