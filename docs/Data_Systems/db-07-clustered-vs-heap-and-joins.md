---
title: "Clustered vs Heap Storage, and What It Does to Joins"
---

# Clustered vs Heap Storage, and What It Does to Joins

## The whole InnoDB-vs-Postgres storage difference is one decision: what a secondary index points at

InnoDB stores the row inside the primary-key B-tree, so a secondary index leaf holds the **primary key value**. Postgres keeps rows in an unordered heap, so every index leaf holds a **ctid**, the tuple's physical address. Everything else follows:

- InnoDB: a secondary lookup walks the secondary tree, then the PK tree again (the "double lookup"). In exchange, a row can move physically and no secondary index notices.
- Postgres: a secondary lookup goes straight to the heap, one jump. In exchange, every tuple move is an index write, which is what heap-only tuples exist to avoid.
- SQL Server lets you choose per table and gets both behaviours: heap tables point by RID, clustered tables point by key.

Covering is a property of the **(index, query) pair**, not the index. Either engine does an index-only scan when the query needs nothing outside the index. A clustered index is always covering because the row is the leaf.

## Joins do not favour one engine; they split by join key

| join shape | InnoDB (clustered) | Postgres (heap) |
|---|---|---|
| on the parent's PK, row needed | row is in the leaf, no extra jump | PK index → ctid → heap, one jump |
| on a secondary key, row needed | secondary → PK → PK tree, **two** walks | secondary → ctid → heap, one jump |
| covering | tie | tie |

PK-joins lean InnoDB, secondary-key joins with a row fetch lean Postgres, and neither is what makes a join slow. That is almost always (1) no index on the join key, so the planner falls to a full nested loop, (2) too many matching rows, or (3) a bad join order. The extra jump is a tuning-level effect next to those.

"Rows times time-to-find-a-match-per-row" is not a join cost model. It is the definition of **nested loop join**, and the per-row term is the entire story: `log M` with an index on the inner key, `M` without. The planner's other two options are hash join (`O(N+M)`, build the small side in memory, probe with the large) and merge join (`O(N+M)` when both inputs are already sorted). Normalization adds joins; a join-key index pays for them. Postgres does not create indexes on foreign keys automatically, so there the `N×M` cliff is one forgotten `CREATE INDEX` away.

## A random UUID primary key costs InnoDB twice

Because the PK is the row locator, it is copied into every secondary index, so a 16-byte random key fattens every index on the table. And because the table *is* the PK tree, random keys scatter inserts across leaf pages instead of appending at the right edge. Postgres pays only the second cost, and only inside the PK index. A time-ordered UUID variant removes the scatter and keeps the width.

**Open:** measure the join table above with `EXPLAIN ANALYZE` on both engines, buffer reads per row; and measure `.ibd` growth for 1M inserts under auto-increment vs random UUID vs time-ordered UUID.
