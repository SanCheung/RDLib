#ifndef __H_SHUTDOWN_H__
#define __H_SHUTDOWN_H__

#pragma once


//操作类型
typedef enum _SHUTDOWN_ACTION
{
	ShutdownNoReboot,
	ShutdownReboot,
	ShutdownPowerOff
} SHUTDOWN_ACTION;

typedef DWORD (WINAPI* lpNtShutdownSystem)(SHUTDOWN_ACTION Action);

typedef DWORD (WINAPI* lpNtSetSystemPowerState)(IN POWER_ACTION SystemAction,
												IN SYSTEM_POWER_STATE MinSystemState,
												IN ULONG Flags);

//关闭计算机
class XDBASE_API KsSystemDown
{
protected:
	HMODULE hNtdll;
	TOKEN_PRIVILEGES tkp;
	HANDLE hToken;

	lpNtShutdownSystem NtShutdownSystem;
	lpNtSetSystemPowerState NtSetSystemPowerState;

public:
	inline BOOL SystemShutdown(SHUTDOWN_ACTION action)
	{
		if (!NtShutdownSystem)
			return FALSE;

		DWORD dwRet = NtShutdownSystem(action);
		if (dwRet == 0)
			return TRUE;
		else
			return FALSE;
	}

	inline BOOL SystemPowerdown(IN POWER_ACTION SystemAction,IN SYSTEM_POWER_STATE MinSystemState,IN ULONG Flags)
	{
		if (!NtSetSystemPowerState)
			return FALSE;

		DWORD dwRet = NtSetSystemPowerState(SystemAction,MinSystemState,Flags);
		if (dwRet == 0)
			return TRUE;
		else
			return FALSE;
	}

	inline BOOL CommonLogoff()
	{
		return Shutdown(EWX_LOGOFF);
	}

	inline BOOL CommonShutdown()
	{
		return Shutdown((EWX_POWEROFF|EWX_SHUTDOWN));
	}
	
	inline BOOL CommonReBoot()
	{
		return Shutdown(EWX_REBOOT);
	}

	inline  BOOL FastLogoff()
	{
		return Shutdown(EWX_LOGOFF|EWX_FORCE);
	}

	inline  BOOL FastShutdown()
	{
		return Shutdown(EWX_POWEROFF|EWX_SHUTDOWN|EWX_FORCE);
	}

	inline	BOOL FastReboot()
	{
		return Shutdown(EWX_REBOOT|EWX_FORCE);
	}

	inline BOOL Shutdown(DWORD dwValue)
	{
		return ExitWindowsEx(dwValue,0);
	}

	inline BOOL SuperShutdown()
	{
		if (NtShutdownSystem != NULL)
			return NtShutdownSystem(ShutdownNoReboot);
		else
			return FALSE;
	}

	inline BOOL SuperReboot()
	{
		if (NtShutdownSystem != NULL)
			return NtShutdownSystem(ShutdownReboot);
		else
			return FALSE;
	}

public:
	KsSystemDown();
   ~KsSystemDown();

};

#endif
