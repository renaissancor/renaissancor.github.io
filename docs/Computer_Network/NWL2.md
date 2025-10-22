# TCP/IP Model Layer 1 Network Access (네트워크 엑세스)

## OSI Layer 1 Physical 

Send 01010101 first to check bit speed, 
analyze pattern and receive data. 

Unit by bit, 1MBPS means over 1MB data per second will 
be lost after buffer got full. 

## OSI Layer 2 Datalink 

Ethernet Protocol : MAC based L2 Protocol 

MAC Address Media Access Control 
NIC Network Interface Card 


# TCP/IP Model Layer 2 Internet (네트워크)

## OSI Layer 3 Network 

IP Internet Protocol 

IP Address `255.255.255.255` Broadcast in Internet 

ARP Address Resolution Protocol 
ARP Cache : At least 20 ~ 30 seconds according to MS document 
Recent Windows OS broadcast ARP including its IP and corresponding MAC when reboot, if newtork IP setup is done, 

Dynamic IP Address to Static Physical MAC Address in LAN.  
MAC Address `FF:FF:FF:FF:FF:FF` Broadcast in LAN.  

ICMP Internet Control Messge Protocol 
ICMP is basically what we call as ping. 

RTT Round Trip Time 

Ping is originally L3, but in video games it is used as RTT 
in L7, Video Game Application

RARP Not learn yet 

Subnet Mask LAN 

In Game Client perspective, Game Server IP and Port must be known. 
However, hard coding IP address of server is unlikely these days. 
It should start with Website domain address, and IP should be dynamically changed sometimes. 

IP Class A Class B Class C 

Class A 
0.0.0.0 ~ 127.255.255.255 
Class A Private IP Address 
10.0.0.0 ~ 10.255.255.255 

Class B 
128.0.0.0 ~ 191.255.255.255 
Class B Private IP Address 
172.16.0.0 ~ 172.31.255.255 

Class C 
192.0.0.0 ~ 223.255.255.255 
Class C Private IP Address 
192.168.0.0 ~ 192.168.255.255 

Class D Multicast, start with 1110 = 224 
224.0.0.0 ~ 239.255.255.255 

Class E 
240.0.0.0 ~ 255.255.255.255 


# TCP/IP Model Layer 3 Transport (전송) 

Socket : TCP Connection 

Socket is File, Kernel Object Virtualized Interface, if it is 
protocol element. 
Some device is bluetooth socket, some is IRDA etc ... 

```cpp
fopen_s(&fp, "CON", "w"); 
``` 
Meaning it is console. 
f.putchar(); 
If it is Socket, then it is goining to get out by TCP network. 

## OSI Layer 4 Transport 

Open Create Close Delete 
r read w write x execute 

TCP Socket Programming:  
TCP virtualized file IO how 

fopen fclose read write etc ... 
Like fopen function open file, socket function 
open close. anything else is same as file. 

File > Stream 
Socket Stream L4 Segment L3 Packet L2 Frame 

TCP/IP source and destination are Port. 
End point is port, and port is opened by socket. 
Socket is opened by Process. 
Thus, Port Number is Process Identifier 

# TCP/IP Model Layer 4 Application (응용)

TCP Connection Begin 
3-Way Handshake 
SYN 
SYN + ACK 
ACK 

Client Send Segement + Footer SYN Its own sequence number 
SYN(1000) 
After server receive sequence number, then it adds 1 to 
sequence number, 1000 + 1 = 1001 as acknowldegment 
SYN(4000) + ACK(1001) 

During 3 Ways Handshake, TCP information including MSS (Maximum Segment Size)
is transferred. 

Takes RTT to get 3 way handshake SYN + ACK.

TCP Connection End 4 Way Handshake 

End process MUST start by Client, not Server 

Client to Server FIN + ACK 
Server to Client ACK 
Server to Client FIN + ACK 
Client to Server ACK 

If Server is in TIME_WAIT, it means Server requested 
4 Way Handshake, which is not a good design. 



Port Forwarding  

## OSI Layer 5 Session 

## OSI Layer 6 Expression 

## OSI Layer 7 Application 

## Proxy Server 


## Terminal Commands 

```bash
ipconfig /all 
```


```bash
route 
route print 
```

```bash
netstat
netstat -ano 
netstat -ano | find "LISTEN"
netstat -ano | find "25000" # Port Number 
```

```bash
tracert 
```

