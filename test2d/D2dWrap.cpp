#include "D2dWrap.h"

#include "stb_image.h"
#pragma comment(lib, "stimage_static.lib")


D2dWrap::D2dWrap(void)
{
	g_factory = nullptr;
	g_render_target = nullptr;

	g_brush = nullptr;
	g_write_factory = nullptr;
	g_text_format = nullptr;

	//g_gradient_stop_collection  = nullptr;
	//g_linear_gradient_brush  = nullptr;
	//g_radial_gradient_brush  = nullptr;

	g_image_factory = nullptr;
	g_bitmap = nullptr;


	_index = 0;
	_count = 100;

	xFrom = 0.f;
	xTo = 500.f;
	
	xStep = (xTo-xFrom)/_count;
}


D2dWrap::~D2dWrap(void)
{
	g_bitmap->Release(); 
	g_image_factory->Release();  

	//g_linear_gradient_brush->Release();  
	//g_radial_gradient_brush->Release();  
	//g_gradient_stop_collection->Release();

	g_text_format->Release();  
	g_write_factory->Release();  
	g_brush->Release();  
	g_render_target->Release();  
	g_factory->Release();  
}

bool D2dWrap::InitD2d( HWND hWnd )
{
	D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_factory );

	RECT rc;  
	GetClientRect( hWnd, &rc );  

	g_factory->CreateHwndRenderTarget(  
		D2D1::RenderTargetProperties(),  
		D2D1::HwndRenderTargetProperties( hWnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)    ),  
		&g_render_target);  


	g_render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::WhiteSmoke), &g_brush);  

	// Init Font  
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(g_write_factory),reinterpret_cast<IUnknown **>(&g_write_factory));  
	g_write_factory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL,  
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 40, L"",&g_text_format);  


	//// Create Gradient Stops  
	//D2D1_GRADIENT_STOP gradient_stops[3];  
	//gradient_stops[0].color = D2D1::ColorF(D2D1::ColorF::LightGreen);  
	//gradient_stops[0].position = 0.0f;  
	//gradient_stops[1].color = D2D1::ColorF(D2D1::ColorF::DarkGreen);  
	//gradient_stops[1].position = 0.5f;  
	//gradient_stops[2].color = D2D1::ColorF(D2D1::ColorF::Green);  
	//gradient_stops[2].position = 1.0f;  

	//// Create Interface  
	//g_render_target->CreateGradientStopCollection(gradient_stops, 3, &g_gradient_stop_collection);  

	//// Create Linear Gradient Brush  
	//g_render_target->CreateLinearGradientBrush(  
	//	D2D1::LinearGradientBrushProperties(D2D1::Point2F(0,0), D2D1::Point2F(600,600)),   
	//	g_gradient_stop_collection, &g_linear_gradient_brush);  

	//g_render_target->CreateRadialGradientBrush(  
	//	D2D1::RadialGradientBrushProperties(D2D1::Point2F(500,500),D2D1::Point2F(),1000.0f,1000.0f),  
	//	g_gradient_stop_collection,&g_radial_gradient_brush);  

	// Initialize Image Factory  
	CoCreateInstance(CLSID_WICImagingFactory,NULL,CLSCTX_INPROC_SERVER,__uuidof(g_image_factory),(LPVOID*)&g_image_factory);  

	IWICBitmapDecoder *bitmapdecoder = NULL;  
	g_image_factory->CreateDecoderFromFilename(L"f:\\c.jpg",NULL,GENERIC_READ,WICDecodeMetadataCacheOnDemand,&bitmapdecoder);//  

	IWICBitmapFrameDecode  *pframe = NULL;  
	bitmapdecoder->GetFrame(0,&pframe);  

	IWICFormatConverter * fmtcovter = NULL;  
	g_image_factory->CreateFormatConverter(&fmtcovter);  
	fmtcovter->Initialize(pframe,GUID_WICPixelFormat32bppPBGRA,WICBitmapDitherTypeNone,NULL,0.0f,WICBitmapPaletteTypeCustom);  
	g_render_target->CreateBitmapFromWicBitmap(fmtcovter, NULL, &g_bitmap);

	fmtcovter->Release();  
	pframe->Release();  
	bitmapdecoder->Release();


	//HGLOBAL m_hGlobal = GlobalAlloc( GMEM_MOVEABLE, 1000000 );
	//LPBYTE	m_lpBuf = (LPBYTE)GlobalLock( m_hGlobal );

	//Bitmap	bmp( L"f:\\其它应用.png" );
	//int w = bmp.GetWidth();
	//int h = bmp.GetHeight();
	//BitmapToBuffer( m_lpBuf, &bmp, w, h );

	//D2D1_SIZE_U		d2size = {w,h};
	//D2D1_BITMAP_PROPERTIES		dbp;
	//dbp.dpiX = 100;
	//dbp.dpiY = 100;
	//dbp.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
	////dbp.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);

	//g_render_target->CreateBitmap( d2size, m_lpBuf, w*4, dbp, &g_bitmap2 );

	//GlobalUnlock( m_hGlobal );
	//GlobalFree( m_hGlobal );

	return true;
}

