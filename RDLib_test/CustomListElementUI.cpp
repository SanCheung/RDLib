#include "StdAfx.h"
#include "CustomListElementUI.h"


CCustomListElementUI::CCustomListElementUI(void)
{
}


CCustomListElementUI::~CCustomListElementUI(void)
{
}

SIZE CCustomListElementUI::EstimateSize( SIZE szAvailable )
{
	return CDUISize( 0, 40 );
}


bool CCustomListElementUI::initUI( LPCWSTR strXML, CListUI *list )
{
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


CCustomListElementUI* CCustomListElementUI::create( LPCWSTR strXML, CListUI *list )
{
	CCustomListElementUI *pThis = new CCustomListElementUI;
	if( pThis->initUI( strXML, list ) )
		return pThis;

	delete( pThis );
	return nullptr;
}

void CCustomListElementUI::SetPos( RECT rc )
{
	CListContainerElementUI::SetPos(rc);

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
