
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

#include "XdBaseLib.h"
#pragma comment( lib, "XdBase.lib" )

bool RunAsAdmin( LPCWSTR strExe, int nShow  )
{
	//CStringW strPath = CAppData::GetInstancePath();
	//strPath += L"tcw.exe";

	SHELLEXECUTEINFO	si = {0};
	si.cbSize = sizeof( SHELLEXECUTEINFO );
	si.fMask = SEE_MASK_INVOKEIDLIST;
	//si.lpVerb = L"runas";
	si.lpVerb = L"open";
	si.lpFile = strExe;
	si.nShow = nShow;

	return (TRUE==ShellExecuteEx( &si ));
}

int getProcessID( const wchar_t *name )
{
	HANDLE hSnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	PROCESSENTRY32 pe = {0};
	pe.dwSize = sizeof(PROCESSENTRY32);

	int procID = -1;

	while(Process32Next(hSnapShot, &pe))
	{
		// processInfo->th32ProcessID,
		// processInfo->szExeFile,
		// processInfo->cntThreads;

		if( !wcsicmp( pe.szExeFile, name ) )
		{
			procID = pe.th32ProcessID;
			break;
		}
	}

	CloseHandle(hSnapShot);
	return procID;
}

int APIENTRY _tWinMain( HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
#define		CLIENTEXE			L"MGCClient.exe"

	while(1)
	{
		int id= getProcessID( CLIENTEXE );
		if( id == -1 )
		{
			//if( IDYES == MessageBox( GetDesktopWindow(), L"“™÷ÿ∆Ù¡À£°", L"", MB_YESNO ) )
			//{
				KsSystemDown ksd;
				ksd.SuperReboot();
				break;
			//}
		}

		Sleep(5000);
	}
}