# TCP / IP Headers

Every field of the IPv4, TCP, and UDP headers on a 32-bit ruler. One row is 4 bytes.

### IP Version 4 Header

```shell
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
+-------+-------+---------------+-------------------------------+
|       |Header |   Type of     |                               |
|Version|Length |   Service     |         Total Length          |
+-------+-------+---------------+-----+-------------------------+
|        Identification         |Flags|      Fragment Offset    | 
+-------------------------------+-----+-------------------------+
| Time To Live  |    Protocol   |       Header Checksum         | 
+---------------+---------------+-------------------------------+
|                          Source IP Address                    |
+---------------------------------------------------------------+
|                     Destination IP Address                    |
+---------------------------------------------------------------+
|                      Options (0 ~ 40 Bytes)                   |
+---------------------------------------------------------------+
```

20 bytes without options (5 rows), up to 60 with them.

| field | bits | meaning |
|---|---:|---|
| `Version` | 4 | 4 for IPv4. IPv6 has a different header |
| `Header Length` | 4 | header size in 32-bit words: 5 → 20 B, max 15 → 60 B |
| `Type of Service` | 8 | priority class (DSCP) + congestion mark (ECN) |
| `Total Length` | 16 | header + payload in bytes, max 65,535 |
| `Identification` | 16 | ties the fragments of one datagram together |
| `Flags` | 3 | reserved, DF (don't fragment), MF (more fragments follow) |
| `Fragment Offset` | 13 | where this fragment sits, in 8-byte units |
| `Time To Live` | 8 | minus 1 per router hop; at 0 the packet is dropped, so it cannot loop forever |
| `Protocol` | 8 | what the payload is: 6 TCP, 17 UDP, 1 ICMP |
| `Header Checksum` | 16 | header only; recomputed at every hop because TTL changes |
| `Source / Destination IP` | 32 each | |
| `Options` | 0–320 | rarely used |

### TCP Header

```shell 
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
+-------------------------------+-------------------------------+
|          Source Port          |      Destination Port         |
+-------------------------------+-------------------------------+
|                          Sequence Number                      |
+---------------------------------------------------------------+
|                   Acknowledgement Number                      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Header |  Reserved |U|A|P|R|S|F|        Window Size            |
|       |           |R|C|S|S|Y|I|                               |
|Length |  Bits     |G|K|H|T|N|N|    Advertisement Window       |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           Check Sum           |      Urgent Pointer           | 
+-------------------------------+-------------------------------+
|                      Options (0 ~ 40 Bytes)                   |
+---------------------------------------------------------------+
```

20 bytes without options, up to 60 with them.

| field | bits | meaning |
|---|---:|---|
| `Source / Destination Port` | 16 each | which process on each host |
| `Sequence Number` | 32 | byte position of this segment's first data byte in **my** stream |
| `Acknowledgement Number` | 32 | next byte I expect from **the other side** |
| `Header Length` | 4 | in 32-bit words, same as IP |
| `Reserved` | 6 | always 0 |
| `URG` | 1 | `Urgent Pointer` is valid |
| `ACK` | 1 | `Acknowledgement Number` is valid; set on every segment after the first SYN |
| `PSH` | 1 | hand the data to the application now |
| `RST` | 1 | abort the connection |
| `SYN` | 1 | open: synchronize sequence numbers |
| `FIN` | 1 | I have nothing more to send |
| `Window Size` | 16 | free space left in **my** receive buffer (rwnd): flow control |
| `Check Sum` | 16 | header + data + a pseudo-header of the IP addresses |
| `Urgent Pointer` | 16 | end of urgent data; rarely used |
| `Options` | 0–320 | MSS, window scale, SACK, timestamps |

- Sequence and acknowledgement numbers count **bytes, per direction**, not packets.
- The window is what the receiver can absorb; cwnd (sender side, not in the header) is what the network can.

### UDP Header

```shell
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
+-------------------------------+-------------------------------+
|          Source Port          |      Destination Port         |
+-------------------------------+-------------------------------+
|          UDP  Length          |      UDP  Check Sum           | 
+-------------------------------+-------------------------------+
```

8 bytes in total: ports, `UDP Length` (header + data), `UDP Check Sum`.
No sequence number, no acknowledgement, no window. That is why the header is small, and why ordering and retransmission are left to the application.

| header | size without options |
|---|---:|
| IPv4 | 20 B |
| TCP | 20 B |
| UDP | 8 B |
