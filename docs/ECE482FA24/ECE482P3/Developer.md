# Developer Guide 

Installation and Testing Guideline 

## Navigation
- [Home](Home.md)
- [Project Roadmap](RoadMap.md)
- [Risk Assessment](Risk.md)
- [Performance Analysis](Performance.md)
- [Developer Guide](Developer.md)

### Implement in user space

First of all, we implemented the idea of lottery scheduling in user space as our first demonstration.
Processes are assigned tickets based on their nice values.
The process with more tickets has a higher chance of being selected.

## One-Line eBPF

Focus on basic use cases like system monitoring calls
or capture process scheduling events. 

```shell
sudo bpftrace -e 'kprobe:schedule { printf("Scheduled Process: %s\\n", comm); }'
```

## Prerequisites 

- Linux Operating System 
  - `GCC` (GNU Compiler Collection) 
  - `make` for build automation
  - `git` for version control
  - Optional: `valgrind` for memory debugging

## Environment Setup 

Clone Repository 
```shell
$ git clone ssh://git@focs.ji.sjtu.edu.cn:2222/ece482-24fa/p3team-02.git  
$ cd p3team-02
```
Build Project 
```shell
$ cd src
$ make vmlinux.h 
$ make NAME=simple_scheduler obj
```
Start scheduler demo 
```shell
$ ./start.sh
$ sudo cat /sys/kernel/debug/tracing/trace_pipe
```
Stop scheduler demo 
```shell
$ ./stop.sh
```
Clean compiled files 
```shell
$ make NAME=simple_scheduler clean
$ make 
```
Then, recompile is successful. 

```shell
make vmlinux.h 
make NAME=simple_scheduler obj

sudo ./start.sh

sudo cat /sys/kernel/debug/tracing/trace_pipe

sudo ./stop.sh

make NAME=simple_scheduler clean
```

This will be final. 

## Debug and Test 

Use `trace_pipe` for process monitoring 
```shell
sudo cat /sys/kernel/debug/tracing/trace_pipe
```
Use debugging tools like `valgrind` for memory safety. 
```shell
valgrind ./start.sh
```
