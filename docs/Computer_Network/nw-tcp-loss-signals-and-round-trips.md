---
title: "TCP Loss Signals and Round Trips"
---

# TCP Loss Signals and Round Trips

Corrections from a self-test on transport behavior. The common thread: for the traffic a service actually sends, latency is counted in round trips and loss signals, not bytes.

---

## Three duplicate ACKs halve cwnd; a timeout resets it to 1 MSS

Nothing is duplicated on the wire. Sender ships segments 1–5, segment 2 is lost. The receiver gets 1 and ACKs "want 2". It gets 3, but can only acknowledge in-order data, so it ACKs "want 2" again. Same for 4 and 5. The sender sees one ACK plus three duplicates: the receiver repeating "still missing 2".

- **Three dup ACKs** prove that 3, 4, 5 arrived. The network still delivers, one packet fell. Retransmit 2 immediately (fast retransmit), **halve cwnd** (fast recovery). Mild evidence, mild reaction.
- **Timeout** means no ACKs at all. Could be a dead link. cwnd → **1 MSS**, slow start again. Harsh evidence, harsh reaction.

Both are loss signals, both cut cwnd, timeout cuts far harder. The intuition that dup ACKs mean "send more" is exactly backwards.

Also: segment size is MSS. `min(cwnd, rwnd)` bounds **bytes in flight**, not packet size. cwnd is the sender's guess about the network; rwnd is the receiver's advertised free buffer, carried in every header.

---

## A 20 KB response costs 5 round trips on TLS 1.2, 2 on HTTP/3 resumed

| Stack | RTTs to deliver 20 KB |
|---|---|
| TCP + TLS 1.2 + HTTP/1.1 | 5 |
| TCP + TLS 1.3 | 4 |
| HTTP/3 (QUIC) | 3 |
| HTTP/3 with 0-RTT resumption | 2 |

Handshake 1 RTT. TLS 1.2 adds 2, TLS 1.3 adds 1, resumption 0. Initial cwnd is 10 MSS, about 14 KB, so the first data RTT delivers 14 KB, ACKs double cwnd, the remaining 6 KB arrive next RTT. On a 100 ms link that is 500 ms versus 200 ms for identical bytes with bandwidth irrelevant. Keep-alive and connection pooling beat compression for small responses.

Head-of-line blocking exists at two layers:

- **HTTP/1.1:** one request at a time per connection. HTTP/2 fixed this with multiplexed streams on one connection.
- **TCP:** one lost packet stalls *every* stream, because TCP delivers in order. HTTP/2 made this worse by putting everything on one connection. **HTTP/3** fixes it: QUIC orders per stream, so a loss stalls only its own.

---

## p99 bad only under load, median fine: three transport suspects before the application

1. **cwnd collapse.** One timeout, cwnd 1 MSS, slow start again. More load means more buffer overflow, more timeouts.
2. **Ephemeral port exhaustion.** Thousands of short connections per second, each normal FIN close leaving TIME_WAIT for 60 s. New connects stall. Note the direction: RST *skips* TIME_WAIT, so RST would avoid this, not cause it. Fix is keep-alive pooling, which also removes the handshake RTTs above.
3. **Listen backlog overflow.** SYNs arrive faster than the server calls `accept()`. The kernel silently drops them; the client retries at 1 s, then 3 s. A p99 spike of *exactly* 1 s or 3 s with nothing wrong in the app is this.

```bash
ss -ti                       # per-connection cwnd, rtt, retrans      → 1
ss -s                        # TIME_WAIT count                        → 2
netstat -s | grep -i listen  # "times the listen queue overflowed"    → 3
```

**Open:** reproduce cause 3 locally with a deliberately slow `accept()` loop and watch the 1 s / 3 s stairs in client latency.
