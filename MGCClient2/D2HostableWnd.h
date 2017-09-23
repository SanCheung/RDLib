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

	// IDrawer 默认绘制入口
	virtual void	DoDraw() = 0;
	
	// 建立所有绘制元素
	virtual void	dh_BuildAllBitmap() = 0;

	virtual LRESULT HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );

	// 动画控制
	virtual void	dh_AniStart();
	virtual void	dh_AniProc();
	virtual void	dh_AniEnd();

protected:
	D2dObjectLib	_de;
	bool	_animing;

};

