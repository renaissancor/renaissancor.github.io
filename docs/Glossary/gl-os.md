---
title: "Glossary: Operating Systems"
tags:
  - glossary
  - operating-systems
---

# Glossary: Operating Systems

The words you must recognize instantly, not the theory behind them.

### Processes and scheduling

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| PID / TID | Process / Thread ID | kernel identifiers | "which PID is holding the port?" |
| PCB / TCB | Process / Thread Control Block | kernel's record of a process or thread. TCB also means a TCP connection's block | |
| IPC | Inter-Process Communication | pipes, sockets, shared memory. In hardware talk, instructions per cycle | |
| FD | File Descriptor | integer handle for a file, socket or pipe | "we ran out of FDs" |
| Syscall | System Call | user → kernel entry | "that loop is one syscall per byte" |
| Context switch | | save one thread, load another | "too many context switches" |
| IRQ | Interrupt Request | device signalling a CPU | "all IRQs are pinned to core 0" |
| ISR | Interrupt Service Routine | code that handles an interrupt | |
| CFS | Completely Fair Scheduler | Linux scheduler before EEVDF | |
| EEVDF | Earliest Eligible Virtual Deadline First | replaced CFS in Linux 6.6 | |
| cgroup | control group | limits and accounts for the resources of a group of processes | "the container hit its cgroup memory limit" |
| Namespace | | isolated view of PIDs, network, mounts. Containers are built mainly from namespaces plus cgroups | |

### Memory

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| VA / PA | Virtual / Physical Address | what code uses / what RAM uses | |
| MMU | Memory Management Unit | hardware that translates addresses | |
| TLB | Translation Lookaside Buffer | cache of address translations | "TLB shootdown" |
| PTE | Page Table Entry | one virtual-to-physical mapping | |
| VMA | Virtual Memory Area | one mapped region of a process | |
| COW | Copy-On-Write | share pages until someone writes | "fork is cheap because of COW" |
| mmap | memory map | map a file or anonymous memory into the address space | |
| Page fault | | MMU can't complete the access: page not present or permission denied. Minor: no I/O. Major: required I/O | "major faults spiked" |
| THP | Transparent Huge Pages | kernel can back anonymous memory (and shmem where enabled) with huge pages without app changes, typically 2 MB on x86-64. Depends on the `enabled` setting | "disable THP for that database" |
| RSS | Resident Set Size | resident RAM mapped by a process, including pages shared with others. Not Receive Side Scaling | "RSS grew to 40 GB" |
| VSZ | Virtual Memory Size | address space mapped, mostly meaningless for sizing | |
| OOM | Out Of Memory | allocation failed, or the OOM killer chose a victim | "it got OOM-killed" |
| ASLR | Address Space Layout Randomization | randomizes stack, heap, library and (for PIE binaries) executable addresses against exploits | |
| DMA | Direct Memory Access | device reads or writes RAM without the CPU | |

### Concurrency

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| CAS | Compare-And-Swap | atomic conditional write | "CAS loop" |
| Futex | Fast Userspace Mutex | wait in the kernel only when contended | |
| RCU | Read-Copy-Update | readers take no locks; old versions are freed after pre-existing readers finish | |
| TLS | Thread-Local Storage | per-thread variables. Not Transport Layer Security | |
| TOCTOU | Time Of Check to Time Of Use | race between checking and acting | |

### I/O and files

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| epoll | event poll | Linux readiness notification | |
| io_uring | | Linux completion-based I/O through shared rings | |
| IOCP | I/O Completion Port | Windows completion-based I/O | |
| VFS | Virtual File System | common kernel interface over filesystems | |
| inode | index node (probable origin) | file metadata; a name points to it | "out of inodes with free disk left" |
| Page cache | | file data cached in RAM | "the second read came from page cache" |

### Binaries

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| ABI | Application Binary Interface | calling convention and layout at machine level | "that upgrade breaks the ABI" |
| ELF / PE | Executable and Linkable Format / Portable Executable | Unix-like (Linux) / Windows binary format | |
| GOT / PLT | Global Offset Table / Procedure Linkage Table | indirection for dynamic linking | |
| POSIX | Portable Operating System Interface | Unix-derived API, shell and utilities standard | |
