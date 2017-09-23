#pragma once


class C01WndScanEnter;
class C02GamePadDlg;
class C04ConfirmDlg;
class C05NetErrorDlg;
class C06AdminLoginDlg;
class C07PayInfoDlg;
class C08NetInterruptDlg;

class XTask;
class CMainDlg  : public CDialogBase
				, xdmp::ThreadWrap
{
public:
	CMainDlg(void);
	virtual ~CMainDlg(void);

	RDLIB_DEFAULT_METHOD( L"dlgDemo.xml", UI_CLASSSTYLE_FRAME, L"mgc_windowclass" )

	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	void	Init();

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void	ShowInfo( CStringW strInfo );

	virtual void	thread_main();

	int		m_nCurrentPage;
	bool	OnChildEvent( void* paramters );

	
	void	fnTask1();	
	shared_ptr<XTask>	_task1;

	void	fnTask2();	
	shared_ptr<XTask>	_task2;

	map< int, CDialogBase* >	_mapDlgs;
	CDialogBase*	_pDlgCurrent;

	void		onDlgShowModal( int id );
	void		onDlgShow( int id );
};