void D2dWrap::resize( int w, int h )
{
	if(g_render_target)  
		g_render_target->Resize(D2D1::SizeU(w,h));
}

bool D2dWrap::draw()
{
	if(!g_render_target)  
		return false;

	if( _index  == _count )
		return false;

	//_index = (_index+1)%_count;
	_index++;
	g_render_target->BeginDraw();  

	// Clear Background  
	g_render_target->Clear(D2D1::ColorF(0.63, 0.84, 0.00));   

	//// Draw Ellipse    
	D2D1_SIZE_F size = g_render_target->GetSize();    
	//D2D1_RECT_F r = {0, 0, size.width, size.height};  
	//g_render_target->FillRectangle(&r, g_linear_gradient_brush);   
	////g_render_target->FillRectangle(&ellipse, g_linear_gradient_brush);

	int  x = xFrom + xStep*_index;

	// Draw Image  
	D2D1_RECT_F imgr = {x, 0, x+size.width/2, size.height/2};  
	//g_render_target->DrawBitmap(g_bitmap, imgr, rand()*1.f/RAND_MAX);  
	g_render_target->DrawBitmap(g_bitmap, imgr); 

	//D2D1_RECT_F imgr2 = {100, 100, 200, 200};  
	////g_render_target->DrawBitmap(g_bitmap, imgr, rand()*1.f/RAND_MAX);  
	//g_render_target->DrawBitmap(g_bitmap2, imgr2);

	// Draw Text   
	const wchar_t * text = L"Direct2D Draw Image";  
	g_render_target->DrawText(text, wcslen(text),  
		g_text_format,  
		D2D1::RectF(100, 190, size.width, size.height),  
		g_brush);  

	g_render_target->EndDraw();
	return true;
}

void D2dWrap::reset()
{
	_index = 0;
}




////////////////////////////////////////////////////////////////////////////////
#define			SAFERELEASE( p )		if( nullptr != p ){ p->Release(); p = nullptr; }


D2dEngine::D2dEngine()
	: _factory( nullptr )
	, _write_factory( nullptr )
	, _image_factory( nullptr )

	, _render_target( nullptr )
	, _bFactoryInited( false )
{
	d2Startup();

	imgdata = nullptr;
	m_pBitmap = nullptr;
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

	RECT rc;  
	GetClientRect( hWnd, &rc );

	D2D1_SIZE_U sz = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	_factory->CreateHwndRenderTarget(  
		D2D1::RenderTargetProperties(),  
		D2D1::HwndRenderTargetProperties( hWnd, sz ),  
		&_render_target);


	// Test;
	LPBYTE pData = NULL;
	DWORD dwSize = 0;
	{
		HANDLE hFile = ::CreateFile( L"F:\\tcClient.png", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		//HANDLE hFile = ::CreateFile( L"F:\\123.bmp", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			
		if( hFile == INVALID_HANDLE_VALUE ) return NULL;
		dwSize = ::GetFileSize(hFile, NULL);
		if( dwSize == 0 ) return NULL;

		DWORD dwRead = 0;
		pData = new BYTE[ dwSize ];
		::ReadFile( hFile, pData, dwSize, &dwRead, NULL );
		::CloseHandle( hFile );
	}
	////////////


	LPBYTE pImage = NULL;
	int n;
	pImage = stbi_load_from_memory( pData, dwSize, &_w, &_h, &n, 4 );




	int imgwidth = _w;
	int imgheight= _h;

	//创建位图  
	D2D1_SIZE_U imgsize= D2D1::SizeU(imgwidth, imgheight);  
	D2D1_PIXEL_FORMAT pixelFormat =  //位图像素格式描述  
	{  
		DXGI_FORMAT_B8G8R8A8_UNORM, //该参数设置图像数据区的像素格式，现为RGBA，可根据需要改为别的格式，只是后面的数据拷贝要做相应的调整  
		//D2D1_ALPHA_MODE_IGNORE 
		D2D1_ALPHA_MODE_PREMULTIPLIED
	};  

	D2D1_BITMAP_PROPERTIES prop =  //位图具体信息描述  
	{  
		pixelFormat,  
		imgsize.width,  
		imgsize.height  
	}; 

	long pitch = imgsize.width*4;  
	imgdata = new char[imgsize.width * imgsize.height * 4];  
	_render_target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);//设置图像为抗锯齿模式  
	HRESULT hr =_render_target->CreateBitmap(imgsize, imgdata, pitch, &prop, &m_pBitmap);

	memset( imgdata, 0, _w*_h*4 );
	//memcpy( imgdata, pImage, _w*_h*4 );

	for (size_t row = 0; row < _h; row++)  
		for (size_t col = 0; col < _w; col++)  
		{  
			int i = col + row*_w;

			if( pImage[i*4 + 3] == 0 )
			{
				imgdata[i*4 + 0] = 0;
				imgdata[i*4 + 1] = 0;
				imgdata[i*4 + 2] = 0;
				imgdata[i*4 + 3] = 0;
			}
			else
			{
				imgdata[i*4 + 0] = pImage[i*4 + 2];		//B
				imgdata[i*4 + 1] = pImage[i*4 + 1];		//G
				imgdata[i*4 + 2] = pImage[i*4 + 0];		//R
				imgdata[i*4 + 3] = pImage[i*4 + 3];
			}

		}

	stbi_image_free(pImage);
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
		return false;

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
	_render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::WhiteSmoke), &br);
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

