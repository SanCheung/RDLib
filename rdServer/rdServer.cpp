// rdServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TCPServer.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CTCPServer m_Server;
	if( !m_Server.Init( 19216 ) )
	{
		printf("����������ʧ�ܣ�����˿��Ƿ�ռ��\n");
		return 0;
	}

	m_Server.Start();

	printf( "������������...\n" );
	while(1)
	{
		Sleep(1000);
	}


	m_Server.Stop();
	return 0;
}

