#include <stdio.h> 
#include <tchar.h>
#include <windows.h>

int _tmain(const int argc, const TCHAR* argv[]) {
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS); 
	SetProcessAffinityMask(GetCurrentProcess(), 1); // Operation2.exe 코드 내부에 삽입

	while (1) {
		for (DWORD i = 0; i < 10000; ++i)
			for (DWORD i = 0; i < 10000; ++i); 

		_fputts(_T("Operation2.exe \n"), stdout); 
		Sleep(0);
	}

	return 0;
}