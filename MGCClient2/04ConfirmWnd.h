#pragma once

// ×÷·Ï£¡£¡£¡£¡
#include "TSingletonWindow.h"

class C04ConfirmWnd : public TSingletonWindow< C04ConfirmWnd >
{
public:
	C04ConfirmWnd(void);
	virtual ~C04ConfirmWnd(void);

	shared_ptr<Image>	_spCancel1;
	shared_ptr<Image>	_spCancel2;
	shared_ptr<Image>	_spCancel3;

	shared_ptr<Image>	_spOK1;
	shared_ptr<Image>	_spOK2;
	shared_ptr<Image>	_spOK3;

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

