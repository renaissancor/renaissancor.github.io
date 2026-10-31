---
title: "Glossary: Networking"
tags:
  - glossary
  - networking
---

# Glossary: Networking

The words you must recognize instantly, not the theory behind them. Security terms are in [Infra and Security](gl-infra-security.md).

### Link and addressing

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| NIC | Network Interface Card / Controller | the hardware between host and medium | "the infra team swapped the NIC" |
| MAC | Media Access Control | 48-bit hardware address inside a LAN | "the switch still had the old port for that MAC after the VM moved" |
| LAN / WAN | Local / Wide Area Network | one site / links between sites | "that hop crosses the WAN" |
| VLAN | Virtual LAN | one physical L2 network split into isolated logical ones | "put it on a separate VLAN" |
| MTU | Maximum Transmission Unit | largest IP packet a link carries, 1500 bytes on standard Ethernet | "big packets vanish, check the MTU" |
| ARP | Address Resolution Protocol | IP → MAC inside a LAN | "stale ARP entry" |
| IP | Internet Protocol | addresses and routes packets between networks | |
| CIDR | Classless Inter-Domain Routing | classless addressing and route aggregation, written as a prefix such as `10.0.0.0/24` | "open that CIDR range" |
| DHCP | Dynamic Host Configuration Protocol | hands out IP addresses on join | "it took a DHCP address, make it static" |
| DNS | Domain Name System | name → IP | "it's always DNS" |
| NAT | Network Address Translation | rewrites addresses at a boundary, usually many private IPs sharing one public IP through ports (NAPT) | "the source IP is the NAT gateway's" |
| ICMP | Internet Control Message Protocol | control and error messages | "ICMP is blocked, so ping proves nothing" |
| Ping | named after sonar | ICMP echo request/reply, measures RTT. "Packet INternet Groper" is a backronym | |
| TTL | Time To Live | hop limit on a packet; expiry time on a DNS record | "lower the DNS TTL before the cutover" |
| BGP | Border Gateway Protocol | how networks exchange routes | "a route leak took them offline" |
| ISP | Internet Service Provider | who connects you to the Internet | |

### Transport

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| TCP | Transmission Control Protocol | reliable, ordered byte stream | |
| UDP | User Datagram Protocol | unreliable datagrams, minimal header | |
| QUIC | a name, no longer an acronym | encrypted, multiplexed transport over UDP; carries HTTP/3 | |
| SYN / ACK / FIN / RST | synchronize / acknowledge / finish / reset | TCP flags: open, confirm, finish sending, abort | "the client got an RST" |
| RTT | Round-Trip Time | there and back | "RTT is 80 ms from that region" |
| MSS | Maximum Segment Size | TCP payload per segment, about MTU minus headers | |
| CWND | Congestion Window | how much unacknowledged data the sender allows itself | |
| BDP | Bandwidth-Delay Product | bytes in flight needed to fill the pipe | "the buffer is smaller than the BDP" |

### Application and traffic

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| HTTP/2, HTTP/3 | Hypertext Transfer Protocol | multiplexed streams over TCP / over QUIC | |
| gRPC | gRPC Remote Procedure Calls | RPC over HTTP/2, usually with Protobuf | "that service only speaks gRPC" |
| RPC | Remote Procedure Call | call a function on another machine | "the RPC timed out" |
| LB | Load Balancer | spreads requests across backends. L4 sees connections, L7 sees requests | "L7 LB returned the 502" |
| VIP | Virtual IP | an address not tied to one machine | "point it at the VIP" |
| CDN | Content Delivery Network | caches content near users | "the CDN served a stale copy" |
| ECMP | Equal-Cost Multi-Path | hash flows across parallel routes | |
| Reverse proxy | | terminates client connections in front of servers | |
| Inbound / outbound | | traffic into / out of a host or network | "outbound 443 is closed" |

### Fast paths

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| RSS | Receive Side Scaling | NIC hashes flows onto multiple queues and cores. Not Resident Set Size | "all traffic landed on one RSS queue" |
| TSO / GSO / GRO | TCP Segmentation / Generic Segmentation / Generic Receive Offload | hand large buffers around, split or merge near the wire | "tcpdump on the host shows 64 KB packets, that's offload" |
| RDMA | Remote Direct Memory Access | read or write remote memory; one-sided operations skip the remote CPU | |
| RoCE | RDMA over Converged Ethernet | RDMA on Ethernet; v2 runs over UDP/IP, usually deployed lossless with PFC plus ECN | |
| InfiniBand | | credit-based, effectively lossless RDMA fabric common in GPU clusters | |
| DPDK | Data Plane Development Kit | kernel-bypass packet processing in user space | |
| eBPF | extended Berkeley Packet Filter, now just a name | sandboxed programs inside the kernel | |
| XDP | eXpress Data Path | eBPF hook at the earliest receive point, normally in the driver, before the network stack | |
