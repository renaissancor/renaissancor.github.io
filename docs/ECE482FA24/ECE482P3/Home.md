# Project 3 Lottery Scheduling 

2024FA ECE4820 Introduction to Operating Systems

## Navigation
- [Home](Home.md)
- [Project Roadmap](RoadMap.md)
- [Risk Assessment](Risk.md)
- [Performance Analysis](Performance.md)
- [Developer Guide](Developer.md)



## Team Members, Roles and contributions

Jae Heung Park

- Documentation of `README` file and Wiki for project release.
- Supportive contribution to the project with effort

He Wentao

- Project basic setup from given source code and functions.
- Compiling and debugging codes to run test cases in local environment.
- Project structure design and plan

Wang Rui

- Project basic setup from given source code and functions.
- Compiling and debugging codes to run test cases in local environment.
- Project structure design and plan

### Overview of eBPF implementation 

For now, since we have get the idea of how to implement the lottery scheduler in user space. 
The next thing to do is to implement it in kernel space with eBPF. 
We referred to the websites and resources provided in wiki, especially in the `sched-ext` repo.
By reading through some sample codes in this repo and refer to what we have learned in the lab, 
we noticed that we need to implement `enqueue`, `dispatch` and `init` these three functions.

### Key Functions 

- `enqueue`: Adds tasks to dispatch queue.
- `dispatch`: Selects tasks for execution based on ticket values.
- `init`: Initializes the scheduler and shared dispatch queues.

```c
struct sched_ext_ops sched_ops = {
    .enqueue = (void *)sched_enqueue,
    .dispatch = (void *)sched_dispatch,
    .init = (void *)sched_init,
    .flags = SCX_OPS_ENQ_LAST | SCX_OPS_KEEP_BUILTIN_IDLE,
    .name = "p3_scheduler"};
```

### Ticket Management 

The first problem we met with was how to store all the ticket information. 
Since we cannot modify a structure called `task_struct`, 
so we use a map with hash to store all the ticket information:

```c
struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, __u32); // task UID
    __type(value, __u64);
    __uint(max_entries, 1024);
} task_tickets_map SEC(".maps");
```

### Shared Dispatch Queues

Next, we initialized two shared Dispatch Queue (DSQ).
A shared dispatch queue used to coordinate the scheduling of different tasks. 
It typically acts as a buffer for tasks or an intermediary for task scheduling,
allowing interaction between different schedulers or task producers and consumers.

```c
s32 BPF_STRUCT_OPS_SLEEPABLE(sched_init)
{
    bpf_printk("Scheduler initialized with DSQ ID: %d\n", SHARED_DSQ_ID);
    int create_1 = scx_bpf_create_dsq(SHARED_DSQ_ID, -1);
    int create_2 = scx_bpf_create_dsq(WINNER_DSQ_ID, -1);
    return create_1 && create_2;
}
```

As you can see, it's very simple to implement.

When enqueue tasks, we noticed that there is a variable called `enq_flags` 
and we think that it's used for signal the priority of different tasks. 
So we use it to help decide the number of tickets. 
Basically, we first generate a random number between 1 and 100 as the ticket number, 
then for high priority, we add 100 more; and for low priority, we minus 100. 
As you can see, the ticket number signals the priority.

When choosing which task to run, we get a random number, than find the corresponding task.

At last, we want to talk about something about discussion. 
First of all, we noticed that `bpf_task_from_pid` has different performance on different version of kernel. 
For example, it works fine on version 6.12 on ubuntu while it meets some problem on version 6.12 on arch. 
To avoid problems, we have to use multiple maps to store information like slice, task ID. 
But we eventually managed to make it work.

Another thing we want to discuss is about the ticket method. 
For now, we tried to implement `nice` with `enq_flags` variable to decide the ticket number. 
But we only had time to implement a rough one, it can be more precise.

Moreover, we noticed that our scheduler fails after running for a while. 
The performance is like the whole computer stucks and then our program was kicked out by the kernel.

