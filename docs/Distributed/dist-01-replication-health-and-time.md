---
title: "Replication Health and Time"
---

# Replication Health and Time

## A healthy link proves liveness, not freshness — every node in a chain can report "in sync" while the data is arbitrarily stale

In a chain `A → B → C`, node C measures its relationship to **B only**. If the `A → B` edge breaks, B stops receiving but keeps serving. C keeps its connection, has nothing to apply, and reports threads running and lag zero.

C is now perfectly synchronized with a source that stopped being current. Every local indicator is green and every one of them is honest. The failure is undetectable at C because the failed edge is not incident to C. Comparing positions between B and C does not help either — they agree with each other while both drift from A.

**Any check whose scope is one hop is blind to the hop above it.** Health has to be evaluated per edge, not per node.

Two consequences worth designing for:

- C must inspect B's *upstream* status, not only its own downstream one. In MySQL this is why the `REPLICATION CLIENT` privilege earns its place beyond replication itself: it lets C run `SHOW REPLICA STATUS` *on B* and observe the `B ← A` edge, with no access to A at all.
- **Silence cannot substitute for a health signal.** The obvious fallback — "no events for N hours means something broke" — fails wherever zero change is legitimate. In one system I measured, 13 of 43 days had literally zero writes: every Saturday, nearly every Sunday, and a public holiday. A staleness alarm keyed on inactivity would fire every weekend. Freshness must be established by position, never by traffic.

A related trap in the same family: `Seconds_Behind_Source` reports `0` rather than `NULL` in some disconnect states, so even a broken *local* link can look healthy on that metric alone. I saw the honest version — read position exactly equal to executed position, applier idle, lag zero — and the point is that the dishonest version is indistinguishable from where the replica stands.

**Open:** can a downstream node bound staleness without any credential on the origin — does the origin's identity or position propagate far enough through the intermediate stream? The same shape applies to a CDN edge that is "healthy" against a stale origin.

## An interval measured across two machines carries their clock offset, so only single-clock intervals are measurements

I wanted "how long does a change take to reach the replica," subtracted a timestamp written on the source from one written on the replica, and got a **negative** number. Effect before cause. That is not a result to explain away — it is direct proof the two clocks disagree.

The commit timestamp is written by A using A's clock; the apply timestamp by B using B's clock. Their difference is `true_duration + (offset_B − offset_A)`. The offset term is invisible and unsigned. When it exceeds the true duration the result goes negative — which is the only reason I noticed. **A skew smaller than the duration yields a plausible wrong number and gets believed.**

Sync *discipline* matters more than sync existence. The machine ran `rdate` once daily, which **steps** the clock rather than slewing it: free-running drift between runs, a jump at the run. NTP and chrony slew continuously for exactly this reason.

| interval | value | clocks |
|---|---|---|
| commit (A) → queued (B) | 3,377 µs | two |
| commit (A) → applied (B), worker 2 | **−2,532 µs** | two |
| commit (A) → applied (B), worker 3 | **−2,258 µs** | two |
| relay write (B → B) | 50 µs | one |
| queue end → apply start (B → B) | 7 µs | one |
| apply execution (B → B) | 847 µs | one |

The four cross-clock samples spanned 11.2 ms. The single-clock intervals were tight — apply execution across four samples ran 621 / 847 / 856 / 1,147 µs.

So the honest report is **"relay arrival to applied: 854 µs"** (one clock, trustworthy) and **"transport: low single-digit milliseconds"** (order of magnitude only). Quoting "3.377 ms" would report a number whose error bar is its own size.

Two rules follow. Decompose every cross-machine measurement into single-clock segments and keep those separately — what survives is real, what spans clocks is an estimate. And **never build correctness logic on cross-host timestamp comparison**: use a logical clock, whose subset test is exact and clock-independent. This is the practical case for logical over physical time, which I had only ever seen argued abstractly.

**Open:** how do you measure the offset in order to correct for it? NTP estimates it by assuming symmetric round-trip delay — how badly does asymmetric routing break that, and what does Spanner's TrueTime actually buy by bounding uncertainty instead of removing it?
