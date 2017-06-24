#pragma once

#include <d2d1.h>  
#include <d2d1helper.h>  
#include <dwrite.h>
#include <wincodec.h>

#include <vector>
#include <memory>
using namespace std;
using namespace std::tr1;


class D2dWrap
{
public:
	D2dWrap(void);
	virtual ~D2dWrap(void);

	bool	InitD2d( HWND hWnd );
	void	resize( int w, int h );
	bool	draw();
	void	reset();


private:
	ID2D1Factory *				g_factory;  
	ID2D1HwndRenderTarget *		g_render_target;

	ID2D1SolidColorBrush  *		g_brush;  
	IDWriteFactory *			g_write_factory;  
	IDWriteTextFormat *			g_text_format;  

	//ID2D1GradientStopCollection *	g_gradient_stop_collection;  
	//ID2D1LinearGradientBrush  *		g_linear_gradient_brush;  
	//ID2D1RadialGradientBrush *		g_radial_gradient_brush;  

	IWICImagingFactory *		g_image_factory;  
	ID2D1Bitmap *				g_bitmap;


	float		xFrom;
	float		xTo;
	float		xStep;

	int		_index;
	int		_count;
};


//template < class T >
//class AutoReleasePtr
//{
//public:
//	AutoReleasePtr()
//	{
//	}
//
//	~AutoReleasePtr()
//	{
//		if( p != nullptr )
//		{
//			p->Release();
//			p = nullptr;
//		}
//	}
//
//	T* operator-> ()
//	{
//		return _pT;
//	}
//
//private:
//	T*	_pT;
//};

typedef	ID2D1Bitmap				d2Bitmap;
typedef	ID2D1SolidColorBrush	d2SolidBrush;
typedef	IDWriteTextFormat		d2Font;
typedef D2D1::ColorF			d2Color;


class IDrawer
{
public:
	virtual void DoDraw() = 0;
};

class D2dEngine
{
public:
	D2dEngine();
	virtual ~D2dEngine();

	bool	d2Draw( IDrawer *drawer );

	bool	d2Startup();
	bool	d2InitHRT( HWND hWnd );
	void	d2Resize( int w, int h );

	void	d2GetSize( float &w, float &h );

	//void	TestLayer();
	void	TestGDIPlus();

	ID2D1Bitmap*	d2NewBitmap( LPCWSTR strFile );
	void			d2DrawBitmap( ID2D1Bitmap *bmp, float x, float y, float w, float h, float alpha );

	ID2D1SolidColorBrush*	d2NewSolidBrush( D2D1::ColorF cr );
	IDWriteTextFormat*		d2NewFont( LPCWSTR strFontFamily, int nFontSize, int nFontWeight );

	void	d2DrawText( LPCWSTR strText, float x, float y, float w, float h,
						IDWriteTextFormat* font, ID2D1SolidColorBrush* sb );

	template< class T >
	void	d2_safe_release( T *p )
	{
		if( nullptr != p )
		{ 
			p->Release(); 
			p = nullptr;
		}
	}

private:
	ID2D1Factory *				_factory;
	IDWriteFactory *			_write_factory;
	IWICImagingFactory *		_image_factory;

	bool	_bFactoryInited;

	char *imgdata; 
	ID2D1Bitmap* m_pBitmap;

	int _w;
	int _h;

public:
	ID2D1HwndRenderTarget *		_render_target;
};

