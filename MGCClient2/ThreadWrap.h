#pragma once

namespace xdmp
{


class ThreadWrap
{
public:
	ThreadWrap(void);
	virtual ~ThreadWrap(void);

	void	thStart();
	void	thShutdownUseEvent();

	int		thWait( int nMs = 3000 );
	int		thWaitEvent( int nMs = 3000 );
	
	bool	thIsRunning();

protected:
	virtual void	thread_main();

	void	thClose();
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

