# Thread Synchronization by Memory Access 

Two perspectives of thread synchronization 

Synchronize means matching different stuffs. 
For instance A server and B server synchronous means both data are in same status 

However, synchronous here represents order, or **sequence**, rather than match.

- Memory access synchronization about critical section 
- Sequence synchronization between threads, ex: if one thread need calculation result of other thread. 

Memory Access based synchronization 
Blocking simultaneous, or Councurrent access of more than one thread into memory 

Memory access synchronization Methods : User mode sync and kernel mode sync 

User mode sync is fast 
kernel mode sync has several functions 

inter thread sync between threads 

**Sequence synchronize**

Two people want to go toilet, and one use toilet while other wait 

**Memory access synchronize**

Two people want to go toilet, both people enter toilet at the same time 

User Mode Sync 

- Critical Section based sync 
- Interlock function based sync (Easy to make sync work)

Kernel Mode Sync (Memory Access sync)

- Mutex based sync 
- Semaphore based sync 
- Named Mutex based sync 
- Event based sync 

### Critical Section 

For sync methds, think about key to open toilet. 
Define beginning and end of the critical section first. 

```cpp
CRITICAL_SECTION gCriticalSection; // This is critical section key 
InitializeCriticalSection(&gCriticalSection); // Initialize 
EnterCriticalSection(&gCriticalSection); // Beginning of critical section, get key 
// Critical Section 
LeaveCriticalSection(&gCriticalSection); // End of critical section, return key 
DeleteCriticalSection(&gCriticalSection); 
``` 

If critical section is bigger, is it more stable? Yes! 
But it will decrement performance, because multiple threads access will be inefficient. 

### Interlock Function based sync 

Considering declaration of CRITICAL_SECTION, and 
Initialize Enter Leave Delete all these functions, 
it seems quite inefficient with too much codes to input. 

When Critical Section is so small, like the shared resources 
are only small global variable, then interlock functions can be used.  
Instead of doing gTotalCount++; inside Critical Sectioned code, 
InterlockedIncrement(&gTotalCount); can be used instead. 

## Kernel Level synch 

If scheduler do not work, context switching will not work. 
Thus, if thread A was working, then thread A will keep working without context switching 
so thread B cannot work while scheduler is paused, thus will not do context switching. 

Timer Disable interrupt based synchronization 

HW notice OS by interrupt by timer interrupt. HW notice OS time by interrupt. 

Disable interrupt means interrupt signal recognized by OS will be blocked, so 
OS will suppose that time is not passing. Then, scheduler will not do context switching, 
because time is not passing according to OS. 

This method is technically possible. 

### Semaphore & Mutex 

In OS, sync name is semaphore. Among semaphore, binary semaphore is mutex. 
So semaphore is theoretically larger concept including mutex. 

Suppose critical section access. 
Semaphore has several keys. 
Mutex has only one key. 

Semaphore can limit access of thread into critical section. 

```cpp 
HANDLE CreateMutex (
    LPSECURITY_ATTRIBUTES lpMutexAttributes, // Security setup, 
    // like whether HANDLE table Key is inherited or not 
    BOOL bInitialOwner, // Owner (TRUE FALSE) Whether Key generator will own it or not 
    LPCTSTR lpName      // Mutex Name 
);
```
Semaphore and Mutex are kernel objects, and synchronization is based on Kernel Mode. 
`LPSECURITY_ATTRIBUTES lpMutexAttribute` is kernel level security setup like 
whether HANDLE table Key is inherited or not. 

Mutex is also Kernel Object. In Mutex, Signaled is not finished status of process or thread, 
but it is not occupied. 

```cpp 
WaitForSingleObject();  // Make Mutex Non Signaled 
ReleaseMutex();         // Make Mutex Signaled 
```  

`WaitForSingleObject()` is waiting for mutex, kernel object, to be signaled, aka not occupied. 
After `WaitForSingleObject()` function will enter Critical Section, and it will be non signaled.

During some function is using `mutex` as a key and in critical section, mutex is non-signaled. 

`ReleaseMutex()` will get called after leaving Critical Section, to change mutex as signaled. 

In code, CRIRICAL_SECTION based synchronization and Mutex based sync looks very similar 
for simple critical section like `gdata++` one line example. 


### About Semaphore

```cpp
HANDLE CreateSemaphore (
    LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, // Security setup, 
    LONG lInitialCount, // Size , suppose it is 3 
    LONG lMaximumCount, // Must be bigger than 3. lMaximumCount >= lInitialCount
    LPCTSTR lpName      // Mutex Name 
); // If function fails return value is NULL 
```

While Mutex is just one single key, semaphore can be compared to 
several keys, whhere key number is `lMaximumCount`. 
While SemaphoreCount value is not zero, it is signaled status. 

```cpp 
WaitForSingleObject();  // Decrement Semaphore Count 
ReleaseSemaphore();     // Increment Semaphore Count 
```  

So, when WaitForSingleObject(); function is called, it will not get blocked, until 
`WaitForSingleObject` is called over `lMaximumCount` without `ReleaseSemaphore`, 
meaning entire total number of keys are in use without return, and 
`lMaximumCount` threads are in critical section. 

If `lMaximumCount` of semaphore is 1, then it is same as Mutex. 

### Named Mutex based process sync 

Suppsoe Process A and B exists. Trying to synchronize thread A and thread B, 
if each belong to different process. 
Is it possible to synchronize threads in different process by WinAPI Mutex! 

However, HANDLE based Mutex access is impossible outside Process. 
Then, how to register Mutex created from Process A to use in Process B? 
By using name `LPCTSTR lpName`. Giving name to mutex represents that 
this mutex will get accessed by external process. 

Register by name, and call Mutex created by other process by function `OpenMutex` 
with name input. 
This will work similar for semaphore too. 

Although it is not likely to use, good to know. 

```cpp
dwWaitResult = WaitForSingleObject(hMutex, INFINITE); 
switch(dwWaistResult) {
    case WAIT_ABANDONED: return; // Mutex return was not appropriately worked, 
    // so Windows OS will trace, sending Mutex key to the process in WaitForSingleObject. 
}
```

Mutex has ownership, but semaphore has no ownership. 
