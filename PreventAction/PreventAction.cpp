// PreventAction.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


#include <windows.h>
#include <crtdbg.h>
#include <math.h>


//����g_hWnd������ֹdll����  
#pragma data_seg("PreventAction")  
HWND g_hWnd = NULL;             //���ھ��  
#pragma data_seg()  
#pragma comment(linker,"/section:PreventAction,RWS")  

HHOOK g_hlowKeyHook = NULL;     //�ͼ����̹��Ӿ��  
HHOOK g_hMouse = NULL;          //��깳�Ӿ��  
HINSTANCE g_hInstance = NULL;   // global instance handle
bool	g_downF1 = false;
bool	g_downF12 = false;
int		g_timeF1 = 0;
int		g_timeF12 = 0;

//�ͼ����̹���  
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)  
{  
	bool isClick = false;  
	PKBDLLHOOKSTRUCT LowKey = NULL;   //�ýṹ������ײ���������¼�����Ϣ  

	if (nCode == HC_ACTION)      //HC_ACTION  ��ʾ����Ϣ  
	{  
		LowKey = (PKBDLLHOOKSTRUCT)lParam;  
		switch (wParam)  
		{  
		case WM_KEYDOWN:  
			{  
				isClick = 1;
				if (LowKey->vkCode == VK_F1)
				{
					g_downF1 = !g_downF1;

					if( g_downF1 )
						g_timeF1 = (int)GetTickCount();
				}
				else if (LowKey->vkCode == VK_F12)
				{
					g_downF12 = !g_downF12;
					if( g_downF12 )
						g_timeF12 = (int)GetTickCount();
				}
				else
				{
					g_downF1 = false;
					g_downF12 = false;
				}

				if( g_downF1 && g_downF12 && 
					abs(g_timeF12-g_timeF1) < 2000 )
				{
					
					UnhookWindowsHookEx(g_hlowKeyHook);    //ж�صͼ����̹���
				}


				//{  
				//	//  ::MessageBox(NULL,"�������Ӻ��ų�������ж�ع���~~�رճ���","��ʾ",MB_OK);  
				//	//SendMessage(g_hWnd, WM_CLOSE, 0, 0);   //���͹ر���������Ϣ  
				//	UnhookWindowsHookEx(g_hlowKeyHook);    //ж�صͼ����̹���  
				//	//UnhookWindowsHookEx(g_hMouse);         //ж�صͼ���깳��
				//}  
				break;  
			}  
		case WM_SYSKEYDOWN:  
		case WM_KEYUP:  
		case WM_SYSKEYUP:  
			{              // ����Win  
				isClick = (LowKey->vkCode == VK_LWIN) || (LowKey->vkCode == VK_RWIN) ||    
					//����Alt+F4  
					((LowKey->vkCode == VK_F4) && ((LowKey->flags & LLKHF_ALTDOWN) != 0)) ||  
					// ����Alt+Tab  
					((LowKey->vkCode == VK_TAB) && ((LowKey->flags & LLKHF_ALTDOWN) != 0)) ||  
					// ����Alt+Esc  
					((LowKey->vkCode == VK_ESCAPE) && ((LowKey->flags & LLKHF_ALTDOWN) != 0)) ||  
					// ����Ctrl+Esc  
					((LowKey->vkCode == VK_ESCAPE) && ((GetKeyState(VK_CONTROL) & 0x8000) != 0));  
				break;  
			}  
		default:  
			break;  
		}  
	}  
	if (isClick)  
	{  
		return 1;  
	}  
	return CallNextHookEx(g_hlowKeyHook,nCode,wParam,lParam);  //������һ������  
}  

//�������������Ϣ  
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)  
{  
	return 1;  
}  

//��װ����  
//void SetHook(HWND hWnd)  //Ϊ�˲������壬���ǽ�������ľ������ȥ  
void SetHook()
{  
	//g_hWnd = hWnd;       //��������������ȫ�־����Ȼ�����������Ĺ��ӹ�����ʹ��  
	//��װ�ͼ����̹���  
	g_hlowKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL,LowLevelKeyboardProc,g_hInstance,0);  
	//��װ��깳��  
	//g_hMouse = SetWindowsHookEx(WH_MOUSE,MouseProc,g_hInstance,0);  
}

__declspec(dllexport) BOOL PAInit()
{
	SetHook();
	return TRUE;
}

