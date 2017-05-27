#pragma once
class CSwhMainLayout : public CDialogBase
{
public:
	CSwhMainLayout(void);
	virtual ~CSwhMainLayout(void);

	RDLIB_DEFAULT_METHOD( L"swhMain.xml", UI_CLASSSTYLE_FRAME, L"testui_maindlg_classname" );

	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();


	CVerticalLayoutUI	*m_pVLHandle;
};

