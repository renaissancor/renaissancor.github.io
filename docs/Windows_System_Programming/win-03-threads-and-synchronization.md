---
title: Windows Threads and Synchronization
---

# Windows Threads and Synchronization

Windows provides two paths for creating threads and a layered set of synchronization primitives split by whether they operate in user mode or kernel mode. This post covers the concrete Win32 API — creation, termination, user-mode and kernel-mode sync, wait functions, and the thread pool. The underlying theory (race conditions, deadlock, the OS scheduler) is covered in the OS post *Synchronization and Deadlock*; only the platform-specific detail is here.

---

## Thread Creation: `CreateThread` vs `_beginthreadex`

The raw Win32 call is `CreateThread`:

```cpp
HANDLE CreateThread(
    LPSECURITY_ATTRIBUTES lpThreadAttributes,  // NULL = default security
    SIZE_T                dwStackSize,          // 0 = default 1 MB
    LPTHREAD_START_ROUTINE lpStartAddress,      // thread entry point
    LPVOID                lpParameter,          // argument passed to entry point
    DWORD                 dwCreationFlags,      // 0 = start immediately
    LPDWORD               lpThreadId           // receives thread ID
);
```

Three parameters matter in practice. `dwStackSize = 0` gives the default 1 MB stack — not zero bytes. `lpStartAddress` is a function pointer typed as `DWORD (WINAPI *)(LPVOID)`; pass the function name directly. `lpParameter` forwards a single pointer argument to that function.

**Why `_beginthreadex` instead.** `CreateThread` does not initialize per-thread state in the C/C++ runtime library. Functions that rely on per-thread CRT state — `errno`, `strtok`, `stdio` buffers — will misbehave or leak memory when threads are created with `CreateThread`. `_beginthreadex` allocates and initializes that per-thread CRT state correctly:

```cpp
uintptr_t _beginthreadex(
    void        *security,
    unsigned     stack_size,
    unsigned    (*start_address)(void *),
    void        *arglist,
    unsigned     initflag,
    unsigned    *threadaddr
);
```

The parameter order is identical to `CreateThread`. Any project that uses CRT functions should use `_beginthreadex` exclusively.

A typical worker thread receiving a pair of values via pointer:

```cpp
DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    DWORD *p    = (DWORD *)lpParam;
    DWORD start = p[0], end = p[1];
    DWORD total = 0;
    for (DWORD i = start; i <= end; i++) total += i;
    return total;
}

int main(void)
{
    HANDLE hThread[3];
    DWORD  ranges[] = { 1, 6,  7, 22,  23, 27 };  // three non-overlapping ranges

    for (int i = 0; i < 3; i++)
        hThread[i] = (HANDLE)_beginthreadex(
            NULL, 0, (unsigned(*)(void*))ThreadProc,
            &ranges[i * 2], 0, NULL);

    WaitForMultipleObjects(3, hThread, TRUE, INFINITE);
    return 0;
}
```

Note that scheduling order between a newly created thread and the line after the creation call is not defined — do not assume either runs first.

---

## Thread Termination

The correct way to end a thread is to `return` from its entry function. This unwinds the stack cleanly and, for `_beginthreadex` threads, frees the per-thread CRT block.

```cpp
BOOL TerminateThread(HANDLE hThread, DWORD dwExitCode);  // CreateThread variant
void _endthreadex(unsigned retval);                       // _beginthreadex variant
```

Both forced-termination calls are **dangerous**: the thread receives no notification and cannot release locks, flush buffers, or free heap allocations it owns. Avoid them outside of process-shutdown scenarios.

To retrieve what a thread returned:

```cpp
DWORD exitCode;
GetExitCodeThread(hThread, &exitCode);
// returns STILL_ACTIVE if the thread has not yet finished
```

### Suspend and Resume

Windows threads carry a *suspend count* in their kernel object. Each `SuspendThread` call increments it; each `ResumeThread` decrements it. A thread only runs when its suspend count reaches zero.

```cpp
DWORD SuspendThread(HANDLE hThread);  // returns previous suspend count, or -1 on failure
DWORD ResumeThread(HANDLE hThread);   // returns previous suspend count, or -1 on failure
```

---

## Synchronization: Two Axes

Thread synchronization in Win32 addresses two distinct problems:

