// KernelObjProb1.cpp 

#include <stdio.h> 
#include <tchar.h>
#include <windows.h>


int _tmain(const int argc, const TCHAR* argv[]) 
{
	SetProcessAffinityMask(GetCurrentProcess(), 1);

	STARTUPINFO si = { 0, };

	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);

	TCHAR command[] = _T("KernelObjProb2.exe");

	BOOL success = CreateProcess(
		NULL, command,
		NULL, NULL, TRUE,
		0, NULL, NULL,
		&si, &pi
	);

	if (!success) {
		_tprintf(_T("CreateProcess failed. Error code: %lu\n"), GetLastError());
		return 1;
	}

	// WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hProcess); 

	return 0;
}