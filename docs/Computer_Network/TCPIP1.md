# Network Programming and Sockets

Building and running the first socket programs on WSL and on Windows, and the one result worth remembering: **a socket is a file descriptor.**

## Run on Linux (WSL)

```shell
gcc -o server hello_server.c
gcc -o client hello_client.c

./server 1234                 # terminal 1: listen on port 1234, leave running
./client 127.0.0.1 1234       # terminal 2: connect over loopback
```

| symptom | cause | fix |
|---|---|---|
| `Permission denied` | not executable | `chmod +x server client` |
| `bind() error` right after a rerun | port 1234 still held by the previous run (TIME_WAIT) | wait, or set `SO_REUSEADDR` before `bind` |
| `connect() error!` | no server listening on that port | start the server first |

## Files and sockets share one descriptor table

Write and read a file with the low-level calls:

```txt
stephen@ASUSLAPTOP:~/TCPIP/c1$ gcc low_open.c -o lopen
stephen@ASUSLAPTOP:~/TCPIP/c1$ ./lopen
file descriptor: 3
stephen@ASUSLAPTOP:~/TCPIP/c1$ cat data.txt
Let's go!
```

```txt
stephen@ASUSLAPTOP:~/TCPIP/c1$ gcc low_read.c -o lread
stephen@ASUSLAPTOP:~/TCPIP/c1$ ./lread
file descriptor: 3
file data: Let's go!
```

Then open a file and two sockets in one program and print their descriptors:

```txt
stephen@ASUSLAPTOP:~/TCPIP/c1$ ./fds
file descriptor 1: 3
file descriptor 2: 4
file descriptor 3: 5
```

- 0, 1, 2 are stdin, stdout, stderr, so the first new descriptor is **3**.
- Files and sockets get **consecutive numbers from the same table**. To the kernel, a socket is just another file, which is why `read` / `write` work on both.

## Run on Windows (Visual Studio or MinGW)

```shell
gcc -o server Server/main.c -lws2_32
.\server.exe 9190

gcc -o client Client/main.c -lws2_32
.\client.exe 127.0.0.1 9190
```

Visual Studio settings for the book's C code:

| error | setting |
|---|---|
| link errors on `socket`, `bind`, ... | Properties → Linker → Input → Additional Dependencies → add `ws2_32.lib` |
| "argument of type ... is incompatible with parameter of type ..." (C string literals) | Properties → C/C++ → Language → Conformance mode → **No** |
