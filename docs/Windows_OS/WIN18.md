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

ANSI American Standard offered (defined? in more correct description)
File related functions supported 

File I/O ANSI standard 
for all Windows Linux UNIX all OS 
Platform independent 

Suppose Hardware, install Windows Linux UNIX etc 
File System dependent, how to define and organize file? 

Who designs file system? Part of OS 
File System is part of Operating System 
Some colleges have independent lecture about file system and its internal 
data structure in OS, while OS lecture partially explain it 
OS defined independent system 

ANSI vs OS 
Windows Linux UNIX etc ... 
File system function call then file creation process will be 
before ANSI System File functions supported by Windows OS is completed 

So, creating file must be done by WINAPI OS System function 
fopen function will create Windows System function 
Linux UNIX similar, ANSI is just a shell. 
Internally recall system function 

Win Linux UNIX etc ... functions 
ANSI functions only support common sharing part 
Each OS System function should be used. 
