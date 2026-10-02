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

## An isolation level is only *when the snapshot is taken*; MVCC sits under all of them

Read committed takes a fresh snapshot **per statement**, so the same row read twice in one transaction can change in between: a non-repeatable read. Repeatable read takes one snapshot **per transaction**, so it cannot. Serializable adds conflict tracking on top, which is what catches the write skew above. Postgres, Oracle and SQL Server default to read committed; InnoDB defaults to repeatable read.

MVCC is the machinery, not a level. Each row version carries the transaction ids that created and deleted it; a snapshot is the set of ids that were committed at the moment it was taken. Dirty reads are impossible by construction, because an uncommitted id is in nobody's set. Reads never block writes and writes never block reads. Locks remain for write-write on the same row, `SELECT ... FOR UPDATE`, and InnoDB's gap locks.

The three classic anomalies split on two axes: whether you saw *uncommitted* data (dirty read) or committed data that changed, and whether a *value* changed (non-repeatable) or the *set* of matching rows changed (phantom).

Standard and engines disagree at repeatable read. The SQL standard permits phantoms there. InnoDB blocks them with next-key locks. Postgres blocks them too, because a per-transaction snapshot cannot see rows that did not exist when it was taken; its repeatable read is snapshot isolation under the standard's name, which is exactly why write skew is the anomaly left standing.

**Open:** show the per-statement snapshot in two `psql` sessions under read committed, then the same sequence under repeatable read.

