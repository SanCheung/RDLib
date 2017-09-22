#pragma once
class CLayeredWindowDrawer
{
public:
	CLayeredWindowDrawer( HWND hWnd, Image *image, const POINT ptTo );
	virtual ~CLayeredWindowDrawer(void);

	Graphics* getDrawer(){ return _spGr.get(); }

private:
	HWND	m_hWnd;
	Image*  m_image;
	POINT	m_ptTo;

	int		_iw;
	int		_ih;

	HDC		_hDcScreen;
	HDC		_hDcMem;
	HBITMAP	_hBitmap;
	HBITMAP _hBitmapOld;

	shared_ptr<Graphics>	_spGr;
};


class CLayeredWindowDrawer2
{
public:
	CLayeredWindowDrawer2( HWND hWnd, CDUIRect &rt );
	virtual	~CLayeredWindowDrawer2();

	HDC		getDrawDC(){ return _hDcMem; }
private:
	HWND		m_hWnd;
	CDUIRect	m_rt;

	HWND		m_hWnd2;


	HDC		_hDcScreen;
	HDC		_hDcMem;
	HBITMAP	_hBitmap;
	HBITMAP _hBitmapOld;
};

