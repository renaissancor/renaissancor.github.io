
```cpp
SOCKET hSocket = ::socket(AF_INET, SOCK_STREAM, 0);

svraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

if (::bind(hSocket, (SOCKADDR*)&svraddr, sizeof(svraddr)) == SOCKET_ERROR)
{
	puts("ERROR: Unable to bind IP Address and Port to the Socket");
	return 0;
}
```

`INADDR_ANY` means any IP address is available 

Server is IP
Port Number is 

Even IP is okay, if other process preoccupied TCP port, then 
process will not be able to use that port. 

ipconfig + netstat

```cpp
svraddr.sin_port = htons(25000);
svraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
```

Little Endian to Big Endian functions

u_short htons([in] u_short hostshort); 
u_long htol([in] u_long hostlong); 

if ::listen(hSocket, SOMAXCONN) == SOCKET_ERROR 

int WSAAPI listen([in] SOCKET s, [in] int backlog); 
`backlog` parameter:  when several client get in, 
without backlog queue 
it was not able to connect server A when server B connect during 
making response in the past, but those problems are now 
handled by OS so no need to use now.   

Anyway server socket is prepared by `bind` `listen` 

SOCKADDR_IN clientaddr = {0,}; 
int nAddrLen = sizeof(clientaddr); 
SOCKET hClient = 0; 
char szBuffer[128] = {0,}; 

There is server socket, and 
telecommunication socket to connect with server too. 
In server perspective client is always remote 

```cpp
while((hClient = ::accept(hSocket, (SOCKADDR*) &clientaddr, &nAddrLen))!= INVALID_SOCKET) {
    puts("New Client Connected"); 
}
```

IP Address + Port 
Accept function 
Remote save 

After accept, send and receive

```cpp
while ((nReceive = ::recv(hClient, szBuffer, sizeof(szBuffer), 0)) > 0) 
// Connect by Client Communication Socket 
{
	::send(hClient, szBuffer, sizeof(szBuffer), 0); // Send to Client 
	puts(szBuffer); fflush(stdout); // Print Buffer 
	memset(szBuffer, 0, sizeof(szBuffer)); // Reset Buffer 
}
```

hSocket is server socket 
hClient is client socket 

BPS Bit Per Second 
Better to send fewer packet with bigger payload 
PPS Packet Per Second 
than send more packet with smaller payload 

Nagle Algorithm 
Reduce PPS, send as much 

Signal Switching means inefficiency. 
Buffered IO 

Timer Buffer full by Read / Write 

Socket is File, input output buffer. 
::getsockopt(hSocket, SOL_SOCKET, SO_SNDBUF, (char*)&nBufSize, &nLen) 

Check Sending  Buffer Size of Socket 

::getsockopt(hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&nBufSize, &nLen) 

Check Receiving Buffer Size of Socket 

send(); 
recv(); 

Are these functions 1 : 1 Match? NO. 
Buffered IO 

send(1); send(1); send(1); 
recv(128); 
Receive might read by once, for several sends. 

Loopback Interface is nearly IPC. 

::setsockopt(hSocket, IPPROTO_TCP, TCP_NODELAY,
	(char*)&nOpt, sizeof(nOpt));

This one will make NO_DELAY. Recommended for Game Client, 
but not recommende for game server. 



Data Structure 
Algorithm Basics 
Operating Systems 
Computer Networks 
Database Management 
Algorithm Design 