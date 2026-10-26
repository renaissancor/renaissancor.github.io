# Network Vocabulary

### Addressing and core protocols

| term | stands for | one line |
|---|---|---|
| NIC | Network Interface Card / Controller | the hardware between host and medium |
| MAC | Media Access Control | 48-bit hardware address inside a LAN |
| IP | Internet Protocol | addresses and routes packets between networks |
| ARP | Address Resolution Protocol | IP → MAC inside a LAN |
| ICMP | Internet Control Message Protocol | control and error messages; `ping` uses it |
| PING | Packet INternet Groper | echo request/reply, measures RTT |
| TCP | Transmission Control Protocol | reliable, ordered byte stream |
| UDP | User Datagram Protocol | unreliable datagrams, minimal header |
| ISP | Internet Service Provider | who connects you to the Internet |
| NAT | Network Address Translation | many private IPs share one public IP |

### Security

| term | stands for | one line |
|---|---|---|
| SSL / TLS | Secure Sockets Layer / Transport Layer Security | encryption over TCP; TLS is the successor |
| HTTPS | HyperText Transfer Protocol Secure | HTTP over TLS |
| SSH | Secure Shell | encrypted remote shell |
| IPsec AH | Authentication Header | integrity + authentication of IP packets |
| IPsec ESP | Encapsulating Security Payload | adds encryption |
| ISAKMP | Internet Security Association and Key Management Protocol | sets up IPsec keys |
| VPN | Virtual Private Network | a private network tunneled over a public one |
| SSL VPN | | VPN over TLS, often per application |
| ACL | Access Control List | allow/deny rules by address and port |
| NAC | Network Access Control | decides which devices may join the network |
| IPS | Intrusion Prevention System | **inline**: sits in the path, can block |
| NIDS | Network Intrusion Detection System | **out of path**: sees a copy, can only alert |
| WAF | Web Application Firewall | filters HTTP, usually as a proxy |
| UTM | Unified Threat Management | firewall + IPS + VPN + ... in one box |

### Where a security device sits

| placement | sees | can block |
|---|---|---|
| **Inline** (firewall, IPS) | the real traffic, in the path | yes |
| **Out of path** (IDS) | a mirrored copy | no, only detect |
| **Proxy** (WAF) | terminates the connection and opens a new one | yes, with full application view |

### Direction

- **Inbound**: packets coming into the host or network.
- **Outbound**: packets going out.
