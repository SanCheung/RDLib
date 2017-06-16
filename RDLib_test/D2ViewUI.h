#pragma once
#include "Animation.h"
#include "D2dEngine.h"

class CD2ViewUI : public CHostableWnd
				, public CAniJob
				, public IDrawer
{
public:
	CD2ViewUI(void);
	virtual ~CD2ViewUI(void);

	virtual UINT GetClassStyle() const			{ return UI_CLASSSTYLE_CHILD; }
	virtual LPCTSTR GetWindowClassName() const	{ return L"ui_d2View_classname"; }

	void	InitAllImage();
	void	SetIconSubFolder( CStringW strSubFolder ){ _strIconSubFolder = strSubFolder; }
	void	AddIconAndText( CStringW strImageFile, CStringW strText );
	void	ReloadBKimage();

	void	AniStart( bool bToRight, int nTime = 1 );
	void	AniProc();
	void	AniEnd();

	int			GetCurrentIndex();
	CStringW	GetTextByIndex( int i );

	int			GetObjectCount() const;

private:
	void	Draw( Graphics &g );
	void	DoDraw();

	virtual LRESULT HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
	void	DoPaint( HDC hDC );
	void	OnLButtonDown(UINT nFlags, POINT point);
	void	OnLButtonUp(UINT nFlags, POINT point);
	void	OnMouseMove(UINT nFlags, POINT point);
	void	OnDoubleClick(UINT nFlags, POINT point);

	bool	m_bMouseDown;

	CDUIPoint	_ptDown;

	vector<CStringW>	_asImageFile;
	vector<CStringW>	_asText;

	CStringW			_strIconSubFolder;
	map<CStringW, shared_ptr<Image> >	_mapImage;


	shared_ptr<Image>	_spBK;
	//shared_ptr<Image>	_spBg3[3];
	shared_ptr<Image>	_spIconBK;

	CStringW					m_strFileBK;
	CStringW					m_strFileIconBK;
	map<CStringW, CStringW >	_mapName2File;

	void	BuildAllBitmap();

	map<CStringW, d2Bitmap* >	_mapImage2;
	d2Bitmap*					_spBK2;
	d2Bitmap*					_spIconBK2;

	d2Font*						_d2Font;
	d2SolidBrush*				_d2sb;

	void	d2DrawItem( CStringW strName, CStringW strText, ImageState *s );



	// 0 ~ 4
	int			_nOffsetIcon;
	bool		_bToRight;

	// ÒÆ¶¯´ÎÊý
	int			_nTime;
	bool		_animing;

	//D2dWrap		_dw;
	D2dEngine		_de;

	//shared_ptr<Image>		_sp;
	ImageState			_iss[5];

	//int		_angle;
	//int 		_alpha;
	//CAniTask		_at;

	void	DrawBigPoint( Graphics *pGr, PointF pt, int size=8 );
	void	DrawImageByState( Graphics *pGr, shared_ptr<Image> sp, ImageState *s );
	void	DrawTextByState( Graphics *pGr, CStringW strText, ImageState *s );

	//bool	DoDraw();
	//void	DrawUseD2d();
	//void	DrawItemByState( CStringW strFileName, CStringW strText, ImageState *s );

	void	InitImageStateArray();

	// 0~4 icon
	// -1  blank
	int		HitTest(  int x, int y );

	virtual void onPan( int dx, int dy );
	virtual void onGestureEnd();
	bool	_bPanning;


	bool	_bClickItem;


public:
	static	CD2ViewUI* CreateUI( HWND hWnd );
};

