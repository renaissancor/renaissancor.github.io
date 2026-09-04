---
title: "Sequential Writes: WAL and LSM"
---

# Sequential Writes: WAL and LSM

The same idea appears at every layer of a storage engine: make the durable write sequential and defer the random ones.

## Commits are fast despite fsync because only the log is flushed

`fsync` is expensive, yet a single-disk Postgres commits thousands of transactions per second. A commit does not write table or index pages. It appends the change record to one sequential **write-ahead log** and fsyncs only that file. Sequential append is the cheapest thing a disk does. **Group commit** batches every transaction from the last millisecond or so into one fsync. Table and index pages are written lazily at **checkpoints**; power loss before that is handled by replaying the log.

This is the OS page cache trick one level up: durability from one cheap sequential write, expensive random writes deferred.

## Append-heavy logs want an LSM tree

- **B-tree:** one sorted tree on disk. Insert finds the page and writes in place. Reads are one descent, predictable. Writes are random I/O anywhere in the tree. Postgres, MySQL, SQLite.
- **LSM tree:** writes go to an in-memory sorted buffer, flushed as an immutable sorted file when full. Background compaction merges files. Writes are pure sequential append. Reads may consult several files, so slower, mitigated by Bloom filters. RocksDB, Cassandra, ClickHouse-style engines.

A request-log table (timestamp, model, latency, tokens; millions of appends a day; queried by time range; never updated) is textbook LSM. In practice that means ClickHouse or TimescaleDB rather than raw Postgres.

## The planner reads the same tradeoff from the other side

A matching index is ignored when the query is not selective. Each index hit is a random read into the table; past roughly 5 to 10% of rows matching, a sequential scan wins. The planner estimates this from statistics, and stale statistics after a bulk load make it guess wrong. `ANALYZE` first.

## Absence has no early exit, and that is what Bloom filters are for

The cost of an LSM read is not scanning within a segment. A segment is sorted, a sparse in-memory index lands you on one block, and that block scan costs the same whether the key is there or not. The cost is *across* segments: a present key stops at the first segment holding it, an absent key cannot stop until every segment is ruled out, because "not in this file" says nothing about the others.

A Bloom filter answers exactly that case, with a matching asymmetry: insert sets *k* bits, so any zero bit at query time is proof of absence. It answers **"definitely no" or "maybe"** — never yes. A false positive costs one wasted read and never a wrong answer. Around 10 bits per key gives roughly 1% false positives; there is no knob for false negatives because they are structurally impossible.

Plain Bloom filters cannot support deletion — clearing a bit would break other keys and introduce false negatives. LSM engines never notice, because SSTables are immutable: build the filter with the file, discard it when compaction retires the file. The structure's central limitation is void by a design choice made for unrelated reasons.

A B-tree needs none of this. One key lives in exactly one place, so an absent key costs the same as a present one: descend to the leaf where it would be, it is not there, done.

## An LSM writes more bytes than a B-tree, not fewer

The obvious reading of "sequential appends" is that an LSM writes less. It writes considerably more. Under leveled compaction a record is rewritten at every level it descends, and each merge into level *n+1* rewrites roughly ten bytes of resident data per byte arriving, since levels are about 10x apart. Commonly cited ranges: **10–30x write amplification for leveled LSM, 2–4x for a B-tree** (WAL plus page, higher with InnoDB's doublewrite buffer).

So the claim is not about volume. It is that (1) every LSM write is sequential while B-tree page updates are scattered random 4KB writes, (2) the foreground write is only a WAL append plus a memtable insert, with all amplification deferred to background compaction, and (3) space amplification is lower — no half-empty pages from splits, and sorted files compress well.

Compaction is disk-to-disk, not a RAM operation: a 100GB database does not fit in memory. It streams SSTables off disk and writes new ones back, holding only small buffers. The extra bytes are disk bytes, which is why write amplification is worth naming at all.

The failure mode mirrors the benefit. Compaction competes with foreground queries for the same disk bandwidth, and under sustained writes it falls behind: segments pile up, reads degrade, disk fills. A B-tree charges its cost immediately instead — less efficient, more predictable.

## Four mechanisms, one principle

| mechanism | foreground | deferred |
|---|---|---|
| WAL | append to one log, fsync it | table and index pages at checkpoint |
| LSM | WAL append plus memtable insert | compaction merges SSTables |
| buffer pool | dirty the page in memory | writeback flushes it |
| `DELETE` | mark dead, or write a tombstone | vacuum, purge, compaction |

The principle is not that reclaiming space is pointless — bloat is a real operational cost. It is that doing the expensive work **eagerly, inside the transaction, is what you cannot afford**, and it can be deferred and batched instead.

Which means all four share a failure mode: the background process falls behind and the deferred bill arrives at once. Compaction losing to foreground I/O, autovacuum losing to churn, a checkpoint write storm. Whenever an engine has a deferred column, the operational question is what happens when it cannot keep up — usually the real incident, rather than the steady state.


**Open:** measure write amplification of leveled versus tiered compaction, with numbers on a real workload rather than cited ranges. And the actual seq-scan crossover on a real table via `EXPLAIN (ANALYZE, BUFFERS)` with `enable_seqscan = off`.
