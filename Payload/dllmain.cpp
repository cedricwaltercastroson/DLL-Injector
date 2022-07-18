// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>

DWORD WINAPI MainThread(LPVOID param) {
//	MessageBoxA(NULL, "DLL injected!", "DLL injected!", MB_OK);
	while (true) {
		if (GetAsyncKeyState(VK_OEM_3) & 0x80000) {
//			MessageBoxA(NULL, "~ pressed!", "~ pressed!", MB_OK);
			Beep(750, 800);
		}
		Sleep(100);
	}
	return 0;
}

BOOL APIENTRY DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, MainThread, hInstDLL, 0, 0);
	}
	return true;
}

