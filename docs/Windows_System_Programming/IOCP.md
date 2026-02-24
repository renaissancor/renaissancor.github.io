# Overlapped I/O 

File IO 

Request OS to read. 

Write is way much slower than read. 
File uses Disk, first its size is 0 and its size increments. 

File Pointer increases more and more. 

Relative and absolute position 
Start Point A B 
However if it is absolute pointer 
first address is the absolute location of starting file, and size. 

fopen_s does NOT make overlapped IO useful, but use WINAPI 
CreateFileA 
-> Open File + Create File Both 

CreateFile function 
CreateFile("TestFile.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL); 

"w" mod 
File mods 
FILE_FLAG_OVERLAPPED this is necessary option. 


## IOCP 

IOCP is, very fast I O Model. 
Why fast? OS manage and interfere. 
I/O is done by OS, not process. 
Proactor works by waiting.

IOCP is I/O callback based. 

100 File -> 100 Threads is inefficient. 

Thread 10 File IO resposible Threads, 
many threads means context switching. 

Even thread pool is managed by OS. 

Virtual Memory system efficient use, prevent 
inefficient memory copy. 

I/O handle related memory, page based lock and unlock. 
User mode application request lock and unlock by page. 

User mode to kernel mode function call means context switching and same 
for vice versa. 

IOCP already generates threads before even socket open. 
OS wake up thread and make socket overlapped IO. 

Get user mode pointer and request to IOCP Queue, OS locks page 4KB 
and complete I/O there. 

NIC DMA and just write to user mode destination, 
no need to copy and paste from kernel buffer to user level. 

socket bind listen accept ... 

accept inside  
recv < send // echo 
send > recv // echo
shutdown <> shutdown 
closesocket <> closesocket 

IOCP 
CreateIOCP means 
listen socket, client sockets (several), 
make several threads, based on system. 
CreateIOCP 
make I/O complete thread automatically 
WSASocket() also initialized, 
then bind() and listen() and accept() 

CreateIOCP() is called again for second times. 
This time it means observe these sockets. 
WSARecv(); function is called. it is same as ReadFileEx(); 
I/O pending, server waits. 
GQCS GetQueuedCompletionStatus 

IOCP Queue automatically alert IO waiting threads to work by GQCS 
GetQueuedCompletionStatus 
