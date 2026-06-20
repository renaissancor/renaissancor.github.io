---
title: "Scheduling and Interrupts"
---

# Scheduling and Interrupts

Modern operating systems give the illusion that many programs run simultaneously on a fixed number of CPU cores. Two mechanisms make this work: **scheduling** (deciding who runs next and for how long) and **interrupts** (signals that force the CPU's attention away from what it is currently doing). Understanding both at the conceptual level is prerequisite to reasoning about any concurrent system.

---

## Preemptive vs. Non-Preemptive Scheduling

The first design question for a scheduler is: who holds the power to take the CPU away from a running thread?

**Non-preemptive (cooperative) scheduling** leaves that decision to the thread itself. A thread runs until it voluntarily yields — by calling `sleep`, blocking on I/O, or explicitly surrendering the CPU. The risk is obvious: a single misbehaving thread can starve everything else indefinitely.

**Preemptive scheduling** gives that power to the OS. The kernel can forcibly suspend any thread at any moment and hand the CPU to another. All mainstream general-purpose operating systems (Windows, Linux, macOS) are preemptive. This is not merely a performance choice; it is a safety requirement. Without preemption, a buggy or malicious thread could make the entire system unresponsive.

### Real-Time OS (RTOS)

A related distinction worth noting: a Real-Time OS is not simply a "faster" OS. An RTOS provides **deadline guarantees** — a task must complete within a bounded time, not just eventually. Hard-RTOS systems are used where missing a deadline has physical consequences (automotive braking, avionics). General-purpose preemptive schedulers optimise for fairness and throughput, not worst-case latency bounds.

---

## Quanta and Round Robin

Once preemption exists, the scheduler needs a unit of time to work with: the **quantum** (also called a time slice). A quantum is the maximum continuous time a thread may hold a CPU core before the scheduler can reclaim it — typically in the range of 15–20 ms on desktop operating systems.

**Round Robin** is the policy applied within a single priority level. Threads at the same priority form a queue. Each gets one quantum in turn; after its quantum expires (or it blocks voluntarily) it moves to the back of the queue. This ensures that no thread at a given priority can monopolise the core.

Key trade-off with quantum size:

- **Too long**: interactive responsiveness degrades — the user moves a mouse cursor but the display lags because the running thread is not preempted quickly enough.
- **Too short**: context switches become too frequent, and the overhead of switching (saving and restoring CPU state) begins to dominate useful work.

---

## Multi-Level Priority Queues

Real schedulers extend Round Robin across priority levels. The ready queue is split into bands — higher-priority bands always drain before lower ones are served (strict priority preemption). Within a band, threads rotate under Round Robin.

```
| Priority 15 | ← real-time / highest
| Priority 14 |
   ...
| Priority  6 | ← [A] → [B] → [C] → (round-robin within level)
   ...
| Priority  1 |
| Priority  0 | ← idle
```

A high-priority thread therefore does not just get more time; it gets to run *before* anything at a lower priority, regardless of how long those lower threads have been waiting.

---

## Starvation and Priority Boosting

Pure priority scheduling introduces **starvation**: a low-priority thread may never run if higher-priority threads continuously occupy the CPU. The standard remedy is **priority boosting** (also called aging).

The OS periodically scans the ready queue for threads that have been waiting too long. When a thread exceeds a starvation threshold (e.g., ~4 seconds), its effective priority is temporarily raised to ensure it gets a quantum. After execution, priority returns to the original value. The scan itself is bounded — only a fixed number of threads are examined and boosted per pass — to prevent the anti-starvation mechanism from itself imposing unbounded overhead.

---

## Priority Inversion

Priority inversion is a subtle scheduling hazard that arises from lock contention:

1. Thread **C** (low priority) acquires a mutex.
2. Thread **A** (high priority) tries to acquire the same mutex and blocks, waiting for C.
3. Thread **B** (medium priority) becomes runnable. Because C is lower priority than B, and A is blocked, **B runs and delays C** — which in turn delays A.

Result: the highest-priority thread is indirectly blocked by a medium-priority thread, inverting the intended priority order.

The standard fix is **priority inheritance**: when a high-priority thread blocks on a lock held by a lower-priority thread, the OS temporarily elevates the lock-holder to the blocker's priority. This prevents medium-priority threads from cutting in and allows the lock-holder to complete quickly and release.

---

## When Does Scheduling Occur?

The scheduler is invoked at well-defined points:

1. **Quantum expiry** — the hardware timer fires after one time slice.
2. **Thread creation or termination** — the ready queue changes.
3. **Blocking** — a thread calls a blocking operation (I/O, sleep, mutex wait); it voluntarily leaves the running state.
4. **Unblocking** — a waiting thread's condition is satisfied; it re-enters the ready queue and may preempt the current thread if its priority is higher.

---

## Context Switching

The **context switch** is the mechanical act of replacing one running thread with another. The component that performs it is called the **dispatcher**.

When a switch is triggered, the dispatcher:

1. Saves the current CPU register set (program counter, stack pointer, general-purpose registers, flags) into the outgoing thread's **Thread Context Block (TCB)**.
2. Selects the next thread from the ready queue (per the scheduling policy).
3. Restores the incoming thread's register set from its TCB.
4. Resumes execution at the instruction where the incoming thread last paused — which is inside the dispatcher itself, from that thread's point of view.

The thread has no awareness of the interruption; it simply continues from where it stopped.

### Intra-process vs. Inter-process Cost

- **Switching between threads in the same process**: cheap. The virtual memory mapping (page tables) is shared, so the CPU's address-translation cache (TLB) largely remains valid.
- **Switching between processes**: expensive. The page-table base register must be reloaded, flushing the TLB. The next memory accesses cause TLB misses, and the CPU caches that held the old process's working set are no longer warm. This can cost thousands of cycles.

The performance implication is direct: fine-grained inter-process communication is far more costly than equivalent intra-process threading.

---

## Interrupts

An **interrupt** is an asynchronous signal that forces the CPU to suspend its current instruction stream and transfer control to a dedicated handler. Interrupts are the primary mechanism by which the outside world — hardware devices, timers, other CPUs — communicates with the kernel.

### Hardware vs. Software Interrupts

| Type | Source | Examples |
|------|--------|---------|
| **Hardware interrupt (IRQ)** | External device | NIC packet arrival, disk DMA completion, keyboard input, timer tick |
| **Software interrupt / trap** | CPU instruction or exception | System call (`int 0x80`, `syscall`), divide-by-zero, page fault |

A **software interrupt** raised deliberately by the kernel (e.g., to trigger a context switch) is sometimes called a **dispatch interrupt** or **reschedule IPC**.

### Interrupt Handling Flow

1. The CPU finishes the current instruction (or reaches an interruptible boundary).
2. It saves minimal state (return address, flags) onto the current stack.
3. It looks up the **Interrupt Vector Table** (IVT / IDT) using the interrupt number.
4. It jumps to the corresponding **Interrupt Service Routine (ISR)**.
5. The ISR handles the event with minimal latency — acknowledging the device, queuing data, setting flags.
6. A **Deferred Procedure Call (DPC)** or equivalent mechanism handles heavier processing outside the ISR context.
7. The ISR executes a return-from-interrupt instruction; the CPU restores saved state and resumes the interrupted code.

### Interrupt Priority Levels (IRQL)

Not all interrupts are equal. The kernel maintains a hardware-enforced **Interrupt Request Level (IRQL)**. An ISR running at a given IRQL can only itself be interrupted by an interrupt at a strictly higher level; lower-level interrupts are masked (deferred) until the IRQL drops.

This hierarchy matters for memory: ISRs and DPCs run at levels where a page fault **cannot** be handled (the page-fault handler runs at a lower level). Any kernel code executing at these elevated levels must operate exclusively from physically resident, non-pageable memory. Violating this causes an immediate kernel panic.

### Interrupts as the Heartbeat of Scheduling

The **timer interrupt** is the scheduler's clock. The hardware timer fires at a fixed frequency; each tick delivers an interrupt to the CPU. The kernel's timer ISR decrements the current thread's remaining quantum. When the quantum reaches zero, the ISR (or DPC) sets a flag requesting a reschedule. At the next safe point, the dispatcher runs and performs the context switch.

Without timer interrupts, preemptive scheduling is impossible: there would be no reliable mechanism to reclaim the CPU from a running thread.

### Message-Signalled Interrupts (MSI)

Traditional interrupts used dedicated electrical pins on the bus — one wire per interrupt line, with a shared controller arbitrating them. Modern hardware uses **Message Signalled Interrupts**: the device writes a small message to a specific memory address to signal the interrupt. This removes the pin bottleneck, allows precise routing to a chosen CPU core, and supports many more interrupt vectors. Network cards using MSI can distribute packet-receive interrupts across multiple cores (**Receive Side Scaling**, RSS), enabling true parallel I/O processing.

---

## The Feedback Loop: Interrupts Drive Scheduling

Interrupts and scheduling are not independent subsystems — they are tightly coupled:

- The **timer interrupt** expires quanta and triggers reschedules.
- **I/O completion interrupts** unblock waiting threads, potentially inserting a high-priority thread into the ready queue that immediately preempts the current one.
- The **dispatcher** itself may raise a software interrupt to defer a context switch to the right kernel boundary.

Understanding this feedback loop explains why CPU-bound workloads that never block behave differently from I/O-bound workloads that frequently yield: the latter naturally integrate with the interrupt-driven scheduler, returning the CPU cooperatively on every I/O operation, while the former relies entirely on quantum expiry.

---

*Study notes — Windows-specific material based on 윤성우, 《윈도우즈 시스템 프로그래밍》 (book + lectures).*
