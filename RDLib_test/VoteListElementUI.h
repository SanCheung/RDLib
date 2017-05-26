#pragma once
class CVoteListElementUI : public CListContainerElementUI
{
public:
	CVoteListElementUI(void);
	virtual ~CVoteListElementUI(void);

	static CVoteListElementUI* create( LPCWSTR strXML, CListUI *list );
	virtual SIZE EstimateSize(SIZE szAvailable);

	template< class T >
	T* FindCtrl( wchar_t* name )
	{
		CControlUI* pRoot = GetItemAt( 0 );
		return (T*)pRoot->FindControl(CPaintManagerUI::__FindControlFromName, name, UIFIND_ALL );
	}

	virtual void	Init();

	void setInfo( CStringW strTitle, CStringW strTime, CStringW strState );

protected:
	CListUI				*m_list;
	CHorizontalLayoutUI	*_pHL;
	CButtonUI			*_pBn;

	bool	initUI( LPCWSTR strXML, CListUI *list );
	void	SetPos(RECT rc);
	bool	OnEvent(void* pv);
	bool	OnButtonEvent(void* pv);

	void DoEvent(TEventUI& evt);
	bool IsHot() { return (m_uButtonState & UISTATE_HOT)!=0; }

};

