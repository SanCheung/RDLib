#include "StdAfx.h"
#include "SQElementUI.h"


CSQElementUI::CSQElementUI(void)
{
}


CSQElementUI::~CSQElementUI(void)
{
}

void CSQElementUI::Init()
{
	m_pTxtInfo = FindCtrl< CTextUI >( L"txtInfo" );
	m_pBn = FindCtrl< CButtonUI >( L"bn" );
	m_pPr = FindCtrl< CProgressUI >( L"pr" );

	m_pBn->OnEvent += MakeDelegate(this, &CSQElementUI::OnButtonEvent);
	m_pTxtInfo->OnEvent += MakeDelegate(this, &CSQElementUI::OnTextEvent);
}


bool CSQElementUI::OnButtonEvent( void* paramters )
{
	TEventUI* pEvent = (TEventUI*)paramters;	
	if ( pEvent->Type > UIEVENT__MOUSEBEGIN && pEvent->Type < UIEVENT__MOUSEEND )
	{
		m_pBn->DoEvent( *pEvent );
		//_pHL->DoEvent( *pEvent );
		if( pEvent->Type == UIEVENT_BUTTONUP )
		{
			//ATLTRACE( "%d\n", pEvent->dwTimestamp );
			GetManager()->SendNotify(this, L"itemButtonClick");
			return false;
		}
	}

	return false;
}

bool CSQElementUI::OnTextEvent( void* paramters )
{
	TEventUI* pEvent = (TEventUI*)paramters;	
	if ( pEvent->Type > UIEVENT__MOUSEBEGIN && pEvent->Type < UIEVENT__MOUSEEND )
	{
		_pHL->DoEvent( *pEvent );
	}

	return false;
}

CSQElementUI* CSQElementUI::create( CListUI *list )
{
	CSQElementUI *pThis = new CSQElementUI;
	if( pThis->initUI( L"ServiceQueryItem.xml", list ) )
		return pThis;

	delete( pThis );
	return nullptr;
}
