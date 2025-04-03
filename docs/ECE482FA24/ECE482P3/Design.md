# System Design

Software design schema structure overview and description


## Navigation

- [Home](Home.md)
- [Project Roadmap](RoadMap.md)
- [Risk Assessment](Risk.md)
- [Performance Analysis](Performance.md)
- [Developer Guide](Developer.md)


## Software Architecture Overview

The project implements a lottery scheduling algorithm in kernel space using eBPF. 
This section provides an overview of the system's key components, workflow, and design considerations. 
The architecture leverages modern kernel technologies to achieve a balance of performance and extensibility.


## System Workflow 

- Run `src/start.sh` to start the project. 
- Queue and manage processes by shared dispatch queues (DSQ) 
- Allocate and schedule tickets by custom scheduler 
- Use kernel probes and eBPF tracing tools to monitor task scheduling and performance in real time.  


## Lottery Scheduling Algorithm

1. Assign tickets to each process based on priority.
2. Build a ticket pool with entries proportional to ticket counts.
3. Select a random ticket to choose the next process to run.
4. Allocate a time slice to the chosen process.
5. Re queue or terminate the process based on its state. 


## Key Components

### Bootloader

A bootloader is a small program that loads the operating system (OS)
into memory when a computer or device is powered on or reset.
It acts as the initial step in the booting process,
ensuring that the necessary system files are loaded for the OS to function properly.


### eBPF (Extended Berkeley Packet Filter)

eBPF is an advanced kernel technology that allows users to run secure and high-performance
programmable code within the operating system kernel.
Originally, eBPF was designed for network packet filtering.
However, it continuously evolved to support use cases in
monitoring, security, networking, and performance optimization.
eBPF programs are verified and loaded into the kernel, enabling functionality extension
without modifying the kernel source code, all while maintaining low overhead and high security.

## eBPF Events monitoring

Following `bpftrace` command can be used to monitor scheduling events in real time

```shell
sudo bpftrace -e 'kprobe:schedule { printf("Scheduled Process : %s\\n", comm); }'
```

`kprobe:schedule` attach kernel probe to `schedule` function in kernel.

This single line can leverage eBPF to monitor events.  


