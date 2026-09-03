---
title: "Sequential Writes: WAL and LSM"
---

# Sequential Writes: WAL and LSM

The same idea appears at three layers: make the durable write sequential and defer the random ones.

## Commits are fast despite fsync because only the log is flushed

`fsync` is expensive, yet a single-disk Postgres commits thousands of transactions per second. A commit does not write table or index pages. It appends the change record to one sequential **write-ahead log** and fsyncs only that file. Sequential append is the cheapest thing a disk does. **Group commit** batches every transaction from the last millisecond or so into one fsync. Table and index pages are written lazily at **checkpoints**; power loss before that is handled by replaying the log.

This is the OS page cache trick one level up: durability from one cheap sequential write, expensive random writes deferred.

## Append-heavy logs want an LSM tree

- **B-tree:** one sorted tree on disk. Insert finds the page and writes in place. Reads are one descent, predictable. Writes are random I/O anywhere in the tree. Postgres, MySQL, SQLite.
- **LSM tree:** writes go to an in-memory sorted buffer, flushed as an immutable sorted file when full. Background compaction merges files. Writes are pure sequential append. Reads may consult several files, so slower, mitigated by Bloom filters. RocksDB, Cassandra, ClickHouse-style engines.

A request-log table (timestamp, model, latency, tokens; millions of appends a day; queried by time range; never updated) is textbook LSM. In practice that means ClickHouse or TimescaleDB rather than raw Postgres.

## The planner reads the same tradeoff from the other side

A matching index is ignored when the query is not selective. Each index hit is a random read into the table; past roughly 5 to 10% of rows matching, a sequential scan wins. The planner estimates this from statistics, and stale statistics after a bulk load make it guess wrong. `ANALYZE` first.

**Open:** write amplification of leveled versus tiered compaction, with numbers. And the actual seq-scan crossover on a real table via `EXPLAIN (ANALYZE, BUFFERS)` with `enable_seqscan = off`.