| Axis | Problem | Win32 mechanism |
|------|---------|-----------------|
| **Memory access** | Multiple threads read-modify-write shared data concurrently | `CRITICAL_SECTION`, Interlocked, Mutex, Semaphore |
| **Sequence / ordering** | Thread B must wait until Thread A produces a result | Event, Timer, or Mutex used as a gate |

---

## User-Mode Synchronization

User-mode synchronization avoids a kernel transition on every acquire/release. It is faster but cannot synchronize across processes.

### `CRITICAL_SECTION`

```cpp
CRITICAL_SECTION gCS;

InitializeCriticalSection(&gCS);

// in each thread:
EnterCriticalSection(&gCS);
    // only one thread at a time inside here
LeaveCriticalSection(&gCS);

DeleteCriticalSection(&gCS);  // at shutdown
```

`EnterCriticalSection` blocks (spin-then-sleep) until no other thread holds the section. Making the protected region larger increases safety but reduces concurrency — size it to cover only the shared state that actually needs protection.

### Interlocked Functions

For a single shared integer counter, the four-function `CRITICAL_SECTION` dance is disproportionate. The interlocked family performs atomic read-modify-write in one instruction with no lock object:

```cpp
LONG gCounter = 0;

InterlockedIncrement(&gCounter);   // atomic ++
InterlockedDecrement(&gCounter);   // atomic --
InterlockedExchange(&gCounter, 5); // atomic assignment
InterlockedCompareExchange(&gCounter, newVal, expected); // CAS
```

Use interlocked functions when the entire critical region is a single variable operation.

---

## Kernel-Mode Synchronization

Kernel-mode objects cross the user/kernel boundary on every wait and release, which costs roughly an order of magnitude more than user-mode. The payoff is richer semantics: cross-process visibility, timeouts, and the ability to wait on multiple objects.

All kernel synchronization objects share the signaled/non-signaled state model and are waited on with the same two functions:

```cpp
DWORD WaitForSingleObject(HANDLE hObject, DWORD dwMilliseconds);
DWORD WaitForMultipleObjects(DWORD nCount, const HANDLE *lpHandles,
                              BOOL bWaitAll, DWORD dwMilliseconds);
// dwMilliseconds = INFINITE to block until signaled
// return values: WAIT_OBJECT_0, WAIT_TIMEOUT, WAIT_ABANDONED, WAIT_FAILED
```

### Mutex

A mutex has exactly one "key". When signaled it is unowned and available; when a thread acquires it the mutex becomes non-signaled until that thread calls `ReleaseMutex`.

```cpp
HANDLE hMutex = CreateMutex(NULL, FALSE, NULL); // not initially owned

WaitForSingleObject(hMutex, INFINITE); // acquire — becomes non-signaled
    // critical section
ReleaseMutex(hMutex);                  // release — becomes signaled again

CloseHandle(hMutex);
```

**Ownership matters.** Only the thread that acquired a mutex may release it. If a thread dies while holding a mutex, `WaitForSingleObject` returns `WAIT_ABANDONED` to the next waiter — the OS transfers ownership so the program can recover.

**Named mutex for cross-process sync.** Give the mutex a name at creation and open it by name in another process:

```cpp
// Process A
HANDLE hMutex = CreateMutex(NULL, FALSE, TEXT("Global\\MyAppLock"));

// Process B
HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, TEXT("Global\\MyAppLock"));
```

### Semaphore

A semaphore holds a counter between 0 and `lMaximumCount`. While the count is above zero the object is signaled. Each `WaitForSingleObject` decrements the count; `ReleaseSemaphore` increments it.

```cpp
HANDLE hSem = CreateSemaphore(
    NULL,
    3,    // lInitialCount — start with 3 permits available
    3,    // lMaximumCount — ceiling
    NULL
);

WaitForSingleObject(hSem, INFINITE);  // decrement count; blocks when count == 0
    // at most 3 threads here simultaneously
ReleaseSemaphore(hSem, 1, NULL);      // increment count by 1
```

A semaphore with `lMaximumCount = 1` is functionally equivalent to a mutex, except it has no ownership — any thread may call `ReleaseSemaphore`.

### Event

Events implement sequence synchronization: one thread signals "something happened" and one or more waiting threads wake up.

