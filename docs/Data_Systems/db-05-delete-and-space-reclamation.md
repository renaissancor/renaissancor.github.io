---
title: "DELETE and Space Reclamation"
---

# DELETE and Space Reclamation

## `DELETE` frees no disk on either engine, and the LSM is the one that eventually gives it back

Delete 90% of a 100GB table and check disk usage ten seconds later. Both engines are using **more** disk than before, not less.

Postgres has three states, and they are easy to collapse into two:

1. **After `DELETE`, before vacuum.** The tuple is entirely still on the page, flagged dead via `xmax`, because MVCC owes it to any older transaction still running. Index entries remain. Every affected page is dirtied and written, and the deletes go to WAL. Table file unchanged, total disk up.
2. **After (auto)vacuum.** Dead tuples and their index entries are removed, and the space becomes reusable **by that table, in those pages**. The file does not shrink — `VACUUM` truncates only if the free space happens to sit at the physical end, which after a scattered delete it does not.
3. **After `VACUUM FULL`, `CLUSTER`, or `pg_repack`.** The table is rewritten into a new smaller file and the space returns to the OS. `VACUUM FULL` takes an `ACCESS EXCLUSIVE` lock and needs room for a second copy; `pg_repack` is the online equivalent.

Autovacuum reaches stage 2 and stops. Nothing reaches stage 3 on its own.

An LSM engine writes one **tombstone** per deleted key — a WAL append and a memtable insert, the fastest write path either engine has. Every original SSTable is untouched, because SSTables are immutable, so disk usage rises here too. Compaction later merges the levels, drops keys that have a tombstone above them, writes smaller files and unlinks the old ones, and the space genuinely comes back. A tombstone can only be discarded once it reaches the bottom level, or an older version underneath resurrects.

Hence the inversion: the append-only engine that never deletes anything ends at about 10GB, while the in-place engine that deletes properly ends with a 100GB file holding 10GB of live data.

## The reason is stable addresses, not laziness

Compaction rewrites files wholesale, so shrinking is free — you simply write a smaller file. Postgres cannot, because index leaves hold a `ctid`, a physical (page, offset) address. Relocating live rows to close gaps would break every pointer aimed at them, so reclamation means rewriting the table *and* all its indexes.

**Stable addresses buy predictable reads and cost cheap reclamation. Immutability costs write amplification and buys free reclamation.** Same coin, both sides.

This is not a Postgres quirk. InnoDB is clustered and behaves identically: `DELETE` sets a delete-marked flag, old versions go to the undo log, and a background **purge** thread removes them later. Freed space is reusable within the page, empty pages return to the tablespace free list, and the `.ibd` file does not shrink — `OPTIMIZE TABLE` is the rebuild. Historically, with `innodb_file_per_table=OFF`, `ibdata1` could only ever grow, and dump-and-reload was the only escape.

One qualification worth not over-learning: stage-2 space is not wasted if the table grows again, since it simply gets refilled. This only bites on a large one-time deletion where you actually want the disk back.

## Read cost during that window inverts too

A point read of a deleted key is **cheap** on an LSM — the tombstone is recent, so it sits high in the levels behind a Bloom filter, and the old data is never reached. It is **expensive** on Postgres before vacuum: full descent, `ctid`, heap fetch, visibility check, and zero rows returned. You pay the complete cost of a successful lookup for nothing.

Range scans reverse it. Scanning a range that is mostly tombstones means reading and skipping every one of them, a known production pathology — Cassandra will abort a query that crosses a tombstone threshold.

**Open:** measure it — `pg_total_relation_size` before a 90% delete, after it, after `VACUUM`, and after `VACUUM FULL`, expecting a change only at the last step.
