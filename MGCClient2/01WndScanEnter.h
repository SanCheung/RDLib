#pragma once

#include "TSingletonWindow.h"

class C01WndScanEnter : public TSingletonWindow< C01WndScanEnter >
{
public:
	C01WndScanEnter(void);
	virtual ~C01WndScanEnter(void);

	shared_ptr<Image>	_spImageBK;
	shared_ptr<Image>	_spImage2w;
	shared_ptr<Image>	_spImageBn;

	int		_bHover;

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

