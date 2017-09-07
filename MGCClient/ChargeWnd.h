#pragma once

#define		WM_EXITTPWND_CLICK				WM_APP + 100

class CChargeWnd : public CWindowWnd
				 , xdmp::ThreadWrap
{
public:
	CChargeWnd(void);
	virtual ~CChargeWnd(void);

	virtual LPCTSTR GetWindowClassName() const { return L"mgc_windowclass"; }
	virtual UINT	GetClassStyle() const { return CS_HREDRAW | CS_VREDRAW | CS_IME | CS_DBLCLKS; }
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);


	static void		Show( HWND hParentWnd );
	static bool		Hide();
	static void		Release();

protected:
	HWND			CreateThis( HWND hHostWnd );

	virtual void	thread_main();

private:
	//void	Update( int nType );
	void	Update();

	bool	m_bMouseTracking;
	bool	m_bPosChanged;

	CDUIPoint	_ptNow;
	HWND		_hHostWnd;

	static	CChargeWnd*	s_instance;

	int		_nType;

	void	UpdateNormal();
	void	UpdateHover();
	void	UpdateButtonHover();

	void	DrawButton( Graphics *pGr, int top, bool bButtonHover );
	void	DrawInfo( Graphics *pGr );

	// 是否正在进行下线操作
	bool	_bOfflineing;
};

