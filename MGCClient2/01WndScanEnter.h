#pragma once
#include "TSingletonWindow.h"

// 作废！！！
class C01WndScanEnter : public TSingletonWindow< C01WndScanEnter >
{
public:
	C01WndScanEnter(void);
	virtual ~C01WndScanEnter(void);

	shared_ptr<Image>	_spImageBK;
	shared_ptr<Image>	_spImage2w;
	shared_ptr<Image>	_spImageBn;

	int		_bHover;

	int		_time;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


// 请使用这个！
class C01ScanEnterDlg  : public CDialogBase
{
public:
	C01ScanEnterDlg(void);
	virtual ~C01ScanEnterDlg(void);

	RDLIB_DEFAULT_METHOD( L"01ScanEnterDlg.xml", UI_CLASSSTYLE_FRAME, L"mgc2_dlg_windowclass" )

	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();

	int		_time;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};
