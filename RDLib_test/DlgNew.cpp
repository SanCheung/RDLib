#include "StdAfx.h"
#include "DlgNew.h"

#include "CategoryUI.h"
#include "GalleryUI.h"

CDlgNew::CDlgNew(void)
{
}


CDlgNew::~CDlgNew(void)
{
}

void CDlgNew::Init()
{
}

LRESULT CDlgNew::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	HWND hParent = GetParent( m_hWnd );
	if( hParent != NULL && ::IsWindow( hParent ) )
		EndModal( 0 );
	else
		PostQuitMessage( 0 );

	return 0;
}

CControlUI* CDlgNew::CreateControl( LPCTSTR pstrClass )
{
	if( EQUSTR( pstrClass, L"Category" ) )
		return new CCategoryUI;

	else if( EQUSTR( pstrClass, L"Gallery" ) )
		return new CGalleryUI;

	return NULL;
}

void CDlgNew::Notify( TNotifyUI& msg )
{
	CStringW	strSenderName = msg.pSender->GetName();
	CStringW	strType = msg.sType;

	if( strType == L"click")
	{
		if( strSenderName == L"bn1" )
		{
			Data2UI();
		}
	}
	else if( strType == L"itemClick" )
	{
		CStringW	str;
		str.Format( L"%d", msg.wParam );
		MsgBox( str );
	}
}

void CDlgNew::Data2UI()
{
}
