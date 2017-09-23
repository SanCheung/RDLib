#pragma once
#include "D2dObjectLib.h"


class CD2HostableWnd	: public CHostableWnd
						, public IDrawer
{
public:
	CD2HostableWnd(void);
	virtual ~CD2HostableWnd(void);

	virtual UINT GetClassStyle() const			{ return UI_CLASSSTYLE_CHILD; }
	virtual LPCTSTR GetWindowClassName() const	{ return L"ui_d2hostableWindow_classname"; }

	// IDrawer Ĭ�ϻ������
	virtual void	DoDraw() = 0;
	
	// �������л���Ԫ��
	virtual void	dh_BuildAllBitmap() = 0;

	virtual LRESULT HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );

	// ��������
	virtual void	dh_AniStart();
	virtual void	dh_AniProc();
	virtual void	dh_AniEnd();

protected:
	D2dObjectLib	_de;
	bool	_animing;

};