__declspec(dllexport) void PATerm()
{
	UnhookWindowsHookEx(g_hlowKeyHook);    //ж�صͼ����̹���  
	//UnhookWindowsHookEx(g_hMouse);         //ж�صͼ���깳��  
}

///**
// * The following global data is SHARED among all instances of the DLL
// * (processes); i.e., these are system-wide globals.
// **/ 
//#pragma data_seg(".preventAction")	// you must define as SHARED in .def
//HHOOK 	g_hHkKeyboard = NULL;	// handle to the keyboard hook
//HHOOK 	g_hHkMouse = NULL;	// handle to the mouse hook
//DWORD	g_dwLastTick = 0;	// tick time of last input event
//LONG	g_mouseLocX = -1;	// x-location of mouse position
//LONG	g_mouseLocY = -1;	// y-location of mouse position
//#pragma data_seg()
//#pragma comment(linker, "/section:.preventAction,rws")
//
//HINSTANCE g_hInstance = NULL;   // global instance handle
//
///**
// * Get tick count of last keyboard or mouse event
// **/
//__declspec(dllexport) DWORD PAGetLastTickCount()
//{
//	return g_dwLastTick;
//}
//
///**
// * Keyboard hook: record tick count
// **/
//LRESULT CALLBACK cbKeyboardTracker(int code, WPARAM wParam, LPARAM lParam)
//{
//	if (code==HC_ACTION) 
//	{
//		g_dwLastTick = GetTickCount();
//
//		int nVK = wParam;
//		if( nVK == 'a' || nVK == 'A' )
//			return 1;
//	}
//	return ::CallNextHookEx(g_hHkKeyboard, code, wParam, lParam);
//}
//
///**
// * Mouse hook: record tick count
// **/
//LRESULT CALLBACK cbMouseTracker(int code, WPARAM wParam, LPARAM lParam)
//{
//	if (code==HC_ACTION) {
//		MOUSEHOOKSTRUCT* pStruct = (MOUSEHOOKSTRUCT*)lParam;
//		//we will assume that any mouse msg with the same locations as spurious
//		if (pStruct->pt.x != g_mouseLocX || pStruct->pt.y != g_mouseLocY)
//		{
//			g_mouseLocX = pStruct->pt.x;
//			g_mouseLocY = pStruct->pt.y;
//			g_dwLastTick = GetTickCount();
//		}
//	}
//	return ::CallNextHookEx(g_hHkMouse, code, wParam, lParam);
//}
//
///**
// * Initialize DLL: install kbd/mouse hooks.
// **/
//__declspec(dllexport) BOOL PAInit()
//{
//	if (g_hHkKeyboard == NULL) {
//		g_hHkKeyboard = SetWindowsHookEx(WH_KEYBOARD, cbKeyboardTracker, g_hInstance, 0);
//	}
//	if (g_hHkMouse == NULL) {
//		g_hHkMouse = SetWindowsHookEx(WH_MOUSE, cbMouseTracker, g_hInstance, 0);
//	}
//
//	_ASSERT(g_hHkKeyboard);
//	_ASSERT(g_hHkMouse);
//
//	g_dwLastTick = GetTickCount(); // init count
//
//	if (!g_hHkKeyboard || !g_hHkMouse)
//		return FALSE;
//	else
//		return TRUE;
//}
//
///**
// * Terminate DLL: remove hooks.
// **/
//__declspec(dllexport) void PATerm()
//{
//	BOOL bResult;
//	if (g_hHkKeyboard)
//	{
//		bResult = UnhookWindowsHookEx(g_hHkKeyboard);
//		_ASSERT(bResult);
//		g_hHkKeyboard = NULL;
//	}
//	if (g_hHkMouse)
//	{
//		bResult = UnhookWindowsHookEx(g_hHkMouse);
//		_ASSERT(bResult);
//		g_hHkMouse = NULL;
//	}
//}






BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		g_hInstance = hModule;
		break;
	case DLL_THREAD_ATTACH:	break;
	case DLL_THREAD_DETACH: break;
	case DLL_PROCESS_DETACH:
		//we do an unhook here just in case the user has forgotten.
		PATerm();
		break;
	}
	return TRUE;
}


/**
 * This is to prevent the CRT from loading, thus making this a smaller
 * and faster dll.
 **/
extern "C" BOOL __stdcall _DllMainCRTStartup( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    return DllMain( hinstDLL, fdwReason, lpvReserved );
}

