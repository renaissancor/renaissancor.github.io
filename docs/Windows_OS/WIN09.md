# Process Scheduling 

OS and Real Time OS Difference 

RTOS and normal OS main difference is response speed. 
RTOS is quicker in response than normal OS. 

Traditionally RTOS are Hard RTOS 

They are not only simply enhanced response 
but **Dead Line**. 

This happens in really time **Critical** circumstances. 
For instance, automobile systems that work 0.1 second slower 
will result in car accident and casulty. 

Preemptive OS and Non Preemptive OS 


## Round Robin Scheduling Algorithm 

For sampe priority classes, to maintain equity 형평성 
execute for only restricted amount of time and 
send CPU resource to other resource with same priority. 

The time unit for each execution is called as Quantum, or Time Slice. 
Same priority class will get CPU resource based on this Time Slice. 
If time slice is too long, then interactive system will not operate properly. 
Basically you move mouse to drag and drop cursor, but graphic will be 
displayed much slower. 

However, too short time slice is also problematic, because 
it means more context switching, meaning lower performance. 

```
|----------------|
|Priority 9 Queue| Highest Priority 
|Priority 8 Queue|     Processes 
|Priority 7 Queue|   _     _     _     _
|Priority 6 Queue|- |_| - |_| - |_| - |_|
|Priority 5 Queue|   -> Round Robin ->  
|Priority 4 Queue|
|Priority 3 Queue|    _
|Priority 2 Queue| - |_|
|Priority 1 Queue|
|Priority 0 Queue|  Lowest Priority 
|----------------|
``` 

### Scheduling Algorithm when scheduling happens? 

So when Scheduling happens ? 

1. Per each Time Slice, aka Quantum 
2. Process Creation and Termination 
3. Current Running Process in Blocking state 

### Priority Inversion 

Suppose Priority in this order 

Process A > Process B > Process C 

Process A has highest priority, so it is in progress. 

Process B is just looking for chance. 

Sunddenly Process A stops operation, because 
Process C should calculate some value and return it to Process A, 
but Process C have not sent it back. 

So, Process A will get into blocked phase to give Process C to work. 

Then, Process B will take CPU resource, taking Process C's time to work. 

So, Process A although has higher priority than Process B, should waint for Process B 
since Process C that Process A is blocked to get response has lower priority than 
Process B. 

The solution might be, instead of Process A just turn to blocked without responsibility 
it might rent its high priority to Process C and then gets blocked, so that 
Process B will not take CPU resource. 
