#pragma once
class CWindowTip : public CWindowWnd
{
public:
	CWindowTip(void);
	virtual ~CWindowTip(void);


	virtual LPCTSTR GetWindowClassName() const { return L"mgc_windowclass"; }
	virtual UINT	GetClassStyle() const { return CS_HREDRAW | CS_VREDRAW | CS_IME | CS_DBLCLKS; }
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);


	static void		Show( HWND hParentWnd );
	static bool		Hide();
	static void		Release();

protected:
	HWND		CreateThis( HWND hHostWnd );

private:
	void	Update();
	HWND	_hHostWnd;

	static	CWindowTip*	s_instance;
};

