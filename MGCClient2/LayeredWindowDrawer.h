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

