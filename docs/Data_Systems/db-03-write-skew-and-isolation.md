---
title: "Write Skew and Isolation Levels"
---

# Write Skew and Isolation Levels

## Snapshot isolation only detects two writes to the same row, so write skew slips through

Two doctors are on call. Two transactions each read the count, see 2, and each removes one doctor, each believing one will remain. Result: zero on call. Neither transaction wrote a row the other read, and the database did not complain.

- Snapshot isolation (Postgres `REPEATABLE READ`) checks whether two concurrent transactions write the **same row**. They wrote different rows. No conflict detected.
- The actual bug is that each write depended on a **read** the other transaction invalidated. Snapshot isolation does not track read-to-write dependencies. This anomaly is **write skew**.
- **Serializable** tracks them. Postgres implements it as SSI and aborts one transaction. InnoDB reaches the same guarantee with range locks.
- Cheaper fixes: `SELECT ... FOR UPDATE` on the rows you read, so the second transaction blocks; or a constraint the write itself violates, such as `on_call >= 1`.

Levels, weakest to strongest: read uncommitted, read committed, repeatable read (snapshot), serializable. Postgres defaults to read committed, which permits more anomalies than this one.

**Open:** reproduce in two `psql` sessions under `REPEATABLE READ`, then under `SERIALIZABLE`, and watch the second abort.
