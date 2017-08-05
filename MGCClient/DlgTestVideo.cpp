#include "StdAfx.h"
#include "DlgTestVideo.h"



void CDlgTestVideo::CallbackPlaying(void *data)
{		
	//CallbackPlayer(data, WM_USER_PLAYING);
}

void CDlgTestVideo::CallbackPosChanged(void *data)
{	
	//CallbackPlayer(data, WM_USER_POS_CHANGED);
}

void CDlgTestVideo::CallbackEndReached(void *data)
{
	////CallbackPlayer(data, WM_USER_END_REACHED);
	////m_player.Play();

	//CAVPlayer *pAVPlayer = (CAVPlayer *) data;

	//HWND	hWnd = ::GetParent( pAVPlayer->GetHWND() );
	//::PostMessage( hWnd, WM_APP+1, 0, 0 );
}


CDlgTestVideo::CDlgTestVideo(void)
{
	libvlc_inst = libvlc_new (0, NULL);
}


CDlgTestVideo::~CDlgTestVideo(void)
{
	libvlc_release (libvlc_inst);
}

void CDlgTestVideo::Init()
{
	//CMsoOcxUI *pUI = FindCtrl<CMsoOcxUI>( L"DSO" );
	//pUI->OpenDocument( L"E:\\123456.docx" );

	//设置播放器的窗口句柄和回调函数
	CMyOwnerWndUI *pWnd = static_cast<CMyOwnerWndUI*>(paint_manager_.FindControl(_T("wndMedia")));
	if(pWnd)
	{
		/* Create a new item */
		//m = libvlc_media_new_location (libvlc_inst, "http://mycool.movie.com/test.mov");

		libvlc_m = libvlc_media_new_path (libvlc_inst, "F:\\Clip_1080_5sec_VC1_15mbps.wmv" );

		/* Create a media player playing environement */
		libvlc_mp = libvlc_media_player_new_from_media (libvlc_m);

		/* No need to keep the media now */
		libvlc_media_release (libvlc_m);

		//on windows
		libvlc_media_player_set_hwnd (libvlc_mp, pWnd->GetHWND() );

		/* play the media_player */
		int x=libvlc_media_player_play (libvlc_mp);


		//m_player.SetHWND(pWnd->GetHWND()); 
		////设置回调
		//m_player.SetCallbackPlaying(CallbackPlaying);
		//m_player.SetCallbackPosChanged(CallbackPosChanged);
		//m_player.SetCallbackEndReached(CallbackEndReached);


		////m_player.Play( "f:\\Clip_1080_5sec_VC1_15mbps.wmv" );
		//m_player.Play( "http://192.168.0.66:11000/file/%E7%94%B5%E5%AD%90%E5%B1%8F%E6%B5%8B%E8%AF%95%E4%BC%9A%E8%AE%AE%E7%AC%AC%E4%BA%94%E6%AC%A1%E5%A4%A7%E4%BC%9A/%E8%A7%86%E9%A2%91Video/Clip_1080_5s_0801142647.wmv",
		//	false );
		////m_player.Play( "F:\\Clip_1080_5sec_VC1_15mbps.wmv", true );
	}
}

LRESULT CDlgTestVideo::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	EndModal( 0 );

	PostQuitMessage( 0 );
	return 0;
}

CControlUI* CDlgTestVideo::CreateControl( LPCTSTR pstrClass )
{
	if( EQUSTR( pstrClass, L"WndMediaDisplay") )
	{
		//自定义窗口
		CMyOwnerWndUI *pUI = new CMyOwnerWndUI;   
		HWND   hWnd = CreateWindow(_T("#32770"), _T("WndMediaDisplay"), WS_VISIBLE | WS_CHILD, \
			0, 0, 0, 0, paint_manager_.GetPaintWindow(), (HMENU)0, NULL, NULL);
		pUI->Attach(hWnd);
		return pUI;
	}

	return NULL;
}

void CDlgTestVideo::Notify( TNotifyUI& msg )
{
	CStringW	strSenderName = msg.pSender->GetName();
	CStringW	strType = msg.sType;

	if( strType == L"click")
	{
		if( strSenderName == L"bn1" )
		{
		}
	}
	else if( strType == L"timer" )
	{
	}
}

LRESULT CDlgTestVideo::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( WM_APP+1 == uMsg )
	{
		//m_player.Stop();
		//m_player.Play( "F:\\Clip_1080_5sec_VC1_15mbps.wmv", true );
		//return 0;
	}
	else if( WM_LBUTTONDOWN == uMsg )
	{
		int x = LOWORD( wParam );
		int y = HIWORD( wParam );
		MsgBox( L"hello!" );
	}
	
	return CDialogBase::HandleMessage(uMsg, wParam, lParam);
}
