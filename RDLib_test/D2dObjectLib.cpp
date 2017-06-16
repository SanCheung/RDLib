#include "StdAfx.h"
#include "D2dObjectLib.h"


D2dObjectLib::D2dObjectLib(void)
{
}


D2dObjectLib::~D2dObjectLib(void)
{
}

ID2D1Bitmap* D2dObjectLib::dl_getImage( LPCWSTR strFileName )
{
	auto it = _imageLib.find( strFileName );
	if( it != _imageLib.end() )
		return it->second;

	ID2D1Bitmap *bmp = d2NewBitmap( strFileName );
	_imageLib.insert( make_pair( strFileName, bmp ) );
	return bmp;
}

IDWriteTextFormat* D2dObjectLib::dl_getFont( LPCWSTR strFamily, int nSize, int nWeight /*= 400 */ )
{
	IDWriteTextFormat *pWTF = nullptr;
	for( auto it = _fontLib.begin(); it != _fontLib.end(); ++it )
	{
		pWTF = *it;

		WCHAR	familyName[512] = {0};
		pWTF->GetFontFamilyName( familyName, 512 );
		
		int		nFontSize = int( pWTF->GetFontSize() + 0.5f );
		int		nFontWeight = (int)pWTF->GetFontWeight();

		if( _wcsicmp( familyName, strFamily ) == 0 && 
			nSize == nFontSize && nWeight == nFontWeight )
		{
			return pWTF;
		}
	}

	pWTF = d2NewFont( strFamily, nSize, nWeight );
	_fontLib.push_back( pWTF );
	return pWTF;
}

ID2D1SolidColorBrush* D2dObjectLib::dl_getSolidBrush( D2D1_COLOR_F cr )
{
	ID2D1SolidColorBrush *pBrush = nullptr;
	for( auto it = _solidBrushLib.begin(); it != _solidBrushLib.end(); ++it )
	{
		pBrush = *it;
		D2D1_COLOR_F crItem = pBrush->GetColor();
		if( crItem.a == cr.a && crItem.r == cr.r && 
			crItem.g == cr.g && crItem.b == cr.b )
			return pBrush;
	}

	pBrush = d2NewSolidBrush( D2D1::ColorF( cr.r, cr.g, cr.b, cr.a ) );
	_solidBrushLib.push_back( pBrush );
	return pBrush;
}