#pragma once

template< class T >
class TSingletonWindow : public CWindowWnd
{
public:
	TSingletonWindow(void)
		: _hMsg( nullptr )
	{
	}

	virtual ~TSingletonWindow(void)
	{
	}

protected:
	virtual LPCTSTR GetWindowClassName() const 
	{ 
		return L"mgc_windowclass";
	}

	virtual UINT	GetClassStyle() const 
	{ 
		return CS_HREDRAW | CS_VREDRAW | CS_IME | CS_DBLCLKS;
	}

	//virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	//{
	//	return CWindowWnd::HandleMessage( uMsg, wParam, lParam );
	//}

protected:
	HWND		CreateThis( HWND hMsg )
	{
		//Create( hMsg, NULL, WS_POPUP, WS_EX_TOPMOST, 0, 0, 1, 1 );   
		Create( hMsg, NULL, WS_POPUP, 0, 0, 0, 1, 1 );   
		_hMsg = hMsg;

		::ShowWindow( m_hWnd, SW_SHOWMAXIMIZED);
		//UpdateWindow( m_hWnd );

		return m_hWnd;
	}

private:
	static	T*	s_instance;
	HWND		_hMsg;

public:
	static void		Show( HWND hMsg )
	{
		mgTrace( L"TSingletonWindow::Show" );

		if( s_instance == nullptr )
		{
			s_instance = new T;
			s_instance->CreateThis( hMsg );
		}
		else
		{
			::ShowWindow( s_instance->m_hWnd, SW_SHOW);
		}

		::SetTimer( s_instance->m_hWnd, 1, 1000, NULL ); 
	}
	
	static bool		Hide()
	{
		if( s_instance != nullptr && IsWindow( s_instance->GetHWND() ) )
		{
			::KillTimer( s_instance->GetHWND(), 1 );
			s_instance->ShowWindow( false );
			return true;
		}

		return false;
	}

	static void		Release()
	{
		DestroyWindow( s_instance->m_hWnd );
		delete s_instance;
		s_instance = nullptr;
	}

	static bool		IsShow()
	{
		if( s_instance == nullptr )
			return false;

		HWND	hWnd = s_instance->GetHWND();
		if( hWnd == nullptr || !::IsWindow(hWnd) )
			return false;

		return ::IsWindowVisible( hWnd );
	}
};


template <class T>
T*  TSingletonWindow<T>::s_instance = nullptr;

