---
title: "Glossary: Infra and Security"
tags:
  - glossary
  - infrastructure
  - security
---

# Glossary: Infra and Security

The words you must recognize instantly, not the theory behind them.

### Reliability

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| SLI | Service Level Indicator | the measured number | |
| SLO | Service Level Objective | the target for it, internal or published | "we burned the error budget" |
| SLA | Service Level Agreement | the contract, with penalties | |
| SRE | Site Reliability Engineering | operations run as engineering | |
| HA | High Availability | survives a component failure | "is that HA or a single node?" |
| SPOF | Single Point Of Failure | one thing whose loss stops everything | |
| DR | Disaster Recovery | recovering from a major disaster: site loss, large-scale failure, attack or operator error | |
| RPO | Recovery Point Objective | how much data you can lose, measured in time | |
| RTO | Recovery Time Objective | how long you can be down | |
| MTTR / MTBF | Mean Time To Repair (also Recovery, Respond, Resolve; ask which) / Between Failures | average recovery time / average time between failures | |
| RCA | Root Cause Analysis | the post-incident explanation | "RCA is due Friday" |
| p50 / p99 | 50th / 99th percentile | median / tail latency | "p99 doubled, p50 is flat" |
| QPS / RPS | Queries / Requests Per Second | request load | |
| TPS | Transactions Per Second | database load. In LLM serving, tokens per second | |

### Deployment

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| CI/CD | Continuous Integration / Delivery (or Deployment) | automated test and release | "CI is red" |
| IaC | Infrastructure as Code | infrastructure defined in versioned files | |
| VM | Virtual Machine | guest OS on a hypervisor | |
| K8s | Kubernetes | container orchestrator; 8 letters elided | |
| VPC | Virtual Private Cloud | your isolated network in a cloud | |
| AZ | Availability Zone | one or more isolated datacenters within a region | "lost one AZ" |
| Blue-green / canary | | two full environments, cut traffic over, roll back by switching back / release to a small share first | "the canary is at 5%" |
| NTP / PTP | Network / Precision Time Protocol | clock sync: tens of ms over the Internet, under 1 ms on a good LAN / sub-microsecond on a LAN with hardware timestamping | |
| LTS / EOL | Long-Term Support / End Of Life | supported for years / no more patches | "that kernel is EOL" |

### Identity and access

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| AuthN / AuthZ | Authentication / Authorization | who you are / what you may do | "AuthN passed, AuthZ denied" |
| IAM | Identity and Access Management | who can do what, centrally | "that role lacks the IAM permission" |
| RBAC | Role-Based Access Control | permissions attach to roles | |
| SSO | Single Sign-On | one login for many systems | |
| MFA | Multi-Factor Authentication | two or more proofs of different types (know, have, are) | |
| OAuth | not officially expanded; often given as Open Authorization | delegated access by token | |
| OIDC | OpenID Connect | login layered on OAuth | |
| JWT | JSON Web Token | compact token carrying claims, usually signed | "the JWT expired" |

### Crypto and transport security

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| SSL / TLS | Secure Sockets Layer / Transport Layer Security | encryption, integrity and authentication for a connection, usually over TCP; TLS replaced SSL, the old name stuck. Not Thread-Local Storage | "the TLS cert expired" |
| HTTPS | HTTP Secure | HTTP over TLS | |
| mTLS | mutual TLS | both sides present certificates | |
| CA | Certificate Authority | signs certificates | "the internal CA isn't trusted" |
| PKI | Public Key Infrastructure | the whole system of keys and CAs | |
| SSH | Secure Shell | encrypted remote shell | |
| VPN | Virtual Private Network | a private network tunneled over a public one | |
| SSL VPN | | VPN over TLS, often per application | |
| IPsec AH | Authentication Header | integrity and authentication of IP packets | |
| IPsec ESP | Encapsulating Security Payload | encryption plus integrity and authentication; AH has no encryption | |
| ISAKMP | Internet Security Association and Key Management Protocol | framework for IPsec security associations; keying is done by IKE. Legacy name, superseded by IKEv2 | |

### Network security devices

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| ACL | Access Control List | list of who may access what; on network gear, allow/deny rules by address and port | "add it to the ACL" |
| NAC | Network Access Control | decides which users and devices may join, based on identity and device health | |
| IPS | Intrusion Prevention System | **inline**: sits in the path, can block | |
| IDS / NIDS | (Network) Intrusion Detection System | **out of path**: sees a copy; alerts, cannot reliably block | |
| WAF | Web Application Firewall | filters HTTP; often a reverse proxy, also a bridge or embedded in the web server | "the WAF blocked that payload" |
| UTM | Unified Threat Management | firewall, IPS, VPN and more in one box | |
| DMZ | Demilitarized Zone | exposed subnet between Internet and internal | |

| placement | sees | can block |
|---|---|---|
| **Inline** (firewall, IPS) | the real traffic, in the path | yes |
| **Out of path** (IDS) | a mirrored copy | not reliably; can send a TCP reset or tell a firewall to block |
| **Proxy** (reverse-proxy WAF) | terminates the connection and opens a new one | yes, with full application view |

### Attacks and response

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| CVE | Common Vulnerabilities and Exposures | public ID for a vulnerability | "is that CVE patched?" |
| CVSS | Common Vulnerability Scoring System | severity from 0 to 10 | |
| DDoS | Distributed Denial of Service | flood from many sources | |
| XSS | Cross-Site Scripting | attacker's script runs in the victim's browser as the trusted site | |
| CSRF | Cross-Site Request Forgery | a victim's browser sends a forged request | |
| SQLi | SQL Injection | input executed as SQL | |
| RCE | Remote Code Execution | attacker runs their own code on the target over the network | "that's an unauthenticated RCE" |
| SIEM | Security Information and Event Management | central log correlation and alerts | |
| SOC | Security Operations Center | the team watching alerts. In hardware, system on chip | |
