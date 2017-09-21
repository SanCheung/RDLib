#pragma once

#include <functional>
using namespace std;

typedef tr1::function< void() >		FnTask;
#define TASKBINDFN( Fn )	bind( &Fn, this)

class XTask : public xdmp::ThreadWrap
{
public:
	XTask(void);
	virtual ~XTask(void);

	void	startTask( FnTask t );

protected:
	virtual void	thread_main();

	FnTask		_task;	
};

