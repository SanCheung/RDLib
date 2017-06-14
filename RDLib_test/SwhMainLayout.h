#pragma once

class CBarCodeOcxUI;
class CIconPathViewD2UI;

class CSwhMainLayout : public CDialogBase
{
public:
	CSwhMainLayout(void);
	virtual ~CSwhMainLayout(void);

	RDLIB_DEFAULT_METHOD( L"swhMain.xml", UI_CLASSSTYLE_FRAME, L"testui_maindlg_classname" );

	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();


	CVerticalLayoutUI	*m_pVLHandle;
	CBarCodeOcxUI *	m_ocx;

	CIconPathViewD2UI	* m_pIPV;
};

