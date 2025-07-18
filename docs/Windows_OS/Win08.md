# Inter Process Communication, HANDLE 

MailReceiver.cpp 

CreateMailslot 

## Process Handle table 

```txt
   Process 
|----------------|
|  Handle Table  | 
|----------------|    \
||Handle|Address||-----\  |------|
||------|-------||      \ | MAIL |
|| .... | 0x....||      / | SLOT |
||------|-------||-----/  |------|
||  256 | 0x2400||    /      ||
||------|-------||           ||
|| .... | 0x....||          \||/
||------|-------||           \/
|----------------|   |---------------|
           |         | Mail Slot     | 
           |-------- | Kernel Object |
    (256, 0x2400)    |---------------|
                       Assign 0x2400 
```

Handle table is where 
Handle info saved 
exist per each process 

When process CreateProcess or CreateMailSlot Function Call 
Resource Generation requested and HANDLE info obtained 

Process HANDLE TABLE will get registered 

Now CloseHandle function call Handle info 256 how is it saved at 0x2400 is known 
inside Handle Table, so that HANDLE 256 represents Kernel object addrexx 0x2400 

Not only just Handle Table, but 

Process Hanlde Table 

Because Handle Table is independent PER Process. 
AKA Each process owns its own handle 

## Inheritance of HANDLE 

CreateProcess creates new function, making new child process 

also handle table for child process is generated together with child process 

HANDLE inheritance 

BOOL CreateProcess 

5th parameter 
BOOL bInheritHandle 
asks whether HANDLE info registered in HANDLE table will be inherited or not 

bInheritHandle TRUE mdans parent process HANDLE table info will be inherited to child 
This is not  
HANDLE inherit, 

but HANDLE table info inherit 

Kernel Object Usage Count increments KO accessing process num. 

Now HANDLE Table info 
KO citing process num, Kernel object Usage Count increment 

If Kernel Object citing process what makes it this 
HANDLE Table that HANDLE info is updated 

Process getting HANDLE means 
HANDLE Table got that Handle info added 

Suppose CreateMailSlot function MailSlot is generated in this order 

1. MailSlot Resource generation 
2. Kernel Object generation 
3. Handle info added to HANDLE table 
4. CreateMailSlot function finish, returning HANDLE value 

When Process got HANDLE, people often focus into step 4, which is problematic. 

Process getting HANDLE means after Step 3. 
Handle Table A Handle info registered, A Handle is obtained. 

Normally 3rd and 4th steps proceed together, 


## Pseudo HANDLE and Duplicate 

Function `GetCurrentProcess` returns HANDLE of process itself.
However, the HANDLE returned by `GetCurrentProcess` is actually 
a Pseudo HANDLE. It is not registered at the HANDLE Table. 
It is rather close to the currently working process cited
promised integer is returned. Or, it might be compared to the 
network IP address `192.168.0.1` or `127.0.0.1` which represents 
IP to maintain localhost rather than real IP address in other's perspective. 

However, somethimes process might require its real HANDLE value 
rather than Pseudo HANDLE that works only inside process itself. 
This is when function `DuplicateHandle` below is used. 

```cpp
BOOL DuplicateHandle(
  [in]  HANDLE   hSourceProcessHandle, // Original HANDLE Owner Process 
  [in]  HANDLE   hSourceHandle,        // Original HANDLE to Duplicate 
  [in]  HANDLE   hTargetProcessHandle, // Duplicated HANDLE Owner Process 
  [out] LPHANDLE lpTargetHandle,       // Duplicated HANDLE value save address 
  [in]  DWORD    dwDesiredAccess,
  [in]  BOOL     bInheritHandle,
  [in]  DWORD    dwOptions
);
```

