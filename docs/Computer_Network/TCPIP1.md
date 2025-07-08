# Network Prgramming and Socket

## Run code in Linux, based on WSL environment 

1 Open WSL Terminal and create files 


2 Compile Both Programs 

```shell
gcc -o server hello_server.c
gcc -o client hello_clinet.c
```

3 Run Server 

```shell
./server 1234 
```

This starts the server and listens on port `1234`. Leave this running.

4 Open Another Terminal for the Client

```shell
./clinet 127.0.0.1 1234 
```

This connects to the server running on the same machine (localhost, 127.0.0.1).

5 If you hit a permissions error (e.g. "Permission denied") 

Make sure the files are executable:

```shell
chmod +x server client
```

6 for server terminal, if you ran code previously running it again might not work 
because port is currently in usage. 

```shell
gcc -o server hello_server.c 
./server 1234 
bind() error
```

```shell
gcc -o client hello_client.c 
./client 127.0.0.1 1234  
connect() error!
```

Because port 1234 is currently in usage 

## Linux File Generation and Data Save 

```shell
gcc low_open.c -o lopen
./lopen
cat data.txt
```

This code will actually return 


```txt
stephen@ASUSLAPTOP:~/TCPIP/c1$ gcc low_open.c -o lopen
stephen@ASUSLAPTOP:~/TCPIP/c1$ ./lopen
file descriptor: 3
stephen@ASUSLAPTOP:~/TCPIP/c1$ cat data.txt
Let's go!
```

Now let's read `data.txt` file generated from the code 

```shell
gcc low_read.c -o lread
./lread
```

This code will return 
```txt
stephen@ASUSLAPTOP:~/TCPIP/c1$ gcc low_read.c -o lread
stephen@ASUSLAPTOP:~/TCPIP/c1$ ./lread
file descriptor: 3
file data: Let's go!
```

Now let's generate file and socket together in linux and comapre file descriptor value as integer form 

```shell
gcc fd_seri.c -o fds
./fds
```

```txt
stephen@ASUSLAPTOP:~/TCPIP/c1$ ./fds
file descriptor 1: 3
file descriptor 2: 4
file descriptor 3: 5
```

## Run code in Windows Visual Studio

client server 

C++ 형식의 인수가 형식의 매개 변수와 호환되지 않습니다.

속성 -> C/C++ -> 언어 -> 준수 모드 -> 아니오 

Compile method 

```shell 
gcc -o client Client/main.c -lws2_32
client <IP> <PORT> 

gcc -o server Server/main.c -lws2_32 
server <PORT> 
``` 

For example, 
```shell
gcc -o server Server/main.c -lws2_32 
.\server.exe 9190

gcc -o client Client/main.c -lws2_32
.\client.exe 127.0.0.1 9190
```

속성 -> 링커 -> 입력 -> 추가 종속성 -> ws2_32.lib 추가 



