# Asynchronous I/O 

File Input Output 

Relationship between I O and CPU Clock 

CPU Clock increment generally represents higher speed. 

IO has very high BUS Clock independency. 
But generally if CPU clock is higher then BUS clock is also higher 

CPU and CPU Clock  and I O Clock 

Relationship between System Clock and I/O 

Suppose System A and B exist 
A System Clock 100 Destination 
B System Clock 200 Destinatino

Faster clock means faster IO? Generally correct, 
but not always. 

File / Network / Console / etc Buffering 
IO Buffer 

If you send data altogether then 

Suppose A Buffer and B Buffer exist 
we can decide Buffer delete policy, but let's suppose that 
Once per 10 clocks buffer is erased. 

Then, System B will get flushed 20 times per second 
while System A will get flushed 10 times per second 

Even though System B is slightly quicker to generate data, 

System A will vacate buffer when a b c d 4 elements are in buffer while 
System B will vacate buffer when only a b or c d 2 elements are in buffer 

Since A combines all elements and send them altogether, they have 
Only One communication signal 
while System B sending smaller elements per communication signal 
use more communication signal per limited abount of time. 

CPU Clock 100 200 is not that much important 
File IO is slow, but Network TCP IP telecommunication is way much slower 
So using more network protocol would represent slower speed, since 
that time is independent from CPU clock, so in worst case 
higher clock CPU gets 3 times lower than normal clock CPU. 

Since I/O, especially compared to Network Communication, 
is relatively not necessarily that big. 

## What is Asynchronous I/O? 

When Write is called data transfer begins 
When Write is returned data transfer ends 
This is Synchronous I/O 

Then Write function call is synchronous, 
but if return time does NOT represent end of I/O, 
it is asynchronous I/O 

Even though I/O function is returned, 
I/O calculation is independent and not that much CPU dependent. 
IO device, handling unit, and actual CPU in calculation have divided role for each. 
So I/O function progress can be concurrent with CPU working for other assembly command  


About 60% percent or just above average calculations are I/O. 
So synchronous IO shows inefficient, fluctuating CPU resoruce usage while 
asynchronous IO shows relatively stable and efficient CPU resource usage. 


Two Types to control Async IO 

- Overlapped I/O 
- I/O Completion Routine 

## Overlapped I/O 

Literally overlap I O 

Read function call starts I.O 
then return first while reading still has over 90% of runtime to finish 

Simultaneous I/O does Not consume that high resource. 

B - A - C Suppose Network Communication 
A B inter communication process bandwidth is allocated quite a lot. 

CPU internally wait quite long for a while for IO 
So it is better to wait 2 or more overlapped IO, than just wait 1 IO 
to increase performance espeically quantity of IO is required. 

After IO there would be saving process 
A B C each complete I/O then should check whether I O is complete. 


## Completion Routine 

A IO 
B IO 
C IO 

Routine Control in center 

Routine D 
Routine E 
Routine F 