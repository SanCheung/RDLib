#include "StdAfx.h"
#include "DlgTestList.h"

//#include "TextUI2.h"
#include "ListHortElementUI.h"
#include "ServiceQueryELementUI.h"

#include "SQElementUI.h"
#include "VoteListElementUI.h"
#include "CommListElementUI.h"

//#include "SeatSettingDlg.h"
//#include "SeatArrangeDlg.h"

//#include "MainDlg.h"
//#include "ExitTPWindow.h"

CDlgTestList::CDlgTestList(void)
	//: _wndET( nullptr )
{
}


CDlgTestList::~CDlgTestList(void)
{
}

void CDlgTestList::Init()
{
	_list = FindCtrl<CListUI>( L"list" );
	_list2 = FindCtrl<CListUI>( L"list2" );
	_list3 = FindCtrl<CListUI>( L"list3" );
}

LRESULT CDlgTestList::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	HWND hParent = GetParent( m_hWnd );
	if( hParent != NULL && ::IsWindow( hParent ) )
		EndModal( 0 );
	else
		PostQuitMessage( 0 );

	return 0;
}

CControlUI* CDlgTestList::CreateControl( LPCTSTR pstrClass )
{
	//if( EQUSTR( pstrClass, L"WhiteBoardWnd" ) )
	//{
	//	m_pWB = CWhiteBoardWnd::instance( m_hWnd );
	//	return m_pWB->GetHostUI();
	//}
	//else if( EQUSTR(pstrClass, L"ComboEx") )	
	//{
	//	return new CComboUIEx;
	//}

	return NULL;
}

void CDlgTestList::Notify( TNotifyUI& msg )
{
	CStringW	strSenderName = msg.pSender->GetName();
	CStringW	strType = msg.sType;

	if( strType == L"click")
	{
		if( strSenderName == L"bn1" )
		{
			//CColorPickerUI *pCp = FindCtrl<CColorPickerUI>(L"cp");
			//pCp->SetChoiceColor( 0xff0000FF );
			Data2UI();
		}
		else if( strSenderName == L"bn2" )
		{
			////SHT::CSeatSettingDlg	dlg( 20, 1 );
			////dlg.DoModal( m_hWnd, L"CTableMain" );

			////CSeatSettingDlg::TestCase( m_hWnd, L"Test!!!" );
			//CMainDlg		dlg;
			//dlg.DoModal( m_hWnd, L"hello" );
		}
		else if( strSenderName == L"bn3" )
		{
			//SHT::CSeatSettingDlg	dlg( 20, 1 );
			//dlg.DoModal( m_hWnd, L"CTableMain" );

			//CSeatArrangeDlg::TestCase( m_hWnd, L"Test!!!" );
		}
		else if( strSenderName == L"bn4" )
		{
			////CDlgExitTP		dlg;
			////dlg.Show();
			//CExitTPWindow::Show( m_hWnd );
		}
		else if( strSenderName == L"bn5" )
		{
			//CExitTPWindow::Hide();
		}
		else if( strSenderName == L"bnClose" )
		{
			CVoteListElementUI *pItem = (CVoteListElementUI *)msg.pSender->GetParent()->GetParent();
			_list3->RemoveAt( pItem->GetIndex() );
		}
	}
	else if( strType == L"itemButtonClick" )
	{
		//CServiceQueryELementUI *pItem = (CServiceQueryELementUI *)msg.pSender;
		//CStringW	str = pItem->GetUserData();
		//int id = str.ToInt();
		//MsgBox( str );

		CSQElementUI *item = (CSQElementUI *)msg.pSender;
		
		CStringW	str;
		str.Format( L"itemButtonClick: %d", item->GetIndex() );
		MsgBox( str );
	}
	else if( L"textchanged" == strType )
	{
		CServiceQueryELementUI *pItem = (CServiceQueryELementUI *)msg.pSender->GetParent()->GetParent()->GetParent();

		CStringW str;
		str.Format( L"%d %s", pItem->GetIndex(), msg.pSender->GetText() );
		FindCtrl<CLabelUI>(L"info")->SetText( str );
	}	
	else if( strType == L"timer" )
	{
		static int sv = 0;
		static int si = 0;

		CServiceQueryELementUI *pItem = (CServiceQueryELementUI *)_list2->GetItemAt( si );
		pItem->SetProgressValue( sv );

		if( sv == 100 )
		{
			//pPr->GetManager()->KillTimer( pPr );
			si++;
			sv = 0;

			if( si >= _list2->GetCount() )
			{
				_list2->GetManager()->KillTimer( _list2 );
			}
		}

		sv++;
	}
}


CTextUI* CDlgTestList::AddText( CStringW str )
{
	CTextUI *p = new CTextUI;
	p->SetText( str );
	//p->SetFont(1);
	p->SetAttribute( L"vcenter", L"true" );
	p->SetTextPadding( CDUIRect(10,10,10,10) );
	return p;
}


void CDlgTestList::Data2UI()
{
	int i;
	for( i = 0; i < 20; i++ )
	{
		CListHortElementUI *pItem = new CListHortElementUI;

		pItem->SetBkColor( 0xFFffffff );

		CStringW	strIndex;
		strIndex.Format( L"%d", i+1 );

		CTextUI *p1 = AddText( strIndex );
		CTextUI *p2 = AddText( L"222222" );
		CTextUI *p3 = AddText( L"33333" );
		CTextUI *p4 = AddText( L"4444" );
		CTextUI *p5 = AddText( L"555" );

		pItem->Add( p1 );
		pItem->Add( p2 );
		pItem->Add( p3 );
		pItem->Add( p4 );
		pItem->Add( p5 );

		pItem->m_pHeader = _list->GetHeader();
		pItem->SetFixedHeight( 80 );
		_list->Add( pItem );
	}

	for( i = 0; i < 100; i++ )
	{
		//CServiceQueryELementUI *pItemUI = new CServiceQueryELementUI;

		//pItemUI->m_pHeader = _list2->GetHeader();
		//if( !pItemUI->InitItem() )
		//{
		//	DEL( pItemUI );
		//	continue;
		//}

		//CStringW	strIndex;
		//strIndex.Format( L"%d", i+1 );
		//pItemUI->SetInfo( strIndex, L"name", L"time", L"Content" );

		//pItemUI->SetUserData( strIndex );
		//_list2->Add( pItemUI );

		CSQElementUI::create( _list2 );
	}

	_list2->GetManager()->SetTimer( _list2, 1, 20 );


	for( i = 0; i < 10; i++ )
	{
		CVoteListElementUI *item = CVoteListElementUI::create( L"votelistitem.xml", _list3 );
		if( i == 0 )
		{
			CCommListElementUI *citem = CCommListElementUI::addToList( _list3, L"voteenditem.xml", false, 100 );
		}

		CStringW	strTitle, strTime, strState;
		strTitle.Format( L"测试投票%d", i+1 );
		strTime = L"2016-11-28";
		strState = L"进行中";
		item->setInfo( strTitle, strTime, strState );
	}

	
}
