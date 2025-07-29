# Asynchronous I/O 

Video Player program, recieve data and play principle 

```
|receive| play  |receive| play  |receive|
| data  | video | data  | video | data  |
```

This is kind of exaggarated, but in this case CPU usage will have 
drastic change during play video and receive data. 

Since CPU will NOT work while receiving data, 
because I/O Input/Output of file is nearly 
CPU independent calculation. 

ANSI standard I/O function usage to read or write file data 
will be in blocking status. These functions waiting until 
I/O finish is **Blocking** Function, and these blocking function 
based I/O is called as **Synchronous I/O**. 

If you launch editor file that should load quite big file, there might be 
subtle Delay during opening such a large file. 
However, in video player perspective it is crucial problem to feel delay in that way. 

Data retrieve does not require CPU allocation that much. 
Thus during play even though I/O work concurrently there won't be big problem. 
So continue I/O, while play buffered data would siginficantly reduce latency. 
This concurrently I/O with execution of CPU functions is called as 
**Asynchronous I/O**. 

## Overlapped I/O 

ANSI standard function `fread` reads data from file. 

However, it is ANSI, so it is synchronous function. 
Meaning that function `fread` will be blocking function until I/O is finished. 
Suppose loading a music to play it during loading more larger file. ANSI based 
synchronous music file read will result in music began to be played after 
all files are loaded, which is far from expected consequence. 


The problem is **Blocking** function. Most functions that were observed, 
are blocking based like this. When function is called, it is in blocking status 
until it is completed, so two different works, file loading and music play 
do NOT work simultaneously unlike intention, since 
function call to receive data followed by function call of music player is impossible. 

This is synchronous I/O, function blocking let play function called after fread is returned. 

```
fread call     fread return 
+              +
|--------------|
| Receive Data |
|--------------|------------~
               | Sound Play ~
               |------------~
               +            
               play call    
```

Thus, to read data and play music simultaneously, function `fread` should be 
**Non-Blocking**. 

Blocking function is returned after completed, 
Non-Blocking function will return regardless of function completion! 
Moreover, even after return, function keep operation. 

ANSI standard does NOT contain concept of Non-Blocking, so `fread` cannot work in Non-Blocking way in default. 
However, Windows System functions like `ReadFileEx` can be operated in Non-Blocking. 

This is asynchronous Overlapped I/O,  function Non-Blocking so play music function 
wait for a short period of time for `ReadFile` return while function `ReadFile` operates after return. 

```
ReadFile       
call     
|    ReadFile
|    Retuen   
+    +       
|--------------|
| Receive Data |
|----------------------------~
     | Sound Play           ~
     |-----------------------~
     +           
    play call    
```

This kind of asynchronous file reading / writing functions that continue execution after return, 
can be literally overlapped multiple times.  

```
+----------------------------+
|        Receive Data        |
+---+------------------------+---+
    |        Receive Data        |
    +---+------------------------+---+
return  |        Receive Data        |
        +----------------------------+
    return
```

This is called as **Overlapped I/O**. I/O calculation might be literally overlapped multiple times. 
