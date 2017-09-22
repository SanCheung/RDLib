#include "StdAfx.h"
#include "LayeredDialogBase.h"
#include "LayeredWindowDrawer.h"

CLayeredDialogBase::CLayeredDialogBase(void)
{
}


CLayeredDialogBase::~CLayeredDialogBase(void)
{
}

void CLayeredDialogBase::Init()
{
	//layerDraw();
}

void CLayeredDialogBase::layerDraw()
{
	CDUIRect	rt;
	GetClientRect( m_hWnd, rt );

	CLayeredWindowDrawer2		lwd( m_hWnd, rt );
	CControlUI *pUIRoot = paint_manager_.GetRoot();
	pUIRoot->SetPos( rt );
	pUIRoot->DoPaint( lwd.getDrawDC(), rt );
}


LRESULT CLayeredDialogBase::OnNcHitTest( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = TRUE;
	return HTCLIENT;
}
