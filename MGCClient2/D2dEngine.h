#pragma once

#include <d2d1.h>  
#include <d2d1helper.h>  
#include <dwrite.h>
#include <wincodec.h>

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

	ID2D1Bitmap*	d2NewBitmap( LPCWSTR strFile );
	void			d2DrawBitmap( ID2D1Bitmap *bmp, float x, float y, float w, float h, float alpha = 1.f );

	ID2D1SolidColorBrush*	d2NewSolidBrush( D2D1::ColorF cr );
	IDWriteTextFormat*		d2NewFont( LPCWSTR strFontFamily, int nFontSize, int nFontWeight = 400 );

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

public:
	ID2D1HwndRenderTarget *		_render_target;
};

