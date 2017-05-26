#include "StdAfx.h"
#include "CommListElementUI.h"

CCommListElementUI::CCommListElementUI(void)
	: _bMultiColumn( false )
	, _nRowHeight( 40 )
{
}

CCommListElementUI::~CCommListElementUI(void)
{
}

SIZE CCommListElementUI::EstimateSize( SIZE szAvailable )
{
	return CDUISize( 0, _nRowHeight );
}


bool CCommListElementUI::initUI( CListUI *list, LPCWSTR strXML, bool bMultiCol, int nRowHeight )
{
	_bMultiColumn = bMultiCol;
	_nRowHeight = nRowHeight;

	CDialogBuilder builder;
	CPaintManagerUI* mgr = GetManager();
	CContainerUI* pRoot = (CContainerUI*)builder.Create( strXML, (UINT)0, NULL, mgr, NULL);
	if (pRoot && Add(pRoot) )
	{
		m_list = list;
		_pHL = FindCtrl< CHorizontalLayoutUI >( L"main" );

		Init();

		list->Add( this );
		return true;
	}

	return false;
}


CCommListElementUI* CCommListElementUI::addToList( CListUI *list, LPCWSTR strXML, bool bMultiCol, int nRowHeight )
{
	CCommListElementUI *pThis = new CCommListElementUI;
	if( pThis->initUI( list, strXML, bMultiCol, nRowHeight ) )
		return pThis;

	delete( pThis );
	return nullptr;
}

void CCommListElementUI::SetPos( RECT rc )
{
	CListContainerElementUI::SetPos(rc);

	if( !_bMultiColumn )
		return;

	CListHeaderUI *pHeader = m_list->GetHeader();
	if( m_pOwner == nullptr || pHeader == nullptr)
		return;

	int nCount = _pHL->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CControlUI *pCtrl = static_cast<CControlUI*>(_pHL->GetItemAt(i));
		CListHeaderItemUI *pHeaderItem = static_cast<CListHeaderItemUI*>(pHeader->GetItemAt(i));
		if (pCtrl != nullptr && pHeaderItem != nullptr)
		{
			RECT rtHeader = pHeaderItem->GetPos();
			RECT rt = pCtrl->GetPos();
			if( rt.left == rtHeader.left && rt.right == rtHeader.right )
				continue;

			rt.left = rtHeader.left+2;
			rt.right = rtHeader.right-2;
			pCtrl->SetPos(rt);
		}
	}
}