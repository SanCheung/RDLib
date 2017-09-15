#pragma once
#include "HScrollUI.h"

class CDlgNew	: public CDialogBase
				, public IScrollHost
{
public:
	CDlgNew(void);
	virtual ~CDlgNew(void);

	RDLIB_DEFAULT_METHOD( L"new.xml", UI_CLASSSTYLE_FRAME, L"testui_maindlg_classname" )

	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();

private:
	void	Data2UI();

	virtual void OnSetRatio( PointF pt );


	CVScrollUI		*m_vs;
};