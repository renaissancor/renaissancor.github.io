#include <stdio.h> 
#include <tchar.h>
#include <windows.h>

int _tmain(const int argc, const TCHAR* argv[]) {
	SetProcessAffinityMask(GetCurrentProcess(), 1);

	STARTUPINFO si = { 0, }; 

	PROCESS_INFORMATION pi; 
	si.cb = sizeof(si); 

	TCHAR command[] = _T("Operation2.exe"); 

	CreateProcess(
		NULL, command,
		NULL, NULL, TRUE,
		0, NULL, NULL, 
		&si, &pi
	); 

	DWORD timing = 0; 
	while (1)
	{
		for (DWORD i = 0; i < 10000; ++i)
			for (DWORD i = 0; i < 10000; ++i); 

		_fputts(_T("Parent \n"), stdout); 
		++timing; 
		if (timing == 2)
			SetPriorityClass(pi.hProcess, NORMAL_PRIORITY_CLASS); 
	}

	return 0;
}