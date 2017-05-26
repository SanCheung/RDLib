#include "StdAfx.h"
#include "ServiceQueryELementUI.h"
//#include <time.h>


CServiceQueryELementUI::CServiceQueryELementUI(void)
{
	m_pTxtInfo = nullptr;
	m_pBn = nullptr;

	srand( (unsigned)time( NULL ) );

	m_pHeader = nullptr;
	m_pPr = nullptr;
}


CServiceQueryELementUI::~CServiceQueryELementUI(void)
{
}

bool CServiceQueryELementUI::InitItem()
{
	CDialogBuilder builder;
	CPaintManagerUI* pPaintManager =  GetManager();
	CContainerUI* pRoot = (CContainerUI*)builder.Create(L"ServiceQueryItem.xml", (UINT)0, NULL, pPaintManager, NULL);
	if (!pRoot)
		return false;

	if( !Add(pRoot) )
		return false;

	m_pHL = (CHorizontalLayoutUI *)pRoot->FindControl(CPaintManagerUI::__FindControlFromName, L"main", UIFIND_ALL);
	m_pTxtInfo = (CTextUI*)pRoot->FindControl(CPaintManagerUI::__FindControlFromName, L"txtInfo", UIFIND_ALL);
	m_pBn = (CButtonUI*)pRoot->FindControl(CPaintManagerUI::__FindControlFromName, L"bn", UIFIND_ALL);
	m_pPr = (CProgressUI*)pRoot->FindControl(CPaintManagerUI::__FindControlFromName, L"pr", UIFIND_ALL);

	m_pBn->OnEvent += MakeDelegate(this, &CServiceQueryELementUI::OnButtonEvent);
	m_pTxtInfo->OnEvent += MakeDelegate(this, &CServiceQueryELementUI::OnTextEvent);

	return true;
}

SIZE CServiceQueryELementUI::EstimateSize( SIZE szAvailable )
{
	//int nHeight = 30 * (rand()%10);
	//return CDUISize( 0, nHeight );
	return CDUISize( 0, 50 );
}


bool CServiceQueryELementUI::OnButtonEvent( void* paramters )
{
	TEventUI* pEvent = (TEventUI*)paramters;	
	if ( pEvent->Type > UIEVENT__MOUSEBEGIN && pEvent->Type < UIEVENT__MOUSEEND )
	{
		m_pBn->DoEvent( *pEvent );
		m_pHL->DoEvent( *pEvent );
		if( pEvent->Type == UIEVENT_BUTTONUP )
		{
			GetManager()->SendNotify(this, L"itemButtonClick");
			return true;
		}
	}

	return false;
}

bool CServiceQueryELementUI::OnTextEvent( void* paramters )
{
	TEventUI* pEvent = (TEventUI*)paramters;	
	if ( pEvent->Type > UIEVENT__MOUSEBEGIN && pEvent->Type < UIEVENT__MOUSEEND )
	{
		m_pHL->DoEvent( *pEvent );
	}

	return false;
}

void CServiceQueryELementUI::SetInfo( LPCWSTR strSeatNo, LPCWSTR strName, LPCWSTR strTime, LPCWSTR strContent )
{
	CStringW		str;
	str.Format(L" {f 0} {c #ff0000}%s{/c}{/f}号座 {f 0}{c #ff0000}%s{/c}{/f} 在 %s 呼叫了 {f 0}{c #ff0000}%s{/c}{/f} 服务！",
		strSeatNo, strName, strTime, strContent );
	m_pTxtInfo->SetText( str );
}

void CServiceQueryELementUI::SetPos( RECT rc )
{
	CListContainerElementUI::SetPos(rc);

	if( m_pOwner == nullptr ) 
		return;		

	if (m_pHeader == nullptr)
		return;

//	int nCount = m_items.GetSize();
	int nCount = m_pHL->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		//CControlUI *pCtrl = static_cast<CControlUI*>(m_items[i]);
		CControlUI *pCtrl = static_cast<CControlUI*>(m_pHL->GetItemAt(i));
		CListHeaderItemUI *pHeaderItem = static_cast<CListHeaderItemUI*>(m_pHeader->GetItemAt(i));
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

void CServiceQueryELementUI::SetProgressValue( int v )
{
	if( m_pPr == nullptr )
		return;

	m_pPr->SetValue( v );

	CStringW	str;
	str.Format( L"%d%%", v );
	m_pPr->SetText( str );
}
