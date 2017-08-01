#pragma once

#define		WM_EXITTPWND_CLICK				WM_APP + 100

class CExitTPWindow : public CWindowWnd
{
public:
	CExitTPWindow(void);
	virtual ~CExitTPWindow(void);

	virtual LPCTSTR GetWindowClassName() const { return L"wcn_exittp"; }
	virtual UINT	GetClassStyle() const { return CS_HREDRAW | CS_VREDRAW | CS_IME | CS_DBLCLKS; }
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);


	static void		Show( HWND hParentWnd );
	static bool		Hide();
	static void		Release();

protected:
	HWND		CreateThis( HWND hHostWnd );

private:
	void	Update( int nType );

	bool	m_bMouseTracking;
	bool	m_bPosChanged;

	CDUIPoint	_ptNow;
	HWND		_hHostWnd;

	CWndShadow	m_shadow;

	static	CExitTPWindow*	s_instance;
};

