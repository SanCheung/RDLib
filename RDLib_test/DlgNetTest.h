#pragma once
class CDlgNetTest : public CDialogBase
{
public:
	CDlgNetTest(void);
	virtual ~CDlgNetTest(void);

	RDLIB_DEFAULT_METHOD( L"NetTest.xml", UI_CLASSSTYLE_FRAME, L"rdlib_classname" )

	//LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();

	// 0: 网络正常，无须弹出对话框
	// 1: 弹出对话框后，网络正常
	// 2: 弹出对话框后，点击退出
	static int	PopNetTestDlg();
	static bool NetOK();
};

