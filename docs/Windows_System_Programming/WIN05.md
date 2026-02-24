# Create & Terminate Process  

Windows OS 
CreateProcess Function Parameters 
```cpp
BOOL CreateProcess (
    LPCTSTR lpApplicationName,          // 1. Name of executable module
    LPTSTR lpCommandLine,               // 2. Command line string
    LPSECURITY_ATTRIBUTES lpProcessAttributes,  // 3. Process security attributes
    LPSECURITY_ATTRIBUTES lpThreadAttributes,   // 4. Thread security attributes
    BOOL bInheritHandles,               // 5. Handle inheritance flag
    DWORD dwCreationFlags,              // 6. Creation flags
    LPVOID lpEnvironment,               // 7. Environment block
    LPCTSTR lpCurrentDirectory,         // 8. Current directory
    LPSTARTUPINFO lpStartupInfo,        // 9. Startup information
    LPPROCESS_INFORMATION lpProcessInformation  // 10. Process information
);
```

struct `STARTUPINFO` 구조체 

```cpp
typedef struct _STARTUPINFOW {
    DWORD   cb;
    LPWSTR  lpReserved;
    LPWSTR  lpDesktop;
    LPWSTR  lpTitle;
    DWORD   dwX;
    DWORD   dwY;
    DWORD   dwXSize;
    DWORD   dwYSize;
    DWORD   dwXCountChars;
    DWORD   dwYCountChars;
    DWORD   dwFillAttribute;
    DWORD   dwFlags;
    WORD    wShowWindow;
    WORD    cbReserved2;
    LPBYTE  lpReserved2;
    HANDLE  hStdInput;
    HANDLE  hStdOutput;
    HANDLE  hStdError;
} STARTUPINFOW, *LPSTARTUPINFOW;
```

현재 디렉터리 설정 
```cpp
DWORD GetCurrentDirectory (
    DWORD nBufferLength, // memory buffer size 
    LPTSTR lpBuffer      // memory buffer pointer 
); 
// If fail, return 0; 
``` 