```cpp
HANDLE hEvent = CreateEvent(
    NULL,   // security
    FALSE,  // bManualReset: FALSE = auto-reset, TRUE = manual-reset
    FALSE,  // bInitialState: non-signaled
    NULL    // name
);

// Producer thread:
SetEvent(hEvent);    // transition to signaled

// Consumer thread:
WaitForSingleObject(hEvent, INFINITE);  // blocks until signaled

ResetEvent(hEvent);  // manual-reset only: return to non-signaled
CloseHandle(hEvent);
```

**Auto-reset vs manual-reset.** With auto-reset, `WaitForSingleObject` atomically consumes the signal and resets the event — exactly one waiting thread is released per `SetEvent`. With manual-reset, the event stays signaled until `ResetEvent` is called explicitly, waking all waiting threads simultaneously.

**Combining Event and Mutex.** Event controls when threads may proceed (sequence); Mutex controls exclusive access once they do (memory). A producer-consumer pattern often needs both:

```cpp
// consumer thread
WaitForSingleObject(hEvent, INFINITE);   // wait for data to be ready
WaitForSingleObject(hMutex, INFINITE);   // acquire exclusive access
    _fputts(buffer, stdout);
ReleaseMutex(hMutex);
```

### Waitable Timer

A waitable timer is a kernel object that becomes signaled after a delay, optionally repeating periodically.

```cpp
HANDLE hTimer = CreateWaitableTimer(NULL, FALSE, NULL); // auto-reset

LARGE_INTEGER dueTime;
dueTime.QuadPart = -100000000LL;  // 10 seconds from now (100-ns units, negative = relative)

SetWaitableTimer(
    hTimer,
    &dueTime,
    5000,   // lPeriod ms: re-signal every 5 s; 0 = one-shot
    NULL, NULL, FALSE
);

WaitForSingleObject(hTimer, INFINITE);
```

Use an event when the trigger is programmatic ("I finished computing X"); use a timer when the trigger is temporal ("every 5 seconds").

---

## Thread Pool

Creating and destroying threads for each unit of work is expensive — the kernel must allocate a stack (default 1 MB), initialize scheduler structures, and later tear them down. A thread pool amortizes that cost by keeping a fixed set of threads alive, blocking them on events when idle.

### Conceptual structure (manual implementation)

```cpp
typedef void (*WORK)(void);

struct ThreadPool {
    WORK         workList[WORK_MAX];
    HANDLE       workerEventList[THREAD_MAX]; // one auto-reset event per thread
    HANDLE       workerThreadList[THREAD_MAX];
    DWORD        idxOfCurrentWork;
    DWORD        idxOfLastAddedWork;
};
```

Each worker thread loops: wait on its personal event, fetch the next work item from the queue, execute it, repeat. When work is submitted, the pool signals one worker event (auto-reset ensures only one thread wakes per work item). Threads in the blocked `WaitForSingleObject` state are not scheduled by the OS — they consume no CPU while idle.

### Windows thread pool API

For production code, use the built-in thread pool rather than rolling your own:

```cpp
// Submit a one-shot work item
PTP_WORK work = CreateThreadpoolWork(WorkCallback, context, NULL);
SubmitThreadpoolWork(work);
WaitForThreadpoolWorkCallbacks(work, FALSE);
CloseThreadpoolWork(work);
```

`WorkCallback` has the signature `void CALLBACK WorkCallback(PTP_CALLBACK_INSTANCE, PVOID context, PTP_WORK)`. The pool manages thread count, reuse, and idle teardown automatically.

---

## Choosing the Right Primitive

| Need | Preferred primitive | Why |
|------|--------------------|----|
| Protect a small counter | `InterlockedIncrement` | Single instruction, no object overhead |
| Protect a code block (single process) | `CRITICAL_SECTION` | User-mode, lowest latency |
| Exclusive access across processes | Named `Mutex` | Kernel object, cross-process by name |
| Limit concurrent access to N | `Semaphore` | Built-in count; no ownership requirement |
| Signal "event occurred" to one waiter | Auto-reset `Event` | Exactly one thread wakes per `SetEvent` |
| Broadcast "event occurred" to all | Manual-reset `Event` | All waiters released until `ResetEvent` |
| Time-based trigger | `WaitableTimer` | Relative or absolute, one-shot or periodic |
| Amortize thread creation cost | Thread pool | Reuses threads; idle threads consume no CPU |

---

*Study notes based on 윤성우, 《윈도우즈 시스템 프로그래밍》 (book + lectures).*
