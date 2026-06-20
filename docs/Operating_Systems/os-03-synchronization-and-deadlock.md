---
title: "Synchronization and Deadlock"
---

# Synchronization and Deadlock

Concurrent threads share memory and devices. That sharing is precisely what makes them useful — and precisely what makes them dangerous. This post covers the core problem (race conditions), the concept that solves it (critical sections and mutual exclusion), the three main primitives that enforce that concept, the deeper essence of what synchronization is trying to achieve, and finally deadlock: when the cure becomes the disease.

---

## Race Conditions

A **race condition** occurs when the correctness of a program depends on the relative timing of two or more threads, and that timing is not controlled.

The canonical example is a shared counter:

```cpp
// Thread A and Thread B both execute this:
g_count++;
```

`g_count++` looks like one operation, but it compiles to three:

```cpp
// 1. Load g_count into a register
// 2. Increment the register
// 3. Store the register back to g_count
```

If Thread A reads `g_count = 5`, gets preempted after step 1, Thread B then reads `5`, increments to `6`, stores `6`, and Thread A resumes and also stores `6` — the net result is `6` instead of `7`. One increment was silently lost. This is a race condition: the outcome depends on who wins the CPU race.

The problem is not that threads run in parallel — it is that a logically atomic operation is not physically atomic. The scheduler can interrupt between any two machine instructions.

---

## Critical Sections and Mutual Exclusion

A **critical section** is any region of code that accesses a shared resource in a way that must not be interleaved with another thread doing the same. In the example above, the three-instruction sequence is the critical section.

**Mutual exclusion** (mutex) is the property that at most one thread executes a critical section at any given time. It is the minimal correctness requirement for shared-state concurrency.

Three classical requirements for a correct mutual exclusion solution:

1. **Mutual exclusion** — at most one thread inside the critical section at a time.
2. **Progress** — if no thread is in the critical section, a thread that wants to enter must eventually be allowed in (no indefinite blocking by outsiders).
3. **Bounded waiting** — after a thread requests entry, there is a finite upper bound on how many times other threads may enter before it does (no starvation).

Software-only solutions (Peterson's algorithm, Dekker's algorithm) exist but are fragile on modern CPUs due to out-of-order execution and memory reordering. In practice, mutual exclusion is enforced with hardware-assisted primitives.

---

## The Three Core Primitives

### Mutex (Mutual Exclusion Lock)

A mutex is the direct implementation of the critical section concept. It has two operations — lock (acquire) and unlock (release) — and one invariant: only the thread that locked it may unlock it.

```cpp
mutex.lock();
// --- critical section ---
g_count++;
// --- end critical section ---
mutex.unlock();
```

When a thread calls `lock()` and the mutex is already held, the thread **blocks**: it is removed from the run queue and does not consume CPU until the mutex becomes available. This is what distinguishes a mutex from a spinlock (which busy-waits in a loop). Blocking gives up the CPU; spinning keeps it.

**Ownership** is the key property of a mutex: a mutex is held by exactly one thread, and that thread is responsible for releasing it.

### Semaphore

A semaphore generalizes the mutex. It holds an integer counter and exposes two operations:

- **wait** (also called P, or `acquire`): decrement the counter; if the result is negative, block.
- **signal** (also called V, or `release`): increment the counter; if any threads are waiting, wake one.

A **binary semaphore** (counter initialized to 1) behaves like a mutex. A **counting semaphore** (counter initialized to N) allows up to N threads to proceed concurrently — useful for resource pools (e.g., "at most 4 database connections at a time").

The critical difference from a mutex: a semaphore has no ownership. One thread can call `wait`, and a *different* thread can call `signal`. This makes semaphores useful for **signaling** (producer–consumer) rather than just mutual exclusion.

```cpp
// Producer
produce_item();
sem.signal();    // announce that an item is ready

// Consumer
sem.wait();      // block until an item exists
consume_item();
```

### Condition Variable

A condition variable allows a thread to atomically **release a mutex and block**, waiting until another thread signals that a condition has changed. It is always used in conjunction with a mutex.

