// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"


#include <Windows.h>

DWORD WINAPI MainThread(LPVOID param) {
	while (true) {
		if (GetAsyncKeyState(VK_SPACE) & 0x80000) {
			MessageBoxA(NULL, "Spacebar pressed!", "Spacebar pressed!", MB_OK);
		}
		Sleep(100);
	}
	return 0;
}

BOOL APIENTRY DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		MessageBoxA(NULL, "DLL injected!", "DLL injected!", MB_OK);
		CreateThread(0, 0, MainThread, hInstDLL, 0, 0);
	}
	return true;
}

