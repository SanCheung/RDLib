// rdServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TCPServer.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CTCPServer m_Server;
	if( !m_Server.Init( 19216 ) )
	{
		printf("服务器启动失败，请检查端口是否占用\n");
		return 0;
	}

	m_Server.Start();

	printf( "服务器运行中...\n" );
	while(1)
	{
		Sleep(1000);
	}


	m_Server.Stop();
	return 0;
}

