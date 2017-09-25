#pragma once
#include "HScrollUI.h"

class CCategoryUI;
//class CGalleryUI;
class CGallery2UI;
class C02GamePadDlg : public CDialogBase
					, public IScrollHost
{
public:
	C02GamePadDlg(void);
	virtual ~C02GamePadDlg(void);

	RDLIB_DEFAULT_METHOD( L"02GamePadDlg.xml", UI_CLASSSTYLE_FRAME, L"02GamePad_windowclass" )

	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void	Data2UI();

	virtual void OnSetRatio( PointF pt );

	CCategoryUI		*m_category;
	//CGalleryUI	*m_gallery;
	CGallery2UI		*m_gallery2;
	CVScrollUI		*m_vs;

	// 0: Î´µÇÂ¼		1: ÒÑµÇÂ¼
	int			m_nLoginState;


	CButtonUI	*m_pBnLogin;
	bool		OnButtonLoginEvent( void* paramters );
};

