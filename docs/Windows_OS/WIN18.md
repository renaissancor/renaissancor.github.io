# File Basic IO 

`CreateFile` function 
Open the file, rather than really create file. 
It creates File Kernel Object in OS, rather than 
creating file in Disk 

```cpp
HANDLE CreateFile (
    LPCTSTR lpFileName, 
    DWORD dwDesiredAccess, 
    DWORD dwShareMode, 
    LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
    DWORD dwCreationDisposition, 
    DWORD dwFlagsAndAttributes, 
    HANDLE hTemplateFile 
); 
```

`ReadFile` reads data 

```cpp
BOOL ReadFile (
    HANDLE hFile, 
    LPVOID lpBuffer, 
    DWORD nNumberOfBytesToRead, 
    LPDWORD lpNumberOfBytesRead, 
    LPOVERLAPPED lpOverlapped 
); 
```
If `ReadFile` function fails return value is zero 

`WriteFile` saves data 
```cpp
BOOL WriteFile(
    HANDLE hFile, 
    LPVOID lpBuffer, 
    DWORD nNumberOfBytesToWrite, 
    LPDWORD lpNumberOfBytesWritten, 
    LPOVERLAPPED lpOverlapped 
)
```

If `WriteFile` function fails return value is zero 