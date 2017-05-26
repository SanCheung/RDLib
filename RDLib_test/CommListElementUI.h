#pragma once

class CCommListElementUI : public CListContainerElementUI
{
public:
	CCommListElementUI(void);
	virtual ~CCommListElementUI(void);

	static CCommListElementUI* addToList( CListUI *list, LPCWSTR strXML, bool bMultiCol = false, int nRowHeight = 40 );
	
	template< class T >
	T* FindCtrl( wchar_t* name )
	{
		CControlUI* pRoot = GetItemAt( 0 );
		return (T*)pRoot->FindControl(CPaintManagerUI::__FindControlFromName, name, UIFIND_ALL );
	}

protected:
	CListUI				*m_list;
	CHorizontalLayoutUI	*_pHL;

	bool				_bMultiColumn;
	int					_nRowHeight;

	bool	initUI( CListUI *list, LPCWSTR strXML, bool bMultiCol, int nRowHeight );
	virtual void	SetPos(RECT rc);
	virtual SIZE	EstimateSize(SIZE szAvailable);
};