void D2dEngine::TestGDIPlus()
{
	//ID2D1Bitmap *bmp2 = d2NewBitmap( L"f:\\123.bmp");
	//d2DrawBitmap( bmp2, 400, 300, 100, 100, 0.5f );

	//d2_safe_release( bmp2 );

	D2D1_RECT_U imgrect;
	imgrect.left = 0;  
	imgrect.right = _w;  
	imgrect.top = 0;  
	imgrect.bottom = _h;  


	m_pBitmap->CopyFromMemory(&imgrect, imgdata, _w * 4);  
	_render_target->DrawBitmap(m_pBitmap, D2D1::RectF(150, 150, 150+_w, 150+_h) );

	D2D1_SIZE_F	sz = {200,200};
	ID2D1BitmapRenderTarget * spBRT;
	_render_target->CreateCompatibleRenderTarget( sz, &spBRT );

	spBRT->BeginDraw();
	spBRT->Clear( D2D1::ColorF(0.63, 0.84, 0.00, 0.3f) );

	ID2D1SolidColorBrush *br = nullptr;
	spBRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &br);

	D2D1_POINT_2F	p1 = {100.f, 100.f};
	D2D1_POINT_2F	p2 = {160.f, 140.f};
	spBRT->DrawLine( p1, p2, br );
	spBRT->EndDraw();

	ID2D1Bitmap	*pBmp;
	spBRT->GetBitmap( &pBmp );

	_render_target->DrawBitmap( pBmp );

}

//void D2dEngine::TestLayer()
//{
//	HRESULT		hr;
//	ID2D1HwndRenderTarget *pRT = _render_target;
//
//	// Create a layer.
//	ID2D1Layer *pLayer = NULL;
//	hr = pRT->CreateLayer(NULL, &pLayer);
//
//	if (SUCCEEDED(hr))
//	{
//		pRT->SetTransform(D2D1::Matrix3x2F::Translation(300, 250));
//
//		// Push the layer with the content bounds.
//		pRT->PushLayer(
//			D2D1::LayerParameters(
//			D2D1::InfiniteRect(),
//			NULL,
//			D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
//			D2D1::IdentityMatrix(),
//			1.0,
//			m_pRadialGradientBrush,
//			D2D1_LAYER_OPTIONS_NONE),
//			pLayer
//			);
//
//		//pRT->DrawBitmap(m_pBambooBitmap, D2D1::RectF(0, 0, 190, 127));
//
//		pRT->FillRectangle(
//			D2D1::RectF(25.f, 25.f, 50.f, 50.f), 
//			m_pSolidColorBrush
//			);
//		pRT->FillRectangle(
//			D2D1::RectF(50.f, 50.f, 75.f, 75.f),
//			m_pSolidColorBrush
//			); 
//		pRT->FillRectangle(
//			D2D1::RectF(75.f, 75.f, 100.f, 100.f),
//			m_pSolidColorBrush
//			);    
//
//		pRT->PopLayer();
//	}
//
//	d2_safe_release(&pLayer);
//}
