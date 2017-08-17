#pragma once
class CDlgPayment : public CDialogBase
{
public:
	CDlgPayment(void);
	virtual ~CDlgPayment(void);
	RDLIB_DEFAULT_METHOD( L"dlgPayment.xml", UI_CLASSSTYLE_FRAME, L"mgc_windowclass" )

	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	int		m_nLeftSecond;
};

