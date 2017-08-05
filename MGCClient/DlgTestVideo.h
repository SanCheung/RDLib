#pragma once

#include <string>
#include "vlc/vlc.h"
#include "vlc/libvlc.h"

// libvlc.lib;libvlccore.lib;
#pragma comment( lib, "libvlc.lib" )
#pragma comment( lib, "libvlccore.lib" )


class  CMyOwnerWndUI: public CControlUI
{
public:
	CMyOwnerWndUI(): m_hWnd(NULL){}

	virtual void SetVisible(bool bVisible = true)
	{
		__super::SetVisible(bVisible);
		::ShowWindow(m_hWnd, bVisible);
	}

	virtual void SetInternVisible(bool bVisible = true)
	{
		__super::SetInternVisible(bVisible);
		::ShowWindow(m_hWnd, bVisible);
	}

	virtual void SetPos(RECT rc)
	{
		__super::SetPos(rc);
		::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
	}

	BOOL Attach(HWND hWndNew)
	{
		if (! ::IsWindow(hWndNew))
		{
			return FALSE;
		}

		m_hWnd = hWndNew;
		return TRUE;
	}

	HWND Detach()
	{
		HWND hWnd = m_hWnd;
		m_hWnd = NULL;
		return hWnd;
	}

	HWND GetHWND()
	{
		return m_hWnd;
	}

protected:
	HWND m_hWnd;
};

enum PlayerState{
	STATE_PREPARE,
	STATE_PLAY,
	STATE_PAUSE
};






class CDlgTestVideo  : public CDialogBase
{
public:
	CDlgTestVideo(void);
	virtual ~CDlgTestVideo(void);

	RDLIB_DEFAULT_METHOD( L"dlgTestVideo.xml", UI_CLASSSTYLE_FRAME, L"testui_maindlg_classname" )

	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	static	void CallbackPlaying(void *data);
	static	void CallbackPosChanged(void *data);
	static	void CallbackEndReached(void *data);

	//CAVPlayer	m_player;

	libvlc_instance_t * libvlc_inst;
	libvlc_media_player_t *libvlc_mp;
	libvlc_media_t *libvlc_m;
	PlayerState playerstate;
};

