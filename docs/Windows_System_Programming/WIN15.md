# Thread Pool 

Generation and Termination of Thread give System a lot of pressure. 

Thread Pool is a structure that manage already created threads 
and allocate one of threads in thred pool when work is requested 
and retrieve that thread when work is done, returning memory pool to the thread pool. 

Middleware 
Framework support Thread Pool 
Engineers 
how to make thread pool? 

Do not need to make thread pool by yourself 
However making for understanding worth 

First understand thread pool 
Second make thread pool 
Complete check about thread pool 



## What is Work 

Function pointer 

WORK defined as function pointer 

```cpp
typedef void (*WORK) (void); 
// In disassembly pointer value address in code section 

typedef struct __WorkerThread 
{
    HANDLE hThread; 
    DWORD idThread; 
} WorkerThread; 


struct __ThreadPool 
{
    WORK workList[WORK_MAX]; (Saving function pointers)

    WorkerThread workerThreadList[THREAD_MAX]; 
    HANDLE workerEventList[THREAD_MAX]; 

    DWORD idxOfCurrentWork; 
    DWORD idxOfLastAddedWork; 

    DWORD threadIdx; 
} gThreadPool; 
``` 

Save WORK, aka function pointer, in sequential order. 

```
    Thread<+
            \
|WORK|WORK|WORK|WORK|WORK|WORK|WORK|    |    |
            |                         |
     idxOfCurrentWork        idxOfLastAddedWork
```

Functions before idxOfCurrentWork are either completed or assigned in progress by Thread. 
So, after idxOfCurrentWork, functions are registered but not executed yet. 

When function is called, Thread waits by using 
WaitForSingleObject looking at Event Kernel Object 
so it gets automatically blocked. 

Thread ststus if blocked, will not be scheduled by OS. 

So when Thread is created, then let each thread blocked to wait for Event, 
so that it sleeps while OS scheduler will not assign resources 
This is status saved in Thread pool. 

Work is function pointer. When WORK is registered, Thread pool might awake thread. 
One or all or whatever ... waking only one thread per work would be efficient way. 

In thread pool wake up thread from thread pool. 
From non signaled to signaled.  

Thread function how to get implemented? First WaitForSingleObject call 
sleeping, when event signaled it gets ready. 
Then that thread will get function pointer from WORK array and execute it. 

When function by WORK access is over and returned get into loop again 
Waiting for Single Object. 

Thread that wait Event Object assigned to itself until it gets signaled, 
when WinAPI Event gets out from WaitForSingleObject call function by 
WORK function pointer then WaitForSingleObject then repeat ...  

# Memory Hierarchy 

Memory 

Main Memory is RAM, DRAM. 

Register is also memory but inside processor, so it is memory inside CPU. 

Cache is not part of CPU, but concept of memory in CPU, 
even though it is physically inside CPU. 

Cache is Memory closest to the CPU.

Secondary Storage AKA Auxiliary Storage like HDD and SSD are also Memory. 
Also SD Card CD ROM USB etc ... these I/O (Input Output) Devices are also memory. 

Programmer should always conscious about memory during system software development. 

Programmer should consider  based on register cache RAM HDD SSD and all other I/O
devices input output timing and wait time as most priority, thinking about it consistently. 

What memory does during program execution is Input / Output. 
Basic concepts all memory hierarchy are same even from register to HDD. 
The point is , how far they are from CPU. It is in order
Register Cache RAM SSD HDD 

Closer to CPU means faster, and further from CPU mean sslower. 
CPU have nearly no time to access register, relatively longer time for cache, 
and should use RAM by some complicated processes IO Bus interface control. 
Also, closer distance to the CPU means more expense and technical limit. 
Thus memory will have pyramid hierarchy 

Register L1Cache L2Cache RAM SSD/HDD 

Mostly, it is generally over 90% probability to include data trying to find
are inside L1 Cache or L2 Cache, Cache hit percentage is over 90%. 

## Virtual Memory 




