#include "StdAfx.h"
#include "VoteListElementUI.h"


CVoteListElementUI::CVoteListElementUI(void)
{
}


CVoteListElementUI::~CVoteListElementUI(void)
{
}

SIZE CVoteListElementUI::EstimateSize( SIZE szAvailable )
{
	return CDUISize( 0, 60 );
}


bool CVoteListElementUI::initUI( LPCWSTR strXML, CListUI *list )
{
	CDialogBuilder builder;
	CPaintManagerUI* mgr = GetManager();
	CContainerUI* pRoot = (CContainerUI*)builder.Create( strXML, (UINT)0, NULL, mgr, NULL);
	if (pRoot && Add(pRoot) )
	{
		m_list = list;
		Init();

		list->Add( this );
		return true;
	}

	return false;
}


CVoteListElementUI* CVoteListElementUI::create( LPCWSTR strXML, CListUI *list )
{
	CVoteListElementUI *pThis = new CVoteListElementUI;
	if( pThis->initUI( strXML, list ) )
		return pThis;

	delete( pThis );
	return nullptr;
}

bool CVoteListElementUI::OnEvent( void* pv )
{
	TEventUI* pEvent = (TEventUI*)pv;	
	if ( pEvent->Type > UIEVENT__MOUSEBEGIN && pEvent->Type < UIEVENT__MOUSEEND )
	{
		_pHL->DoEvent( *pEvent );
	}

	return false;
}

bool CVoteListElementUI::OnButtonEvent( void* pv )
{
	TEventUI* pEvent = (TEventUI*)pv;	
	if ( pEvent->Type > UIEVENT__MOUSEBEGIN && pEvent->Type < UIEVENT__MOUSEEND )
	{
		_pHL->DoEvent( *pEvent );
		_pBn->DoEvent( *pEvent );
	}

	return false;
}

void CVoteListElementUI::Init()
{
	_pHL = FindCtrl< CHorizontalLayoutUI >( L"main" );

	CLabelUI *plb = FindCtrl<CLabelUI>( L"title" );
	plb->OnEvent += MakeDelegate(this, &CVoteListElementUI::OnEvent);

	plb = FindCtrl<CLabelUI>( L"time" );
	plb->OnEvent += MakeDelegate(this, &CVoteListElementUI::OnEvent);

	plb = FindCtrl<CLabelUI>( L"state" );
	plb->OnEvent += MakeDelegate(this, &CVoteListElementUI::OnEvent);

	_pBn = FindCtrl<CButtonUI>( L"bnClose" );
	_pBn->OnEvent += MakeDelegate(this, &CVoteListElementUI::OnButtonEvent);
}

void CVoteListElementUI::DoEvent( TEventUI& evt )
{
	CListContainerElementUI::DoEvent( evt );

	_pBn->SetVisible( IsHot() );
}

void CVoteListElementUI::SetPos( RECT rc )
{
	CListContainerElementUI::SetPos(rc);

	rc.right -= 3;
	rc.top += 3;
	rc.left = rc.right-11;
	rc.bottom = rc.top+11;
	_pBn->SetPos( rc );
}

void CVoteListElementUI::setInfo( CStringW strTitle, CStringW strTime, CStringW strState )
{
	FindCtrl<CLabelUI>( L"title" )->SetText( strTitle );
	FindCtrl<CLabelUI>( L"time" )->SetText( strTime );
	FindCtrl<CLabelUI>( L"state" )->SetText( strState );
}


