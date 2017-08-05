#pragma once

#include "vlc/vlc.h"
#include "vlc/libvlc.h"

class CVlcWindow : public CWindowWnd
{
public:
	CVlcWindow(void);
	virtual  ~CVlcWindow(void);

	static void		Show( HWND hParentWnd );
	static bool		Hide();
	static void		Release();

protected:
	virtual LPCTSTR GetWindowClassName() const { return L"wcn_exittp"; }
	virtual UINT	GetClassStyle() const { return CS_HREDRAW | CS_VREDRAW | CS_IME | CS_DBLCLKS; }
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);


protected:
	HWND		CreateThis( HWND hHostWnd );

private:
	static	CVlcWindow*	s_instance;

	HWND		_hHostWnd;


protected:
	libvlc_instance_t*		libvlc_inst;
	libvlc_media_player_t*	libvlc_mp;
	libvlc_media_t*			libvlc_m;
};

