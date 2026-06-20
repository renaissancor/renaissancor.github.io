---
title: "Processes and Threads"
---

# Processes and Threads

Modern operating systems multiplex a single CPU across many concurrent activities. The two fundamental units of that multiplexing are the **process** and the **thread**. Understanding what each one *is* — its address space, its kernel structures, its cost — is the foundation for every higher-level topic in systems programming.

---

## What is a Process?

A process is an isolated execution environment: a running program together with all the resources the OS has allocated to it.

The kernel tracks every process with a **Process Control Block (PCB)**. The PCB holds:

- the virtual address space map (code, data, heap, stack regions),
- the open handle/file-descriptor table,
- security tokens and ownership information,
- pointers to the process's threads.

Because each process owns a *private* virtual address space, one process cannot read or write another's memory without an explicit OS-mediated mechanism. This isolation is the defining property of a process — it is both its chief safety guarantee and its chief cost.

---

## What is a Thread?

A thread is the unit of *execution* scheduled by the OS. Every process contains at least one thread, and can contain many.

Where processes are isolated, threads within the same process **share**:

- the code section,
- the global data section,
- the heap.

Each thread has its own:

- **stack** — local variables and call frames; sharing stacks is impossible,
- **program counter / instruction pointer** — its position in the code,
- **register set** — the CPU state at any given moment.

Because threads share the code section, any thread can call any function defined in the process. Child *processes*, by contrast, get a separate copy of the code section and cannot call the parent's functions directly.

---

## Kernel Structures: PCB, TCB, TEB

Three data structures together describe a running thread's full state.

### PCB — Process Control Block
Owned and managed by the kernel. Contains process-wide resources: address-space metadata, open handle tables, security context. One PCB per process.

### TCB — Thread Control Block
Owned and managed by the kernel. Contains per-thread scheduling state: thread priority, quantum remaining, register context saved at the last context switch, pointer to the kernel stack. One TCB per thread.

### TEB — Thread Environment Block
Lives in **user-mode memory**, so user-mode code can read it without a syscall. Contains the thread ID, the base address of the thread's Thread Local Storage (TLS) slots, and other runtime support data.

On 64-bit Windows the **GS segment register** is permanently pointed at the current thread's TEB. Assembly stubs and the runtime use `gs:[offset]` to reach thread-specific data without any indirection through the kernel.

```
User Mode
  ┌─────────────────────────────┐
  │  TEB  (GS points here)      │  ← Thread ID, TLS base, …
  │  User Stack                 │  ← local variables, call frames
  └─────────────────────────────┘

Kernel Mode
  ┌─────────────────────────────┐
  │  TCB  (per thread)          │  ← register context, priority, quantum
  │  Kernel Stack (per thread)  │  ← separate; needed for kernel re-entry
  │  PCB  (per process)         │  ← address-space map, handles
  └─────────────────────────────┘
```

Every thread also has a dedicated **kernel stack** (typically 4–12 KB). Because a context switch can occur even while the thread is executing kernel-mode code, the kernel cannot use a shared stack; each thread must have its own place to save its kernel-mode state.

---

## Thread Local Storage (TLS)

Global variables are shared across all threads in a process. TLS provides the escape hatch: a variable declared in TLS has one independent copy per thread, invisible to all other threads.

The TEB's TLS pointer is the fast path to that per-thread storage. The C runtime (`_beginthreadex` / the CRT startup) initializes TLS slots before the thread's entry function runs, which is one reason to prefer CRT thread creation functions over raw OS calls when using the standard library.

A typical use case is a per-thread error code (`errno` in the CRT, for example) — each thread reads and writes its own copy without locks.

---

## Fibers: User-Mode Threads

Windows also exposes **Fibers** — execution contexts that look like threads to the code running in them, but are scheduled entirely by the *application*, not the OS.

Key properties:

- The OS knows nothing about fibers; the scheduler sees only the kernel thread that hosts them.
- Switching between fibers is **cooperative** (the running fiber must explicitly yield).
- A context switch between fibers is cheap: it is a pure user-mode register save/restore, with no kernel transition.
- Fibers are vulnerable to the same blocking problem as user-level threads generally: if the host thread blocks on a kernel wait (I/O, mutex), *all* fibers on that thread block with it.

Fibers predate modern async/await patterns and are rarely the right tool today, but they illustrate the broader principle of user-mode scheduling.

---

## Kernel-Level vs. User-Level Threads

The distinction between kernel-level and user-level threads is architectural, not Windows-specific.

| Property | Kernel-Level Thread | User-Level Thread |
|---|---|---|
| Scheduler visibility | Kernel sees each thread individually | Kernel sees only the process |
| I/O block behavior | Only the blocked thread stops; others run | Entire process blocks if any thread blocks |
| Context switch cost | Higher — requires user↔kernel mode transition | Lower — no mode switch within the process |
| Parallelism on multi-core | True parallel execution across cores | Limited to one core per process (without hybrid mapping) |

**Kernel-level threads** are what Windows and Linux implement natively. The kernel's scheduler distributes CPU time directly to threads; if thread A blocks on I/O, thread B in the same process can immediately receive the CPU.

**User-level threads** (green threads, fibers, coroutines) live entirely in user space. The process manages its own internal schedule. The advantage is cheaper switching; the cost is that a single blocking syscall stalls all user-level threads in that process because the kernel only sees one schedulable entity.

---

## Process vs. Thread: The Cost Dimension

The performance difference between process-level and thread-level context switching follows directly from address-space ownership.

**Switching between two threads in the *same* process:**

- Save/restore register set (TCB swap).
- No address-space change — page tables stay the same.
- TLB and CPU cache remain warm.
- Relatively cheap.

**Switching between threads in *different* processes:**

- Save/restore register set.
- Load new CR3 (page-table base) — the entire virtual address mapping changes.
- **TLB flush**: cached virtual→physical translations are now invalid.
- **Cache pressure**: the new process's working set is cold; L1/L2 misses spike.
- Significantly more expensive.

This cost asymmetry is the primary reason multi-threaded designs are preferred over multi-process designs for tightly coupled work: threads amortize the address-space overhead across all their parallelism.

The flip side is isolation: a buggy thread can corrupt the entire process heap; a buggy child process cannot touch the parent. The choice between processes and threads is always a trade-off between isolation and performance.

---

## Kernel vs. User Mode: Why It Matters for Thread Switching

The CPU operates in (at least) two privilege rings:

- **User mode (Ring 3):** restricted instructions; cannot access kernel memory.
- **Kernel mode (Ring 0):** unrestricted; full access to hardware and kernel data.

A **kernel mode switch** (also called a mode transition) happens on every syscall or hardware interrupt. The *thread* does not change — it is still running — but its privilege level rises to Ring 0 so it can execute kernel code. The `SYSCALL` / `INT 2E` instruction triggers this transition.

A **context switch** is different: the *executing thread* changes. The Dispatcher (a software-interrupt routine in the Windows kernel) saves the current TCB and loads another. Context switches happen when a thread's quantum expires or when it voluntarily blocks.

Both cost something. A kernel mode switch costs a register save and a privilege-level change. A context switch costs that plus, potentially, the full address-space overhead described above.

---

*Study notes — Windows-specific material based on 윤성우, 《윈도우즈 시스템 프로그래밍》 (book + lectures).*
