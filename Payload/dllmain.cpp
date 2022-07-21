#pragma warning(disable: 4200)

// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <memory.h>
#include <stdint.h>

unsigned long int gold_boost = 10000;

struct resource {
	unsigned long int gold;
};

struct memory_ptr {
	DWORD base_address;
	int total_offsets;
	int offsets[];
};

memory_ptr gold_hack_ptr = {
		0x00002DB0,
			3,
	{0x58, 0x130, 0xB8}
};

//pointer to resource struct
resource* resource_hack;

// get module base address
HMODULE getBaseAddress() {
	return GetModuleHandle(NULL);
}

DWORD* trace_pointer(memory_ptr* hack) {
	DWORD* location = (DWORD*)((DWORD)getBaseAddress() + (DWORD)hack->base_address);
	for (int i = 0; i < hack->total_offsets; i++) {
		location = (DWORD*)(*location + (int)hack->offsets[i]);
	}
	return location;
}

void init_pointers() {
	if (resource_hack == NULL) {
		resource_hack = (resource*)(trace_pointer(&gold_hack_ptr));
	}
}

void increase_gold() {
	init_pointers();
	if (resource_hack->gold < 990000) {
		Beep(1000, 500);
		resource_hack->gold += (long int)gold_boost;
	}
//	resource_hack->gold += (long int)gold_boost;
}

DWORD WINAPI MainThread(LPVOID param) {
//	Beep(1000, 1000);
//	MessageBoxA(NULL, "DLL injected!", "DLL injected!", MB_OK);
	while (true) {
		if (GetAsyncKeyState(VK_OEM_3) & 0x80000) {
//			Beep(1000, 500);
			increase_gold();
//			MessageBoxA(NULL, "~ pressed!", "~ pressed!", MB_OK);
		}
		Sleep(100);
	}
	return 0;
}

//DllMain entry and create MainThread
BOOL APIENTRY DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, MainThread, hInstDLL, 0, 0);
	}
	return true;
}

