#pragma once

#include "vlc/vlc.h"
#include "vlc/libvlc.h"

//typedef void (* pfnCallback) (void *data);

class CVlcWindow : public CWindowWnd
{
public:
	CVlcWindow(void);
	virtual  ~CVlcWindow(void);

	static HWND		Show( HWND hParentWnd );
	static bool		Hide();
	static void		Release();
	static bool		IsShow();

protected:
	virtual LPCTSTR GetWindowClassName() const { return L"mgc_windowclass"; }
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
	//libvlc_media_t*			libvlc_m;

	//pfnCallback           m_pfnPlaying;				//�ļ���ȡ��ϣ�׼������
	//pfnCallback           m_pfnPosChanged;			//�ļ�λ�øı�ʱ�Ļص�����
	//pfnCallback           m_pfnEndReached;			//�ļ�������ϵĻص�����

	static void		OnVLC_Event(const libvlc_event_t *event, void *data);

	void	vlcReleasePlayer();
	void	Play();
	void	Replay();
};

