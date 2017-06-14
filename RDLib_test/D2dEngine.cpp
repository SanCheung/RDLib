#include "stdAfx.h"
#include "D2dEngine.h"
//#include "AppHelper.h"

#pragma comment(lib, "dwrite.lib")  
#pragma comment(lib, "d2d1.lib")

////////////////////////////////////////////////////////////////////////////////
D2dEngine::D2dEngine()
	: _factory( nullptr )
	, _write_factory( nullptr )
	, _image_factory( nullptr )

	, _render_target( nullptr )
	, _bFactoryInited( false )
{
	d2Startup();
}

D2dEngine::~D2dEngine()
{
	d2_safe_release< ID2D1HwndRenderTarget >( _render_target );

	d2_safe_release< IDWriteFactory >( _write_factory );
	d2_safe_release< IWICImagingFactory >( _image_factory );
	d2_safe_release< ID2D1Factory >( _factory );
}

bool D2dEngine::d2InitHRT( HWND hWnd )
{
	if( !_bFactoryInited )
		return false;

	if( nullptr != _render_target )
		return true;

	RECT rc;  
	GetClientRect( hWnd, &rc );

	D2D1_SIZE_U sz = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
	//GDI融合
	D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
	rtProps.usage =  D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;

	HRESULT hr = _factory->CreateHwndRenderTarget(  
		rtProps,  
		D2D1::HwndRenderTargetProperties( hWnd, sz ),  
		&_render_target);
	if( hr != S_OK )
		return false;

	return true;
}


void D2dEngine::d2Resize( int w, int h )
{
	if( nullptr == _render_target )
		return;

	_render_target->Resize( D2D1::SizeU(w,h) );
}


bool D2dEngine::d2Startup()
{
	if( _bFactoryInited )
		return true;

	HRESULT hr = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, &_factory );
	if( S_OK != hr || nullptr == _factory )
	{
		//xdTrace( L"初始化Direct2D失败，请检查是否安装DirectX" );
		return false;
	}

	hr = DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED,__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown **>(&_write_factory) );
	if( S_OK != hr || nullptr == _write_factory )
		return false;

	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL,CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), 
		(LPVOID*)&_image_factory);  
	if( S_OK != hr || nullptr == _image_factory )
		return false;

	_bFactoryInited = true;
	return true;
}

ID2D1Bitmap* D2dEngine::d2NewBitmap( LPCWSTR strFile )
{
	if( nullptr == _render_target )
		return nullptr;

	HRESULT		hr;
	ID2D1Bitmap	*bmp = nullptr;

	IWICBitmapDecoder *bitmapdecoder = nullptr;  
	hr = _image_factory->CreateDecoderFromFilename( strFile, NULL,GENERIC_READ, WICDecodeMetadataCacheOnDemand, &bitmapdecoder);
	if( S_OK == hr )
	{
		IWICBitmapFrameDecode  *pframe = nullptr;  
		hr = bitmapdecoder->GetFrame(0,&pframe);
		if( S_OK == hr )
		{
			IWICFormatConverter * fmtcovter = nullptr;  
			hr = _image_factory->CreateFormatConverter( &fmtcovter );
			if( S_OK == hr )
			{
				hr = fmtcovter->Initialize(pframe, GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone, 
					NULL, 0.0f,
					WICBitmapPaletteTypeCustom);
				if( S_OK == hr )
				{
					_render_target->CreateBitmapFromWicBitmap(fmtcovter, NULL, &bmp);
					fmtcovter->Release();
				}
			}
			pframe->Release(); 
		}
		bitmapdecoder->Release();
	}

	return bmp;
}

void D2dEngine::d2DrawBitmap( ID2D1Bitmap *bmp, float x, float y, float w, float h, float alpha )
{
	if( nullptr == _render_target || nullptr == bmp )
		return;

	D2D1_RECT_F rt = { x, y, x+w, y+h };    
	_render_target->DrawBitmap( bmp, rt, alpha ); 
}

ID2D1SolidColorBrush* D2dEngine::d2NewSolidBrush( D2D1::ColorF cr )
{
	if( nullptr == _render_target )
		return nullptr;

	ID2D1SolidColorBrush *br = nullptr;
	_render_target->CreateSolidColorBrush(D2D1::ColorF(cr), &br);
	return br;
}

IDWriteTextFormat* D2dEngine::d2NewFont( LPCWSTR strFontFamily, int nFontSize, int nFontWeight )
{
	if( nullptr == _render_target )
		return nullptr;

	IDWriteTextFormat*	tf;
	_write_factory->CreateTextFormat( strFontFamily, NULL, 
		(DWRITE_FONT_WEIGHT)nFontWeight,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		nFontSize, L"", &tf );

	return tf;
}

void D2dEngine::d2DrawText( LPCWSTR strText, float x, float y, float w, float h,
	IDWriteTextFormat* font, ID2D1SolidColorBrush* sb )
{
	if( nullptr == _render_target )
		return;

	_render_target->DrawText( strText, wcslen( strText ),  
		font, D2D1::RectF(x, y, x+w, y+h), sb );  
}

bool D2dEngine::d2Draw( IDrawer *drawer )
{
	if( nullptr == _render_target )  
		return false;

	_render_target->BeginDraw();
	//清除背景
	_render_target->Clear(D2D1::ColorF(D2D1::ColorF::Blue));  
	drawer->DoDraw();
	_render_target->EndDraw();
	return true;
}

void D2dEngine::d2GetSize( float &w, float &h )
{
	if( nullptr == _render_target )  
		return;

	D2D1_SIZE_F size =_render_target->GetSize();
	w = size.width;
	h = size.height;
}