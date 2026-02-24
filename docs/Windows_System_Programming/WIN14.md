# Thread Synchronization by sequence 

Thread Synchronization by sequence 

Manipulate sequence 

### Producer Consumer Pattern 

Suppose 

Input -> Thread -> Buffer -> Thread -> Output 

Already output size and time are expected. 
Output size and time are dependent on input. 

However, input is unexpected, because input is based on external factors. 
It can be many or few. If designe dfor 10Byte input and received 10MB input, 
if resource is not enough there might be data loss. 
If it is simply saving for input, it might take some time but okay, 
but if direct output is required, it would be impossible to 
give fully processed output based on input over limit to process for CPU. 

System engineers use this model above for this. 
Input thread keep receiving input and accumulate date into buffer. 
Output thread keep reading buffer and process it to output. 
Unless buffer got overflowed it is safe. 

In this model, input thread is producer thread while output thread is consumer thread. 

### Event based sync 

Manual Reset Event (수동 리셋 모드 이벤트)
```
+--------------+   SetEvent  +----------+
|              |------------>|          |
| Non Signaled |             | Signaled |
|              |<------------|          |
+--------------+ ResetEvent  +----------+
```
Event Object signaled or nonsignaled should be manually changed. Not automatic change 

Auto Reset Event (자동 리셋 모드 이벤트)
```
+--------------+   SetEvent  +----------+
|              |------------>|          |
| Non Signaled |             | Signaled |
|              |<------------|          |
+--------------+   WaitFor   +----------+
                   SingleObject 
```
Still Signaled is given by Producer Thread SetEvent() Function Manually.

However, from Singaled to Non-Signaled is done by `WaitForSingleObject` 
Thread will be in blocking mode until Kernel Object Event became Signaled. 

## Need more understanding 

Suppose two threads A and B access to Auto Reset Event 
If Producer Thread change Event as Signaled by `SetEvent` then 
Among one of threads A and B, `WaitForSingleObject` waiting 
if one thread is out then it will be Non-Signaled, and only one 
thread will have chance to do. 
If intended to wake ONLY One thread use Auto Reset Event. 

On the other hand, 
Suppose two threads A and B access to Manual Reset Event 
both Thread A and B will get activated simulataneously. 

### Event and Mutex 

In StringEvent.cpp 
Main thread is Producer thread and 
_beginthreadex thread is consumer thread. 

SetEvent(hEvent); 
WaitForSingleObjecT(hThread, INFINITE); 

Both threads waiting for Event to be signaled by `WaitForSingleObject`. 

Generally Critical SEction means memory access. Or certain code block.
Thread launch sequence got sync, but console access to print output did not.  

However, sometimes two different code blocks in different function 
might form critical section too. For instance, _fputs(string, stdout) in 
several threads, it is not completely synchronized. 

In StringEvent2.cpp 
This code is implemented with adding Mutex, 
covering _fputts(_T("str"), output); with 
```CPP
WaitForSingleObject(gSynString.hEvent, INFINITE); 
WaitForSingleObject(gSynString.hMutex, INFINITE); 
_fputts(whatever); 
ReleaseMutex(gSynString.hMutex);
```


### Timer Sync 

Timer and event meaning in software 

```cpp
HANDLE CreateWaitableTimer (
    LPSECURITY_ATTRIBUTES lpTimerAttributes, 
    BOOL bManualReset, 
    LPCTSTR lpTimerName 
)
```

Timer is also Kernel Object. 
If it alarms, it is signaled. 
Timer is, if some amount of time passes, it is signaled. 

Event is signaled when designated by concept like "Right Now!". 

However, Timer is Signaled like "10 seconds later and once per 5 seconds" like this. 

```cpp
BOOL SetWaitableTimer (
    HANDLE hTimer, 
    const LARGE_INTERGER* pDueTime, // Start Signal after 10 seconds, if input is 10 
    LONG lPeriod, // Signal per 5 second, and other times will be non signaled if input 5 
    PTIMERAPCROUTINE pfnCompletionRoutine, 
    LPVOID lpArgToCompletionRoutine, 
    BOOL fResume 
);
```

Check `PeriodicTimer.cpp` and `ManualResetTimer.cpp`. 

Timer based sync 

Critical Section seems more challenging

Event and Timer 
But if software is more sophisticated, Event and Timer are also often used. 
