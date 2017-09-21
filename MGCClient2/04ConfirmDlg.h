#pragma once
#include "LayeredDialogBase.h"

class C04ConfirmDlg  : public CLayeredDialogBase
{
public:
	C04ConfirmDlg(void);
	virtual ~C04ConfirmDlg(void);

	RDLIB_DEFAULT_METHOD( L"04ConfirmDlg.xml", UI_CLASSSTYLE_FRAME, L"mgc2_dlg_windowclass" )

	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	//virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//void	layerDraw();
};

