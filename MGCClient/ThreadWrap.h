#pragma once

namespace xdmp
{


class ThreadWrap
{
public:
	ThreadWrap(void);
	virtual ~ThreadWrap(void);

	void	Start();
	//void	Shutdown();
	void	ShutdownUseEvent();

	//void	SetExitFlag(){ _bExitFlag = true; }

	int		Wait( int nMs = 3000 );
	int		WaitEvent( int nMs = 3000 );
	
	bool	IsRunning();

protected:
	virtual void	thread_main();

	void	Close();
	//bool	GetExitFlag(){ return _bExitFlag; }

private:  // ½ûÖ¹¸´ÖÆ
	ThreadWrap (const ThreadWrap&) {}
	const ThreadWrap& operator= (const ThreadWrap&) {}

private:
	HANDLE			_hThread;
	bool			_bExitFlag;

	HANDLE			_hExitEvent;

private:
	static UINT WINAPI funProcess( LPVOID pVoid );
};


}

