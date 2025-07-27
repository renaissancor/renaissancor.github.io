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