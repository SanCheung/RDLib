#include "StdAfx.h"
#include "DlgNetTest.h"

// IsNetworkAlive
#include <Sensapi.h>
#pragma comment( lib, "Sensapi.lib" )


CDlgNetTest::CDlgNetTest(void)
{
}


CDlgNetTest::~CDlgNetTest(void)
{
}

void CDlgNetTest::Init()
{
	CControlUI *pIcon = FindCtrl( L"icon" );
	pIcon->GetManager()->SetTimer( pIcon, 1, 500 );
}

//LRESULT CDlgNetTest::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
//{
//	HWND hParent = GetParent( m_hWnd );
//	if( hParent != NULL && ::IsWindow( hParent ) )
//		EndModal( 0 );
//	else
//		PostQuitMessage( 0 );
//
//	return 0;
//}

CControlUI* CDlgNetTest::CreateControl( LPCTSTR pstrClass )
{
	return NULL;
}

void CDlgNetTest::Notify( TNotifyUI& msg )
{
	CStringW	strSenderName = msg.pSender->GetName();
	CStringW	strType = msg.sType;

	if( strType == L"click")
	{
		if( strSenderName == L"bn" )
		{
			//BOOL bHandled;
			//OnClose( 0, 0, 0, bHandled );
			EndModal( 2 );
		}
	}
	else if( strType == L"timer" )
	{
		CControlUI *pIcon = FindCtrl( L"icon" );
		pIcon->SetBkImage( _tcslen( pIcon->GetBkImage() )>0 ? L"": L"net/net.png" );

		if( NetOK() )
		{
			pIcon->GetManager()->KillTimer( pIcon );
			EndModal( 1 );
		}
	}
}

int CDlgNetTest::PopNetTestDlg()
{
	if( NetOK() )
		return 0;

	CDlgNetTest	dlg;
	return dlg.DoModalNoCaption( NULL );
}

bool CDlgNetTest::NetOK()
{
	DWORD	dwFlags = 0;
	if( IsNetworkAlive( &dwFlags ) )
		return true;

	return false;
}
