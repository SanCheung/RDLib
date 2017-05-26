#include "StdAfx.h"
#include "PowerOff.h"

KsSystemDown::KsSystemDown()
{
	NtShutdownSystem=NULL;
	NtSetSystemPowerState=NULL;
	OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY|TOKEN_ADJUST_PRIVILEGES,&hToken);
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);
	CloseHandle(hToken);

	hNtdll = GetModuleHandleA("ntdll.dll");
	if (hNtdll)
	{
		NtShutdownSystem = (lpNtShutdownSystem)GetProcAddress(hNtdll,"NtShutdownSystem");
		NtSetSystemPowerState = (lpNtSetSystemPowerState)GetProcAddress(hNtdll,"NtSetSystemPowerState");
	}
}

KsSystemDown::~KsSystemDown()
{
	tkp.Privileges[0].Attributes = 0;
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);
}