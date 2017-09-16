#pragma once
#include "HScrollUI.h"

class CGalleryUI;
class CCategoryUI;
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
	CGalleryUI		*m_gallery;
	CVScrollUI		*m_vs;

	// 0: δ��¼		1: �ѵ�¼
	int			m_nLoginState;
};
