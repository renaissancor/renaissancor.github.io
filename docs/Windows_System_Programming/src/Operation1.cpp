#include <stdio.h> 
#include <tchar.h>
#include <windows.h>
#pragma comment(lib, "User32.lib")

int _tmain(const int argc, const TCHAR* argv[]) {
	// SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS); // 낮은 우선순위
	SetProcessAffinityMask(GetCurrentProcess(), 1);             // CPU 0만 사용

	STARTUPINFO si = { 0, }; 

	PROCESS_INFORMATION pi; 
	si.cb = sizeof(si); 
	TCHAR command[] = _T("Operation2.exe"); 

	CreateProcess(
		NULL,
		command,
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		&si,
		&pi
	); 

	WaitForInputIdle(pi.hProcess, INFINITE);
	
	if (!SetProcessAffinityMask(pi.hProcess, 1)) {
		_ftprintf(stderr, _T("SetProcessAffinityMask failed: %lu\n"), GetLastError());
		return 1;
	}

	while (1) {
		for (DWORD i = 0; i < 10000; ++i)
			for (DWORD i = 0; i < 10000; ++i); // Busy Waiting 
		_fputts(_T("Operation1.exe \n"), stdout); 
	}

	return 0; 
}