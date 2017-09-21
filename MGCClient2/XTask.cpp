#include "StdAfx.h"
#include "XTask.h"


XTask::XTask(void)
{
}


XTask::~XTask(void)
{
}

void XTask::startTask( FnTask t )
{
	if( thIsRunning() )
		return;

	_task = t;
	thStart();
}

void XTask::thread_main()
{
	if( _task )
		_task();

	thClose();
}
