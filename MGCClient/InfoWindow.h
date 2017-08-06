#pragma once
class CInfoWindow  : public CWindowWnd
{
public:
	CInfoWindow(void);
	virtual ~CInfoWindow(void);

	static void		Show( HWND hHostWnd );
	static bool		Hide();
	static void		Release();

protected:
	virtual LPCTSTR GetWindowClassName() const { return L"mgc_windowclass"; }
	virtual UINT	GetClassStyle() const { return CS_HREDRAW | CS_VREDRAW | CS_IME | CS_DBLCLKS; }
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);


protected:
	HWND		CreateThis( HWND hHostWnd );

private:
	static	CInfoWindow*	s_instance;

	HWND		_hHostWnd;
};

