# Winsock TCP Echo Server, Step by Step

A TCP server in Winsock, in the order the calls happen: `socket → bind → listen → accept → recv/send`.

## Before the code

- **Server and client are roles.** TCP needs one side listening; P2P has no fixed server, every peer does both.
- **Port = process.** A port number is bound to one socket, owned by one process.
- **Loopback** `127.0.0.1` is this host. A connection over loopback never touches a NIC, so it is close to IPC. One host can also have several NICs (Ethernet, USB Wi-Fi), each with its own address.

## 1. socket + bind

```cpp
SOCKET hSocket = ::socket(AF_INET, SOCK_STREAM, 0);

SOCKADDR_IN svraddr = { 0 };
svraddr.sin_family = AF_INET;
svraddr.sin_port = htons(25000);                  // port, host → network byte order
svraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // accept on every local IP

if (::bind(hSocket, (SOCKADDR*)&svraddr, sizeof(svraddr)) == SOCKET_ERROR)
{
	puts("ERROR: Unable to bind IP Address and Port to the Socket");
	return 0;
}
```

- `INADDR_ANY`: listen on all of this host's addresses.
- `htons` / `htonl`: host byte order (little endian on x86) → network byte order (big endian), for 16- and 32-bit values.
- `bind` fails if another process already holds the port. Check with `netstat -ano | find "25000"`.

## 2. listen

```cpp
if (::listen(hSocket, SOMAXCONN) == SOCKET_ERROR) { /* error */ }
```

`backlog` is the queue of connections that finished the handshake but have not been `accept`ed yet. It still matters: when it overflows, the kernel silently drops new SYNs and clients retry after 1 s, then 3 s. See [TCP Loss Signals and Round Trips](nw-tcp-loss-signals-and-round-trips.md). `SOMAXCONN` asks for the system maximum.

## 3. accept

```cpp
SOCKADDR_IN clientaddr = { 0 };
int nAddrLen = sizeof(clientaddr);
SOCKET hClient = 0;

while ((hClient = ::accept(hSocket, (SOCKADDR*)&clientaddr, &nAddrLen)) != INVALID_SOCKET) {
    puts("New Client Connected");
    // ... step 4 with hClient
}
```

Two kinds of socket:

| socket | role |
|---|---|
| `hSocket` | listening socket: only accepts |
| `hClient` | one per client: carries the data. `accept` also fills in the client's IP and port |

## 4. recv / send (echo)

```cpp
char szBuffer[128] = { 0 };
int nReceive = 0;
while ((nReceive = ::recv(hClient, szBuffer, sizeof(szBuffer), 0)) > 0)
{
	::send(hClient, szBuffer, nReceive, 0);  // echo back exactly what arrived
	memset(szBuffer, 0, sizeof(szBuffer));
}
```

Send `nReceive` bytes, not `sizeof(szBuffer)`: the buffer is 128 bytes but a `recv` may return fewer.

## TCP is a byte stream, not messages

`send` and `recv` do **not** match 1:1. Three `send(1 byte)` calls can arrive as one `recv` of 3 bytes. The socket has kernel send and receive buffers:

```cpp
::getsockopt(hSocket, SOL_SOCKET, SO_SNDBUF, (char*)&nBufSize, &nLen);  // send buffer size
::getsockopt(hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&nBufSize, &nLen);  // receive buffer size
```

The application has to frame its own messages (length prefix, delimiter).

## Nagle: fewer packets vs lower latency

Per-packet cost is fixed, so **fewer, larger packets** (low PPS, same BPS) are cheaper than many tiny ones. **Nagle's algorithm** holds back small writes while earlier data is unacknowledged, to merge them.

```cpp
int nOpt = 1;
::setsockopt(hSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&nOpt, sizeof(nOpt));  // Nagle off
```

`TCP_NODELAY` sends each write immediately. A game client sending small inputs wants that. A server pushing many small updates may prefer to batch writes itself rather than rely on either Nagle or per-write sends.
