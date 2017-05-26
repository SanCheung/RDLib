#pragma once

//class CTextUI2;
//class CExitTPWindow;
class CDlgTestList : public CDialogBase
{
public:
	CDlgTestList(void);
	virtual ~CDlgTestList(void);

	//DUILIB_DEFAULT_METHOD( L"skin/", L"dlgTestList.xml", UI_CLASSSTYLE_FRAME, L"testui_maindlg_classname" )
	RDLIB_DEFAULT_METHOD( L"dlgTestList.xml", UI_CLASSSTYLE_FRAME, L"testui_maindlg_classname" )

	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();

private:
	void	Data2UI();
	CTextUI * AddText( CStringW str );
	CListUI	*_list;
	CListUI	*_list2;
	CListUI	*_list3;

	//CExitTPWindow	*_wndET;
};