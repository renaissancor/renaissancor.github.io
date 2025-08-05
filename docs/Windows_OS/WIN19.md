# Asynchronous I/O 

Video Player program, recieve data and play principle 

```
|receive| play  |receive| play  |receive|
| data  | video | data  | video | data  |
```

This is kind of exaggarated, but in this case CPU usage will have 
drastic change during play video and receive data. 

Since CPU will NOT work while receiving data, 
because I/O Input/Output of file is nearly 
CPU independent calculation. 

ANSI standard I/O function usage to read or write file data 
will be in blocking status. These functions waiting until 
I/O finish is **Blocking** Function, and these blocking function 
based I/O is called as **Synchronous I/O**. 

If you launch editor file that should load quite big file, there might be 
subtle Delay during opening such a large file. 
However, in video player perspective it is crucial problem to feel delay in that way. 

Data retrieve does not require CPU allocation that much. 
Thus during play even though I/O work concurrently there won't be big problem. 
So continue I/O, while play buffered data would siginficantly reduce latency. 
This concurrently I/O with execution of CPU functions is called as 
**Asynchronous I/O**. 

## Overlapped I/O 

ANSI standard function `fread` reads data from file. 

However, it is ANSI, so it is synchronous function. 
Meaning that function `fread` will be blocking function until I/O is finished. 
Suppose loading a music to play it during loading more larger file. ANSI based 
synchronous music file read will result in music began to be played after 
all files are loaded, which is far from expected consequence. 


The problem is **Blocking** function. Most functions that were observed, 
are blocking based like this. When function is called, it is in blocking status 
until it is completed, so two different works, file loading and music play 
do NOT work simultaneously unlike intention, since 
function call to receive data followed by function call of music player is impossible. 

This is synchronous I/O, function blocking let play function called after fread is returned. 

```
fread call     fread return 
+              +
|--------------|
| Receive Data |
|--------------|------------~
               | Sound Play ~
               |------------~
               +            
               play call    
```

Thus, to read data and play music simultaneously, function `fread` should be 
**Non-Blocking**. 

Blocking function is returned after completed, 
Non-Blocking function will return regardless of function completion! 
Moreover, even after return, function keep operation. 

ANSI standard does NOT contain concept of Non-Blocking, so `fread` cannot work in Non-Blocking way in default. 
However, Windows System functions like `ReadFileEx` can be operated in Non-Blocking. 

This is asynchronous Overlapped I/O,  function Non-Blocking so play music function 
wait for a short period of time for `ReadFile` return while function `ReadFile` operates after return. 

```
ReadFile       
call     
|    ReadFile
|    Retuen   
+    +       
|--------------|
| Receive Data |
|----------------------------~
     | Sound Play           ~
     |-----------------------~
     +           
    play call    
```

This kind of asynchronous file reading / writing functions that continue execution after return, 
can be literally overlapped multiple times.  

```
+----------------------------+
|        Receive Data        |
+---+------------------------+---+
    |        Receive Data        |
    +---+------------------------+---+
return  |        Receive Data        |
        +----------------------------+
    return
```

This is called as **Overlapped I/O**. I/O calculation might be literally overlapped multiple times. 

In `cpp` struct code, Overlapped struct. 

```cpp
typedef struct _OVERLAPPED 
{
    ULONG_PTR Internal; 
    ULONG_PTR InternalHigh; 
    DWORD Offset;     // Union 
    DWORD OffsetHigh; // Union 
    HANDLE hEvent; 
} OVERLAPPED; 

```

## Completion Routine Based Extended I/O 

Event Kernel Object is used to be signaled when I/O is complete. 



```cpp
BOOL WriteFileEx(
    HANDLE hFile, // HANDLE to Output File 
    LPCVOID lpBuffer, // Data Buffer 
    DWORD nNumberOfBytesToWrite, // numbers of Bytes to Write 
    LPOVERLAPPED lpOverlapped, // Overlapped Buffer 
    LPOVERLAPPED_COMPLETEION_ROUTINE lpCompletionRoutine // 
);
```

I/O Completion Routine is basically 
adding Routine (CALLBACK Function) after I/O is completed 
Difference between Overlapped I/O and I/O Completion Routine is 
simply that, just adding Routine (CALLBACK Function) after I/O is completed 

```cpp
VOID CALLBACK FileIOCompletionRoutine(
    DWORD dwErrorCode, // Completion Code 
    DWORD dwNumberOfByytesTransferred, // Number of Bytes Transferred 
    LPOVERLAPPED lpOverlapped // IO Information Buffer 
);
```

This `WriteFileEx` function is IO CoRoutine based. 
It connects I/O and completion routine. 
Since Completion Routine will automatically call CALLBACK function, 
Event Kernel Object and its HANDLE, used in `Overlapped` struct 
is now unnecessary. 

Waiting can be done like this. 

```cpp
WaitForSingleObject(overlappedInst.hEvent, INFINITE); 
```

Anyway, Completion Routine like `WriteFileEx` or `ReadFileEx` can be used 
like this, and given function must be void function pointer. 
Also, this function is not called by process thread, but 
automatically by Windows System as a CALLBACK function. 

Also, since CoRoutine do NOT use Event HANDLE value as Event Kernel Object, 
it can be used to pass value intended, since only void function is 
used as CALLBACK function without return, so use HANDLE instead. 

## Alertable State 

SleepEX(INFINITE, TRUE); 

Thread that called Function will be **alertable state** 
I/O function call get return, and I/O continues. 
If I/O completion is required for further execution, and still uncertain about 
when I/O will finish, but still after I/O Completion, CALLBACK function is guaranteed. 

Then, the priority of the work would be into Completion Routine. 
However, if intended to execute Completion Routine, so either want to 
get priority of CoRoutine higher or lower than original priority of CoRoutine, 
Alertable State is used. Alert that it is okay to go Completion Routine and 
do CALLBACK function. 

There are three ways of Alarm methods of these I/O Completion Routine Alertable State
```cpp 
DWORD SleepEx(
    DWORD dwMiliseconds, 
    BOOL bAlertable 
)
DWORD WaitForSingleObjectEx(...)
DWORD WaitForMultipleObjectsEx(...)
```

These functions make Alertable State, Completion Routine Priority Setup. 
`SleepEx`, unlike `Sleep`, has `BOOL bAlertable` value as difference. 

If these are called, when I/O function already returned but still in I/O, then 
they will not get called. 

## APC (Asynchronous Procedure Call)

Every Thread has each of their own APC Queue, 

Each Thread, when their I/O are completed and became alertable state, then 
all CALLBACK functions are collected in APC Queue 

So what inside APC Queue are CALLBACK functions that will be 
executed when I/O is in alertable state. 

