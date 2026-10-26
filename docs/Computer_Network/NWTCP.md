# Socket Options and `select` model

### TCP Socket Options

| option | what it changes |
|---|---|
| non-blocking (`FIONBIO`) | calls return immediately instead of waiting |
| `TCP_NODELAY` | turns Nagle off: small writes go out at once |
| `SO_LINGER` | what `closesocket` does with unsent data |

```cpp
unsigned long mode = 1; // 1 = non-blocking, 0 = blocking
ioctlsocket(socket, FIONBIO, &mode);

int flag = 1; // Nagle off
setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));

struct linger lingerOpt;
lingerOpt.l_onoff = 1;   // linger on
lingerOpt.l_linger = 10; // close waits up to 10 s for unsent data to be delivered
setsockopt(socket, SOL_SOCKET, SO_LINGER, (char*)&lingerOpt, sizeof(lingerOpt));
```

`SO_LINGER` cases:

| `l_onoff` | `l_linger` | close |
|---|---|---|
| 0 | – | returns at once; the kernel sends the rest in the background (default) |
| 1 | 0 | **abortive**: sends RST, drops unsent data, skips TIME_WAIT |
| 1 | N > 0 | blocks up to N seconds until unsent data is acknowledged |

#### Broadcasting in UDP

```cpp
SOCKET mysock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
int broadcastEnable = 1; 
::setsockopt(mysock, SOL_SOCKET, SO_BROADCAST, 
    (char*)&broadcastEnable, sizeof(broadcastEnable));

sockaddr_in thisAddr; 
thisAddr.sin_family = AF_INET; 
thisAddr.sin_port = htons(THIS_PORT); 
thisAddr.sin_addr.s_addr = inet_addr("192.168.1.255");  // subnet broadcast address
```

Broadcast must be enabled explicitly with `SO_BROADCAST`; `x.x.x.255` reaches every host on a /24.

### WinSocket TCP Server and Client

| server | client |
|---|---|
| `WSAStartup` → `socket` → `bind` → `listen` → `accept` | `WSAStartup` → `socket` → `connect` |

```cpp
// Server
WSADATA wsaData;
if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { /* handle error */ }
SOCKET listenSock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
::bind(listenSock, (sockaddr*)&serverAddr, sizeof(serverAddr));
::listen(listenSock, SOMAXCONN);

sockaddr_in clientAddr;
int addrLen = sizeof(clientAddr);
SOCKET clientSock = ::accept(listenSock, (sockaddr*)&clientAddr, &addrLen);
```

```cpp
// Client
SOCKET clientSock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
::connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr));  // length by value
```

Full walkthrough: [Winsock TCP Echo Server, Step by Step](TCPIP.md).

### WinSocket UDP Server and Client

```cpp
SOCKET sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
::bind(sock, (sockaddr*)&thisAddr, sizeof(thisAddr));

::sendto(sock, buffer, len, 0, (sockaddr*)&destAddr, sizeof(destAddr));   // where to
::recvfrom(sock, buffer, len, 0, (sockaddr*)&fromAddr, &fromLen);         // from whom
```

No connection, so every send names its destination and every receive reports its source. A UDP client may still `connect` to fix one peer, then use plain `send` / `recv`.

### TCP/UDP `select` model

```cpp 
#define FD_SETSIZE 256    // override FD_SETSIZE from 64 
#include <winsock2.h>     // override FD_SETSIZE before #include <winsock2.h>

std::vector<SOCKET> sockets(FD_SETSIZE, INVALID_SOCKET);  

// ... Server setup, socket binding 

for(;;) { // Whatever loop logic 
    fd_set readfds; 
    FD_ZERO(&readfds);        // reset fd_set 
    for (SOCKET s : sockets) if (s != INVALID_SOCKET)
        FD_SET(s, &readfds);  // add socket to fd_set 

    struct timeval timeout;   
    timeout.tv_sec  = 2;      //      seconds wait (2.0 seconds)
    timeout.tv_usec = 500000; // microseconds wait (0.5 seconds)
    SOCKET socketMax = UpdateSocketMax(); 
    int ret = ::select(socketMax + 1, &readfds, nullptr, nullptr, &timeout); // O(N)
    // pass nullptr instead of &timeout to wait forever

    if (ret > 0) {
        // handle sets 
        for(SOCKET &s : sockets) {
            if(FD_ISSET(s, &readfds)) { 
                // ::recv(...); 
            }
        }
    }
    else if(ret == 0) {
        // timeout, no event 
    } else if (ret < 0) {
        // error handle
        int err = WSAGetLastError(); 
    }
}

WSACleanup(); 
```

- The `fd_set` is **rebuilt every loop**: `select` overwrites it with only the ready sockets.
- Cost is **O(N)** per call in the number of sockets, twice: building the set and scanning it with `FD_ISSET`. Fine for hundreds; IOCP / epoll exist for more.
- Windows caps `FD_SETSIZE` at 64 unless redefined **before** including `winsock2.h`.
