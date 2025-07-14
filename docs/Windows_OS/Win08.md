# Handle Table 

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