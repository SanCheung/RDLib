#pragma once

class CBarCodeOcxUI : public CHostableWnd 
{
public:
	CBarCodeOcxUI(void);
	virtual ~CBarCodeOcxUI(void);

	virtual UINT GetClassStyle() const			{ return UI_CLASSSTYLE_CHILD; }
	virtual LPCTSTR GetWindowClassName() const	{ return L"xdc_drawpad_classname"; }
	virtual LRESULT HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );

	static	CBarCodeOcxUI* CreateUI( HWND hWnd );

	void	SetValue( CStringW strValue );

protected:	
	COcxSite	*_pSite;

};

