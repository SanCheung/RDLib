#pragma once
class IScrollHost
{
public:
	virtual void OnSetRatio( PointF pt ) = 0;
};

class CHScrollUI : public CControlUI
{
public:
	CHScrollUI(void);
	virtual ~CHScrollUI(void);

	void	Event( TEventUI& event );
	void	DoPaint(HDC hDC, const RECT& rcPaint);

	void	SetRatio( float fRatio );
	void	SetHost( IScrollHost *host ) { _host = host; }

private:
	int		_x;

	bool		_bMouseDown;
	int			_xDown;

	IScrollHost *_host;
};


class CVScrollUI : public CControlUI
{
public:
	CVScrollUI(void);
	virtual ~CVScrollUI(void);

	void	Event( TEventUI& event );
	void	DoPaint(HDC hDC, const RECT& rcPaint);

	void	SetRatio( float fRatio );
	void	SetHost( IScrollHost *host ) { _host = host; }

private:
	int		_y;

	bool		_bMouseDown;
	int			_yDown;
	IScrollHost *_host;
};

