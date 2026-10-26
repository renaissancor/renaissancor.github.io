# TCP/IP Model, Layer by Layer

What each TCP/IP layer does, bottom up, with the OSI layers it covers.

## 1. Network Access (OSI 1 Physical + 2 Data Link)

- **Physical**: bits on the wire. The receiver locks onto a known alternating pattern (the Ethernet preamble) before reading data. Rates are in bits per second; data that arrives faster than the buffer drains is lost.
- **Data link**: Ethernet frames addressed by **MAC** (Media Access Control) address, handled by the **NIC**. `FF:FF:FF:FF:FF:FF` = broadcast within the LAN. Details: [Physical Layer & Data Link Layer](NW0724.md).

## 2. Internet (OSI 3 Network)

| protocol | job |
|---|---|
| **IP** | addresses hosts across networks. `255.255.255.255` = broadcast to the local network |
| **ARP** | IP → MAC inside a LAN. Answers are cached for tens of seconds (Windows); hosts also announce their own IP/MAC when their network comes up |
| **ICMP** | control messages; `ping` is ICMP echo and measures **RTT** (round trip time) |

A game's "ping" is the same idea measured by the application (L7), not ICMP.

Clients should reach a server by **domain name**, not a hard-coded IP, because server IPs change.

### IP address classes

Classful addressing is obsolete (replaced by CIDR, `/24` style), but the private ranges are still the ones in use.

| class | first bits | range | private range |
|---|---|---|---|
| A | `0` | 0.0.0.0 – 127.255.255.255 | 10.0.0.0 – 10.255.255.255 |
| B | `10` | 128.0.0.0 – 191.255.255.255 | 172.16.0.0 – 172.31.255.255 |
| C | `110` | 192.0.0.0 – 223.255.255.255 | 192.168.0.0 – 192.168.255.255 |
| D | `1110` | 224.0.0.0 – 239.255.255.255 | multicast |
| E | `1111` | 240.0.0.0 – 255.255.255.255 | reserved |

`127.0.0.0/8` is loopback (`127.0.0.1` = this host).

## 3. Transport (OSI 4)

**A socket is a file.** It is a kernel object behind the same open / read / write / close interface:

```cpp
fopen_s(&fp, "CON", "w");   // "CON" = the console, written through the file API
```

Writing to a socket goes out over TCP instead of to the console. Everything else is the same as file I/O.

- **Port** = endpoint. A process opens a socket, the socket is bound to a port, so a port identifies a process on a host.
- Units on the way down: stream (L4 TCP) → **segment** → **packet** (L3) → **frame** (L2).

### Opening: 3-way handshake

```text
   Client                                  Server
     |  SYN       seq=1000                    |
     |--------------------------------------->|
     |  SYN+ACK   seq=4000  ack=1001          |
     |<---------------------------------------|
     |  ACK       seq=1001  ack=4001          |
     |--------------------------------------->|
     |           connection established       |
```

- Each side picks its own starting sequence number; the reply acknowledges it **+1**.
- MSS (maximum segment size) is agreed in these options.
- Costs **1 RTT** before the first byte of data.

### Closing: 4-way handshake

```text
   Active closer                         Passive closer
     |  FIN                                   |
     |--------------------------------------->|
     |                                   ACK  |
     |<---------------------------------------|
     |                                   FIN  |
     |<---------------------------------------|
     |  ACK                                   |
     |--------------------------------------->|
   TIME_WAIT (2 × MSL)                     CLOSED
```

Either side may close first. **The side that closes first is left in TIME_WAIT.** So if the server initiates the close, TIME_WAIT piles up on the server, which is why servers are usually designed to let the client close.

## 4. Application (OSI 5–7)

Session, presentation and application merge here: HTTP, DNS, game protocols. Port forwarding and proxy servers also live at this level of concern.

## Commands (Windows)

| command | shows |
|---|---|
| `ipconfig /all` | interfaces, IP, MAC, DNS |
| `route print` | routing table |
| `netstat -ano` | all connections with owning PID |
| `tracert <host>` | router hops to a host |

```bash
netstat -ano | find "LISTEN"   # listening sockets
netstat -ano | find "25000"    # who uses port 25000
```
