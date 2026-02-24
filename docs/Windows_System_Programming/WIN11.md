# Process VS Thread 

Divide Process ? 

If parent process create child processes, then 
they have their own independent code, data, and heap section. 

However, if thread is created, then only stack is independent 
while code, data, and heap are shared. 

Code 
Stack Share between Threads is not likely. Actually it is impossible. 

But, each stack will execute commands from same code section, 
but with different Program Counters (aka Instruction Registers). 

Functions : 
Global functions, and main function ? 
In thread, suppose that 
they have invisible independent main functions too. 

Share not only code section 
but also resources 

Suppose that m2 and m3 are thread main functions 
Since they share code section, 
m2 and m3 can access all functions existing in the code section of the process. 

Compared to creating child process, which cannot access parent process functions
because their code section is separated. 

## What is Kernel Level thread and User Level Thread? 

Very Important 

In Past, some OS did not support threads. Designed by single thread. 

Kernel level thread support 

**Kernel Level Thread** 

Kernel access directly to the thread since thread info is saved and managed by Kernel

```
+-----------User Area-------------+
|------------|
|  Process 1 | 
|  Thread A  | 
|  Thread B  |
|------------| 
+---------------------------------+

+----------Kernel Area------------+
|           | <---> Thread A Info |
|  Process  | <---> Thread B Info |
| Scheduler | <---> Thread C Info |
|           | <---> Thread D Info |
+---------------------------------+
```

If Scheduler have 100 Resources, 
and distribute it to 5 threads equally, 
it would be distributed 20 for each threads. 

**User Level Thread** 

Kernel access Process, and from there access threads inside process 
based on the information provided by the process. 

So, Kernel has Process info, but not thread info. 
Each tread info is inside Process. 

```
+-----------User Area-------------+
|------------|
|  Process 1 | 
|  Thread A  | 
|  Thread B  |
|------------| 
+---------------------------------+

+----------Kernel Area-------------+
|           | <---> Process 1 Info |
|  Process  | <---> Process 2 Info |
| Scheduler | <---> Process 3 Info |
|           | <---> Process 4 Info |
+----------------------------------+
```

If Scheduler have 100 Resources, 
and distribute it to 2 process equally, 
and process A has 1 thread while other process B has 4 threads, 
it would be distributed 50 50 for each process, and 
process B will redistribute thread running time 
so that each 1 of 4 threads in process B have 12.5 
while 1 thread in process A gets 50 resource. 
This is just example for understanding, real OS will work more efficiently than this. 

### Kernel Level Thread vs User Level Thread 

For Kernel Level Thread 
In Kernel perspective, 
CPU resource is directly distributed to each thread without passing process. 
If kernel level thread A is blocked by I/O then scheduler will send 
priority to the thread B or thread c. 

For User Level Thread 
In Kernel Perspective, CPU resource is redistributed again by process. 
Suppose User level thread A and B are in Process A. 
If user level thread A is blocked by I/O then scheduler will not 
detect or figure out what thread A or B inside process A is blocked but 
only figure out that process A is interrupted. 
Thus, scheduler will assign resource to process C, which is not inside process A. 
Even though user level thread B inside process A is not blocked and cannot run, 
scheduler will not assign resource to thread B becaue it only detects 
that process A is blocked, without information about threads status inside process A. 

### Kernel Mdoe and User Mode 

Program is ran by 
OS and application. 
In embedded system it is OS + App. 

Suppose that Memory is divided into 
Kernel Space and User space. 
```
            ^ |--------| ^
            | |  User  | | User Mode 
        All | |  Area  | | Accessible 
 Accessible | |--------| v
  in Kernel |
       Mode | |--------| ^
            | | Kernel | | User Mode 
            | |  Area  | | Unaccessible 
            v |--------| v
```

In Windows Perspective, 
both User area and kernel area should run. 

Also, user space might potentially access to kernel area in memory. 
However, kernel is afraid of user area to access kernel area and 
change very important logic of kernel core logic. 

Thus, kernel level thread is slow, because 
inter thread context switching between kernel level threads 
represents mode change from user mode to kernel mode. 

This is why kernel level thread context switching is slower, 
because it requires thread to change from user mode to kernel mode. 

However,for user level thread, since process manages user level threads, 
context switching  between user level threads inside same process will 
not require mode change from user to kernel and from kernel to user. 
Kernel level context switching will happen only in process perspective.   

So in summery

**Kernel Level Thread** 

PROS : Kernel direct accessed thread, safety and several functions 

CONS : context switching will involve user mode to kernel mode 

**User Level Thread**
PROS: User mode to Kernel mode context switching is unnecessary 

CONS: process level blocking, if one of several threads inside process is blocked 
entire process and all other threads inside will be blocked too. 