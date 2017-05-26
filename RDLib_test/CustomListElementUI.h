#pragma once

class CCustomListElementUI : public CListContainerElementUI
{
public:
	CCustomListElementUI(void);
	virtual ~CCustomListElementUI(void);
	
	static CCustomListElementUI* create( LPCWSTR strXML, CListUI *list );
	virtual SIZE EstimateSize(SIZE szAvailable);

	template< class T >
	T* FindCtrl( wchar_t* name )
	{
		CControlUI* pRoot = GetItemAt( 0 );
		return (T*)pRoot->FindControl(CPaintManagerUI::__FindControlFromName, name, UIFIND_ALL );
	}

protected:
	CListUI				*m_list;
	CHorizontalLayoutUI	*_pHL;

	bool	initUI( LPCWSTR strXML, CListUI *list );
	void	SetPos(RECT rc);
};