```cpp
mutex.lock();
while (!condition_is_true()) {
    cond.wait(mutex);   // atomically releases mutex, blocks, re-acquires on wake
}
// proceed — condition is now true
mutex.unlock();
```

The `while` (not `if`) is essential: spurious wakeups are allowed by the specification, and between when a thread is woken and when it re-acquires the mutex, another thread may have consumed the resource. Always re-check the condition.

Condition variables are the standard building block for **bounded buffers**, **thread pools**, and any pattern where a thread must wait for a state change rather than just for exclusive access.

---

## The Essence of Synchronization

Synchronization is not primarily about locks. It is about two deeper guarantees:

1. **Atomicity** — a sequence of operations that should appear indivisible does appear indivisible from all other threads.
2. **Visibility** — when one thread writes a value, other threads that need to see it actually do, in the correct order.

Modern CPUs execute instructions out of order and cache writes locally. Without explicit synchronization primitives, there is no guarantee that a value written by Thread A is visible to Thread B at all, let alone in program order. Synchronization primitives act as **memory barriers**: they tell the CPU and the compiler that reorderings across this point are not permitted.

This is why even a simple flag like `bool done = true` is not safe without synchronization across threads: the store may be reordered, buffered in a store queue, or cached in a way that Thread B never sees. The primitive is not just about mutual exclusion — it is about making the memory model coherent.

---

## Deadlock

Deadlock is what happens when synchronization is applied without discipline. Two or more threads each hold a resource and are each waiting for a resource held by the other. Neither can proceed. Neither will release. The system freezes.

### The Four Coffman Conditions

Deadlock can only occur if all four of the following conditions hold simultaneously:

1. **Mutual exclusion** — resources cannot be shared; only one thread at a time.
2. **Hold and wait** — a thread holds at least one resource while waiting to acquire others.
3. **No preemption** — resources cannot be forcibly taken; a thread must release them voluntarily.
4. **Circular wait** — there exists a cycle: Thread A waits for Thread B's resource, Thread B waits for Thread C's, ..., Thread N waits for Thread A's.

All four must hold. Breaking any one of them prevents deadlock.

### Breaking the Conditions

**Break mutual exclusion**: Make resources shareable. Only applicable to read-only resources (multiple threads can hold a read lock simultaneously). Not applicable to writable state.

**Break hold and wait**: Require threads to request all resources at once, before starting. If any resource is unavailable, the thread requests nothing and waits. This is conservative but safe. Alternatively, a thread that cannot acquire all resources releases the ones it already holds and retries.

**Break no preemption**: Allow the OS or runtime to forcibly revoke a resource from a thread. Practical for some resources (CPU time, locks with timeout), impractical for others (a half-written file).

**Break circular wait**: Impose a **global ordering** on all resources. Require every thread to acquire resources in that order. If Lock A always comes before Lock B in the ordering, no thread can hold B while waiting for A — the cycle is impossible by construction.

```cpp
// All threads must acquire locks in order: mutex_A before mutex_B.
// This makes circular wait structurally impossible.
mutex_A.lock();
mutex_B.lock();
// ... work ...
mutex_B.unlock();
mutex_A.unlock();
```

The global ordering approach is the most commonly used technique in practice: define a lock hierarchy, enforce it in code review, document it as a constraint.

### Deadlock Avoidance vs. Prevention

- **Prevention**: structurally rule out one of the four conditions (as above). Static, conservative, always safe.
- **Avoidance**: dynamically decide whether granting a request would lead to an unsafe state (e.g., Banker's Algorithm). Requires knowing maximum resource needs in advance — rarely practical in general-purpose software.
- **Detection and recovery**: allow deadlock to occur, detect it (cycle detection in the resource-allocation graph), then recover by aborting or rolling back one thread. Used in databases (deadlock victim selection), not typical in systems programming.

In most application code, **prevention via lock ordering** is the right tool: cheap, static, and eliminates the problem entirely.

---

*Study notes — Windows-specific material based on 윤성우, 《윈도우즈 시스템 프로그래밍》 (book + lectures).*
