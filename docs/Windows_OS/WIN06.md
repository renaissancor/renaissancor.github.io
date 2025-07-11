# Kernel Object and HANDLE 


Kernel Object 

CreateProcess let Windows OS make Process 


Process Status Info 

- Running 
- Blocked
- Ready 

Process Priority Info 

OS Process Scheduler will handle these two info : 

- Process Status Info 
- Process Priority Info 

In Windows File generation of Kernel Object 
Files are also under Windows Kernel's control 

1 to 1, kernerl object per resource 

```txt
 Kernel Object       Resource 
|------------|      |---------|
| Pipe KO    | <--> | Pipe    |
|------------|      |---------|
| Process KO | <--> | Process |
|------------|      |---------|
| Thread KO  | <--> | Thread  |
|------------|      |---------|
```

In Windows OS, Kernel Object is controlled by HANDLE 

For instance Priority change of Process, which is Windows Kernel Object, by 

```cpp
BOOL SetPriorityClass (
    HANDLE hProcess, 
    DWORD dwPriorityClass 
);
```

so Process is accessible by HANDLE datatype 

## Operation1.cpp Source Code 

```cpp
#include <stdio.h> 
#include <tchar.h>
#include <windows.h>
#pragma comment(lib, "User32.lib")

int _tmain(const int argc, const TCHAR* argv[]) {
	SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS); // Normal Priority 
	SetProcessAffinityMask(GetCurrentProcess(), 1);             // Single Core Setup, use CPU No.1 Only  

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
```

## Operation2.cpp Source Code 
```cpp
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
```

Compile and Run then 

```shell 
cl Operation1.cpp
cl Operation2.cpp
.\Operation1.cpp 
```

Result is similar to 

```txt 
Operation2.exe
Operation2.exe
Operation2.exe
...
Operation2.exe
Operation2.exe
Operation2.exe
Operation2.exe
Operation2.exe
Operation2.exe
Operation1.exe
Operation2.exe
Operation2.exe
Operation2.exe
```

### Kernel Objects are Subordinate to OS, not Process, while 

Kernel Object is Subordinate to the Windows Operating System, not Process 

### HANDLE , aka handle table, are subordinate to Process, not OS 

KerObjShare.cpp 

```cpp
#include <stdio.h> 
#include <tchar.h>
#include <windows.h>

int _tmain(const int argc, const TCHAR* argv[]) {
	
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
```

The core difference between `Operation1.cpp` and `KerObjShare.cpp` are 
Last lines that only exist in `KerObjShare.cpp` code. 
```cpp
++timing; 
		if (timing == 2)
			SetPriorityClass(pi.hProcess, NORMAL_PRIORITY_CLASS);
```

Structure of PROCESS_INFORMATION is following 

```cpp

typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread; 
    DWORD dwProcessId; 
    DWORD dwThreadId; 
} PROCESS_INFORMATION; 
```

By Process HANDLE variables obtained 