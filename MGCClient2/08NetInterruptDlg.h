#pragma once
#include "LayeredDialogBase.h"

class C08NetInterruptDlg  : public CLayeredDialogBase
{
public:
	C08NetInterruptDlg(void);
	virtual ~C08NetInterruptDlg(void);

	RDLIB_DEFAULT_METHOD( L"08NetInterruptDlg.xml", UI_CLASSSTYLE_FRAME, L"mgc2_dlg_windowclass" )

	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	// 倒计时的界面显示
	void	secondToUI( int nSecond );
};