# IPC Inter Process Communication 


## IPC Meaning 

Inter-Process Communication 
프로세스 사이의 통신 

통신 Communication, 데이터 주고받기 
프로세스들은 서로 만날 수 없음 공간이 독립적으로 분리

```txt
 Process A Process B
|--------| |--------|
|  DATA  | |  DATA  |
|--------| |--------|
|  HEAP  | |  HEAP  |
|~~~~~~~~| |~~~~~~~~|
| STACK  | |  STACK |
|--------| |--------|
```

두 다른 프로세스의 메모리 구조 
서로 만날 수 없도록 디자인 

## Mail Slot Based IPC 

Windows OS는 다양한 IPC 기법 제공 
메일 슬롯 Mail Slot 도 그중 하나 
편지를 넣을 수 있는 우체통 입구 
우체통은 Mail Box 

IPC를 위한 
Kernel Object MailSlot 을 먼저 만들어야 함 
```cpp
HANDLE CreateMailSlot (
    LPCTSTR lpName, 
    DWORD nMaxMessageSize,
    DWORD lReadTimeOut, 
    LPSECURITY_ATTRIBUTES lpSecurityAttributes
); 
```
if function `CreateMailSlot` fails, return value is `INVALID_HANDLE_VALUE`


## Singnaled VS Non-Signaled 

IPC 커널 오브젝트 상태 
여기서 Singal 은 끄라는 Signal 

Signaled 상태 (신호를 받은 상태)

Non-Signaled 상태 (신호를 받지 않은 상태) 

Kernel Object status change from 
- Signaled to Non-Signaled 
- Or Non-Signaled to Signaled 

Kernel Object state info 

Process end 
Non Signaled to Signaled or Vice Versa 




## Handle Inheritence 핸들 상속 

Microsoft Documentation 
[CreateProcessA Function](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessa) 

```cpp 
BOOL CreateProcessA(
  [in, optional]      LPCSTR                lpApplicationName,
  [in, out, optional] LPSTR                 lpCommandLine,
  [in, optional]      LPSECURITY_ATTRIBUTES lpProcessAttributes,
  [in, optional]      LPSECURITY_ATTRIBUTES lpThreadAttributes,
  [in]                BOOL                  bInheritHandles,
  [in]                DWORD                 dwCreationFlags,
  [in, optional]      LPVOID                lpEnvironment,
  [in, optional]      LPCSTR                lpCurrentDirectory,
  [in]                LPSTARTUPINFOA        lpStartupInfo,
  [out]               LPPROCESS_INFORMATION lpProcessInformation
);
```

5th parameter `bInheritHandles` determines whether HANDLE table registered 
handles to the child process or not. If `TRUE` is input then 
parent process HANDLE table info will get inherited to child process.

Here is how handle table works by inheritance 
```txt
 Parent Process HANDLE Table         Child Process HANDLE Table 
| HANDLE | Address |Inherit?|       | HANDLE | Address |Inherit?| 
|   127  |  0x1200 |   Y    |------>|   127  |  0x1200 |   Y    |
|   168  |  0x1680 |   N    |   /-->|   256  |  0x2400 |   Y    |
|   256  |  0x2400 |   Y    |--/
```

There exist column to figure out whether HANDLE will be inherited. 
Also, when Child process inherit HANDLE from parent process, it will be 
automatically inherit HANDLE to the child process's child process and continue. 

Then, HANDLE inheritence and Usage Count 

Suppose MailSlot created by CreateMailSlot function then process will be following. 

1. Mail Slot resource generation 
2. Kernel Object generation 
3. HANDLE info updated to HANDLE table **HANDLE**
4. CreateMailSlot Function get out, return HANDLE 

When Process gets HANDLE, people mistakenly focus on step 4. 
However, process receive HANDLE at step 3, when HANDLE info update to 
process HANDLE table. 

Suppose UC Usage Count in the following graphics from 
