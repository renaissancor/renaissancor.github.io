---
title: "Replication Mechanics: Row Identity and Catalog Estimates"
---

# Replication Mechanics: Row Identity and Catalog Estimates

## Row-based replication must *find* each row, so a missing primary key turns one delete into a table scan per row

Row-based replication ships the effect of a statement, not the statement. A `DELETE` touching one million rows becomes one million row events, each carrying that row's image.

To apply one event the replica must locate the matching row. With a primary key that is a single index lookup. With no key at all it scans the table, comparing full row images. Cost goes from `O(events)` to `O(events × rows)` — every event scans everything.

The consequence is not "slower." One bulk delete on a large key-less table produces work that does not finish, and lag grows without bound. The replica never catches up, so it never reaches a state where it could be repaired cheaply. MySQL's `replica_rows_search_algorithms=HASH_SCAN` softens this but does not remove the asymmetry.

This inverts the usual intuition about which tables are dangerous. **A backup table nobody reads is often the highest-risk object in the schema**, because it is exactly the kind of table that never got a primary key and gets truncated in one statement.

Auditing a 19-table schema, both of my priors were wrong:

| prior | reality |
|---|---|
| the biggest tables are the risk | the 5.0 GB and 4.2 GB tables both had primary keys — harmless by this measure |
| the "high-churn" 8.1M-row table is the risk | it had a primary key, and total change volume for the whole host was 2.7 MB/day |
| — | the only two key-less tables were a 1.9M-row backup table and a 96K-row metadata table |

Size did not predict risk. Key presence did.

Postgres takes the stricter line: logical replication requires a `REPLICA IDENTITY` and *refuses* `UPDATE`/`DELETE` without one. An explicit error beats silent unbounded lag.

**Open:** how much does `HASH_SCAN` actually recover — benchmark a bulk delete on a key-less table against `INDEX_SCAN`.

## Catalog row counts are a fixed random sample computed per host, so identical databases report different numbers

`information_schema.tables.table_rows` is not a count and not a lagging count. For InnoDB it is an extrapolation from a random page sample, and **each server computes and stores its own**.

- `innodb_stats_persistent = ON` (default). InnoDB dives into `innodb_stats_persistent_sample_pages` random index pages — **default 20** — and extrapolates.
- The result is written to `mysql.innodb_table_stats` and kept. It is not re-rolled per query; it changes only on `ANALYZE TABLE` or after roughly 10% of rows are modified.
- `information_schema_stats_expiry` is a *second*, separate cache in front of that. Setting it to 0 bypasses the dictionary cache but returns the same persisted value — so the number looks stable, which makes it more convincing, not less.

A source and its replica each drew their own 20 pages at their own moment. The error is not noise around a shared value; it is one frozen bad sample per host per table.

Comparing a source and replica on a 6.5M-row table:

| | rows | error vs true |
|---|---|---|
| true `COUNT(*)`, both hosts | **6,549,212** | — |
| source estimate | 6,407,525 | −2.2% |
| replica estimate | 5,966,778 | −8.9% |

The two estimates differed by 440,747 rows. The two datasets differed by zero.

Two things falsified the "divergence" reading before the count confirmed it. **Physical size was identical to five significant figures** on both — at 822 bytes/row a real 440,747-row deficit is ~345 MB, and the observed difference was 0.0 MB. And another table showed the *replica with 73,849 more rows than the source*, which one-way replication lag cannot produce. The sign flip is the tell: real lag is directional, sampling error is not.

The tools are layered, and the catalog is not one of them: `COUNT(*)` verifies rows, a transaction-id subset test verifies position.

**Open:** does raising the sample-page count shrink the cross-host spread predictably, or just move it? And is Postgres `reltuples` sampled the same way on a physical replica?
