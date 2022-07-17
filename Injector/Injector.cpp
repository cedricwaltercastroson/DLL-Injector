#pragma warning(disable: 6001)
#pragma warning(disable: 6387)

#include <Windows.h>
#include <TlHelp32.h>

VOID startup(LPCTSTR lpApplicationName) {

	// additional information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// start the program up
	CreateProcess(lpApplicationName,    // the path
		NULL,							// Command line
		NULL,							// Process handle not inheritable
		NULL,							// Thread handle not inheritable
		FALSE,							// Set handle inheritance to FALSE
		0,								// No creation flags
		NULL,							// Use parent's environment block
		NULL,							// Use parent's starting directory 
		&si,							// Pointer to STARTUPINFO structure
		&pi								// Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	);
	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

int getProcId(const wchar_t* target) {

	DWORD pID = 0;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	do {
		if (wcscmp(pe32.szExeFile, target) == 0) {
			CloseHandle(hSnapshot);
			pID = pe32.th32ProcessID;
			break;
		}
	} while (Process32Next(hSnapshot, &pe32));
	CloseHandle(hSnapshot);
	return pID;
}

int APIENTRY wWinMain(_In_     HINSTANCE hInstance,
					  _In_opt_ HINSTANCE hPrevInstance,
					  _In_     LPWSTR    lpCmdLine,
					  _In_     int       nCmdShow) {

	const wchar_t* process = L"Game.exe";	// name of the process to inject into
	startup(process);						// launches the process/program
	int pID = getProcId(process);			// get our process id

	char dll[] = "payload.dll";				// name of the dll file to inject
	char dllPath[MAX_PATH] = { 0 };			// full path of the dll
	GetFullPathNameA(dll, MAX_PATH, dllPath, NULL);

	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, TRUE, pID);
	LPVOID pszLibFileRemote = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	WriteProcessMemory(hProcess, pszLibFileRemote, dllPath, strlen(dllPath) + 1, NULL);
	HANDLE handleThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, pszLibFileRemote, NULL, NULL);

	WaitForSingleObject(handleThread, INFINITE);
	CloseHandle(handleThread);
	VirtualFreeEx(hProcess, dllPath, 0, MEM_RELEASE);
	CloseHandle(hProcess);

	return 0;
}
