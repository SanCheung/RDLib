#pragma once
class CDlgNetTest : public CDialogBase
{
public:
	CDlgNetTest(void);
	virtual ~CDlgNetTest(void);

	RDLIB_DEFAULT_METHOD( L"NetTest.xml", UI_CLASSSTYLE_FRAME, L"rdlib_classname" )

	//LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();

	// 0: �������������뵯���Ի���
	// 1: �����Ի������������
	// 2: �����Ի���󣬵���˳�
	static int	PopNetTestDlg();
	static bool NetOK();
};

