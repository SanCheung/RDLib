#pragma once

class CMainDlg : public CDialogBase
{
public:
	CMainDlg(void);
	virtual ~CMainDlg(void);

	//DUILIB_DEFAULT_METHOD( L"skin/", L"dlgMain.xml", UI_CLASSSTYLE_FRAME, L"testui_maindlg_classname" )
	RDLIB_DEFAULT_METHOD( L"dlgMain.xml", UI_CLASSSTYLE_FRAME, L"testui_maindlg_classname" );

	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();
};

