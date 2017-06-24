#pragma once

#include "D2dEngine.h"

class D2dObjectLib : public D2dEngine
{
public:
	D2dObjectLib(void);
	virtual ~D2dObjectLib(void);

public:
	ID2D1Bitmap*			dl_getImage( LPCWSTR strFileName );
	IDWriteTextFormat*		dl_getFont( LPCWSTR strFamily, int nSize, int nWeight = 400 );
	ID2D1SolidColorBrush*	dl_getSolidBrush( D2D1_COLOR_F cr );

private:
	void	clearAll();

protected:
	map< wstring, ID2D1Bitmap* >	_imageLib;
	vector< IDWriteTextFormat*	>	_fontLib;
	vector< ID2D1SolidColorBrush* >	_solidBrushLib;
};

