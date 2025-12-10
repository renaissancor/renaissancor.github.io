# Socket Options and `select` model 

### TCP Socket Options 

- Blocking / Non Blocking Socket 
- Nagle Algorithm 
- Linger Option 

```cpp
unsigned long mode = 1; // 1 = non-blocking, 0 = blocking
ioctlsocket(socket, FIONBIO, &mode);

int flag = 1; // Nagle Deactivate 
setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));

struct linger lingerOpt;
lingerOpt.l_onoff = 1;   // Activate linger, terminate by RST 
lingerOpt.l_linger = 10; // Unit: Second, close socket after 10 seconds wait  
setsockopt(socket, SOL_SOCKET, SO_LINGER, (char*)&lingerOpt, sizeof(lingerOpt));
```

#### Broadcasting in UDP 

```cpp
SOCKET mysock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
int broadcastEnable = 1; 
::setsockopt(mysock, SOL_SOCKET, SO_BROADCAST, 
    (char*)&broadcastEnable, sizeof(broadcastEnable));

sockaddr_in thisAddr; 
thisAddr.sin_family = AF_INET; 
thisAddr.sin_port = htons(THIS_PORT); 
thisAddr.sin_addr.s_addr = inet_addr("192.168.1.255");
```

### WinSocket TCP Server and Client 

TCP Server 

1. WSA Setup 
2. ::bind 
3. ::listen 
4. ::accept 

```cpp
WSADATA wsaData;
if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { /*Handle Error*/}
SOCKET listenSock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
sockaddr_in clientAddr;
int addrLen = sizeof(clientAddr);
SOCKET clientSock = accept(listenSock, (sockaddr*)&clientAddr, &addrLen);
```

TCP Client 

1. WSA Setup 
2 ::connect 

```cpp
SOCKET clientSock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
socklen_t serverAddrLen = sizeof(serverAddr); 
::connect(clientSock, (sockaddr*) &serverAddr, &serverAddrLen); 
```

### WinSocket UDP Server and Client  

UDP Server 
```cpp
SOCKET sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
::bind(udpSock, (sockaddr*)&thisAddr, sizeof(thisAddr)); 
```

UDP send & recv should have destination to send, and source received from. 

```cpp
::sendto(sock, buffer, len, 0, (sockaddr*)&destAddr, sizeof(destAddr));
::recvfrom(sock, buffer, len, 0, (sockaddr*)&fromAddr, &fromLen);
```

For client, if SOCKET is ::connect to certain server, then 
send and recv is also possible. 

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
    int ret = ::select(socketMax + 1, &readfds, nullptr, nullptr, nullptr);  // Wait Infinite 

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

