---
title: "SQLD Cheat Sheet"
---

# SQLD Cheat Sheet

One rule per line, plus the reason when the reason is what makes it stick. Scope follows the post-2024 syllabus: 50 questions in 90 minutes, subject 1 data modeling (10 questions), subject 2 SQL (40 questions). Pass at 60 / 100 with at least 40% in each subject. The exam mixes Oracle and SQL Server, and often asks which DBMS a statement belongs to. Oracle unless noted.

Every example uses this table:

```
EMP
empno name  dept  sal  mgr
1     King  10    500  NULL
2     Lee   10    300  1
3     Park  20    300  1
4     Kim   20    200  3
5     Choi  NULL  100  3
```

```
King(1)
├── Lee(2)
└── Park(3)
    ├── Kim(4)
    └── Choi(5)
```

## Subject 1: data modeling

### Models

- Traits: 추상화 (abstraction), 단순화 (simplification), 명확화 (clarity).
- Three elements: things (엔터티), their attributes (속성), relationships (관계).
- Three views:
    - Data (what)
    - Process (how)
    - 상관 (interaction between data and process)
- Levels:
    - 개념 (conceptual): the big picture
    - 논리 (logical): keys, normalization
    - 물리 (physical): tables, data types
- 3-schema: 외부 (external, user views) / 개념 (conceptual, the whole DB) / 내부 (internal, storage).
    - Logical independence sits between external and conceptual.
    - Physical independence sits between conceptual and internal.
- Notations: IE (crow's foot) and Barker.
    - Circle = optional
    - Bar = mandatory
    - Crow's foot = many
- Peter Chen notation: rectangle = entity, diamond = relationship, ellipse = attribute.

### Entities

- Requirements:
    - The business needs it
    - It has a unique identifier
    - 2+ instances
    - Used by a business process
    - Has attributes
    - 1+ relationship (code and statistics entities can be exceptions)
- Classification:
    - By tangibility: 유형 (tangible), 개념 (conceptual), 사건 (event).
    - By when it arises: 기본 (basic, independent), 중심 (central, from basic), 행위 (action, from two or more parents).

### Attributes

- By characteristic: 기본 (basic), 설계 (designed, e.g. code), 파생 (derived, computed from other attributes).
- By role: PK / FK / 일반 (ordinary).
- By decomposition: 단일 (simple) / 복합 (composite, e.g. an address).
- By value count: 단일값 (single-valued) / 다중값 (multi-valued).
- Domain: the allowed range of values for an attribute.

### Relationships

- Notation: 관계명 (name), 차수 (degree, 1:1 / 1:M / M:N), 선택성 (optionality, 필수/선택).
- Kinds: 존재에 의한 (existence, e.g. an employee belongs to a dept) vs 행위에 의한 (action, e.g. a customer places an order).
- An M:N relationship is resolved with an intersection entity, which gives two 1:M relationships.
- 식별 relationship: the parent's PK becomes part of the child's PK (solid line), and the child is a weak entity.
- 비식별 relationship: the parent's PK is an ordinary FK column in the child (dotted line).
- Arc (exclusive) relationship: each child row relates to exactly one of several parents.

### Identifiers and keys

| Basis | Kinds |
|---|---|
| 대표성 | 주식별자 (primary) / 보조식별자 (secondary) |
| 생성 | 내부 (internal, its own) / 외부 (external, an FK from a parent) |
| 속성 수 | 단일 (single) / 복합 (compound) |
| 대체 여부 | 본질 (natural, from the business) / 인조 (artificial, e.g. a sequence) |

- Primary identifier properties: 유일성 (unique), 최소성 (minimal), 불변성 (never changes), 존재성 (never NULL).
- An artificial identifier keeps keys short, but risks duplicate business rows and needs an extra unique index on the natural key.
- Key kinds:
    - Super key: unique
    - Candidate key: unique and minimal
    - Primary key: the chosen candidate
    - Alternate key: the candidates not chosen
    - Foreign key: refers to another table's key

### Normalization

- Anomalies it removes:
    - Insert anomaly: you can't store a fact without an unrelated one.
    - Update anomaly: a fact stored twice gets updated in only one place.
    - Delete anomaly: deleting a row loses an unrelated fact.
- Normal forms:
    - 1NF: atomic values, no repeating groups
    - 2NF: no partial dependency on part of a composite key
    - 3NF: no transitive dependency (A → B → C)
    - BCNF: every determinant is a candidate key
    - 4NF: no multivalued dependency
    - 5NF: no join dependency
- Normalization reduces redundancy and writes, but adds joins.
- Supertype/subtype conversion:
    - One-to-One: a table each for the supertype and every subtype
    - Plus: a table per subtype, each carrying the supertype's columns
    - Single (all-in-one): one table
- NULL in modeling: a nullable FK makes the relationship optional. A nullable column changes aggregates and join results.

### Transactions

- ACID: atomicity, consistency, isolation, durability.
- Isolation levels and the anomalies each still allows:
    - READ UNCOMMITTED: dirty read possible
    - READ COMMITTED: non-repeatable read possible (the default in both Oracle and SQL Server)
    - REPEATABLE READ: phantom read possible
    - SERIALIZABLE: none of these
- The anomalies:
    - Dirty read: reading data that hasn't been committed.
    - Non-repeatable read: the same row gives a different value on a second read.
    - Phantom read: new rows appear on a second read.

## Subject 2: SQL basics

### Execution order

```
FROM → WHERE → GROUP BY → HAVING → SELECT → ORDER BY
```

- A SELECT alias works in ORDER BY but **not** in WHERE or GROUP BY, because both run before the alias exists. (Oracle 23ai allows it in GROUP BY. The exam follows the older rule.)
- To filter on an aggregate, use HAVING. WHERE runs before the groups exist.
- HAVING also works without GROUP BY: the whole table becomes one group.
- After GROUP BY, SELECT and ORDER BY may use only group keys and aggregates.
- ORDER BY accepts a position (`ORDER BY 2`), an alias or a column name.
- Oracle has `NULLS FIRST` / `NULLS LAST`. SQL Server has no such syntax.

### NULL means "unknown"

Comparing anything with unknown gives unknown, and that row is dropped.

- `NULL + 1` is NULL. `col = NULL` never matches. Write `col IS NULL`.
- Concatenation is the exception in Oracle: `'a' || NULL` = `'a'`, because NULL is treated as `''`. In SQL Server `'a' + NULL` is NULL.
- `COUNT(*)` counts rows (5). `COUNT(dept)` skips NULLs (4). `COUNT(DISTINCT col)` counts distinct non-NULL values.
- SUM, AVG, MAX and MIN skip NULLs, so `AVG` is the sum divided by the **non-NULL** count.
- On an empty table with no GROUP BY, you still get one row: `COUNT` is 0, and `SUM` / `MAX` are NULL. With GROUP BY you get 0 rows.
- GROUP BY puts all NULLs into a single group.
- `NOT IN (...a list containing NULL...)` returns **0 rows**, because `x <> NULL` is unknown for every x. `NOT EXISTS` doesn't have this problem.
- NULL functions:
    - `NVL(a,b)` (SQL Server `ISNULL`): a, or b if a is NULL
    - `NVL2(a,b,c)`: b if a is not NULL, otherwise c
    - `NULLIF(a,b)`: NULL if a = b, otherwise a
    - `COALESCE(a,b,...)`: the first non-NULL value
- Sorting: Oracle treats NULL as the **largest** value (last in ASC, first in DESC). SQL Server treats it as the smallest.
- In Oracle, `''` (empty string) is NULL.

### WHERE operators

- Precedence: `NOT` > `AND` > `OR`. Use parentheses when unsure.
- `BETWEEN a AND b` includes both ends. `IN (...)` is a chain of ORs.
- `LIKE`: `%` = any number of characters, `_` = exactly one. `LIKE '%\_%' ESCAPE '\'` matches a literal underscore.
- SQL Server only: `LIKE '[a-c]%'` matches a bracket range of characters.
- CHAR vs VARCHAR2: CHAR is blank-padded, so `'a '` = `'a'` is true when both are CHAR. Between VARCHAR2 values the comparison is exact.

### Functions

| Oracle | SQL Server |
|---|---|
| `NVL` | `ISNULL` |
| `SUBSTR` | `SUBSTRING` |
| `INSTR` | `CHARINDEX` |
| `LENGTH` | `LEN` |
| `CEIL` | `CEILING` |
| `SYSDATE` | `GETDATE()` |
| <code>'a' &#124;&#124; 'b'</code> | `'a' + 'b'` |

- `SUBSTR('SQLD',2,2)` = `'QL'` (start position, then length, counting from 1) · `INSTR('SQLD','L')` = 3 · `LPAD('7',3,'0')` = `'007'`
- `SUBSTR('DATABASE',-4,2)` = `'BA'`: a negative start counts from the end.
- `LTRIM('xxABx','x')` = `'ABx'`: the second argument is a **set** of characters to strip from the left. `TRIM(LEADING '0' FROM '00120')` = `'120'`.
- Oracle `CONCAT` takes exactly 2 arguments.
- `ROUND(123.456,1)` = 123.5 · `ROUND(123.456,-1)` = 120 · `TRUNC(123.456,1)` = 123.4
- CEIL always rounds **up**: `CEIL(-1.5)` = -1. FLOOR always rounds **down**: `FLOOR(-1.5)` = -2. `MOD(7,3)` = 1.
- Dates:
    - date + 1 = the next day · date + 1/24 = one hour later
    - `ADD_MONTHS(d,n)`, `MONTHS_BETWEEN(a,b)`, `LAST_DAY(d)`, `EXTRACT(YEAR FROM d)`
    - Conversion: `TO_CHAR(d,'YYYY-MM-DD HH24:MI:SS')` and `TO_DATE` (`HH24`: 24-hour clock, `MI`: minutes)
    - `TRUNC(d,'MM')` = the first day of the month.
    - BETWEEN trap: `TO_DATE('20260131','YYYYMMDD')` is midnight, so `d BETWEEN ... AND TO_DATE('20260131','YYYYMMDD')` misses the rest of Jan 31. Write `d < TO_DATE('20260201','YYYYMMDD')`.
- `CASE`:
    - Searched form: `CASE WHEN cond THEN ...`
    - Simple form: `CASE col WHEN val THEN ...`, which **can't test NULL**
    - With no ELSE, the result is NULL when nothing matches.
- `DECODE(x, a,'A', b,'B', 'other')` is Oracle-only, and it treats NULL = NULL as a match.

### JOIN

- INNER: both sides must match.
- LEFT / RIGHT: keep every row of that side, with NULLs filled in on the other.
- FULL: keep both sides. Row count = matched + unmatched left + unmatched right. NULL keys never match each other.
- CROSS: every pair (m × n rows).
- Self join: the same table under two aliases (`emp e JOIN emp m ON e.mgr = m.empno`).
- Non-equi join: a condition such as `ON e.sal BETWEEN g.lo AND g.hi`.
- `NATURAL JOIN` joins on **every** same-named column. `USING (col)` joins on just the named ones. Either way, the join column takes **no table prefix**. SQL Server supports neither.
- Oracle `(+)` goes on the side that **might be empty**: `e.dept = d.dept(+)` is `e LEFT JOIN d`.
    - Every condition on the optional table needs `(+)`. Without it, the outer join becomes an inner join.
    - `(+)` can't be combined with OR or IN, and there is no FULL join with `(+)`.
- ANSI trap: a condition on the right table in **WHERE** turns a LEFT join back into an INNER one. Put it in **ON** to keep the left rows.

## Subject 2: SQL advanced

### Subqueries

- A single-row operator (`=`, `<`) against a multi-row result is an **error**. Use `IN`, `ANY` or `ALL`.
- `> ALL` means greater than the **max**. `> ANY` means greater than the **min**.
- `= ANY` is the same as `IN`. `<> ALL` is the same as `NOT IN`. Against an empty result, `ALL` is TRUE and `ANY` is FALSE.
- `EXISTS` asks whether the subquery returned any row. The values don't matter, so `SELECT 1` is fine.
- Correlated subquery: the inner query refers to the outer row and runs once per outer row.
- Where subqueries go:
    - In SELECT: a scalar subquery, which must return 1 value. 0 rows gives NULL. 2+ rows is an error (ORA-01427).
    - In FROM: an inline view, the only kind that can take ORDER BY.
    - In WHERE: a nested subquery.
- Multi-column: `WHERE (dept, sal) IN (SELECT dept, MAX(sal) ... GROUP BY dept)`. A pair containing NULL, such as (NULL, 100), never matches.

### Set operators

- `UNION` removes duplicates. `UNION ALL` keeps them, so it does no dedup work and is the cheapest.
- Set operators treat NULLs as **equal**: UNION keeps one NULL row, even though `NULL = NULL` is unknown in WHERE and joins.
- `INTERSECT`: rows in both. `MINUS` (SQL Server `EXCEPT`): rows in the first but not the second.
- Precedence:
    - Oracle evaluates all set operators left to right, with equal precedence.
    - SQL Server evaluates INTERSECT first.
- Column names come from the **first** query. `ORDER BY` goes once, at the very end. Column counts and types must match.

### ROLLUP, CUBE, GROUPING SETS

| Syntax | Groupings produced |
|---|---|
| `ROLLUP(a,b)` | (a,b), (a), () — a staircase, n+1 levels, **order matters** |
| `CUBE(a,b)` | (a,b), (a), (b), () — every combination, 2ⁿ |
| `GROUPING SETS(a,b)` | (a), (b) — only what you list, **no grand total** unless you add `()` |
| `GROUP BY a, ROLLUP(b)` | (a,b), (a) — partial rollup, no grand total |

- `GROUPING(col) = 1` means "this NULL is a subtotal label, not real data". It's usually wrapped in CASE or DECODE to print 'Total'.
- `GROUPING_ID(a,b)` turns those flags into bits: (a,b) gives 0, (a) gives 1, () gives 3.

### Window functions (OVER)

`OVER` computes an aggregate **but keeps every row**. GROUP BY collapses rows; OVER writes the result back onto each row.

```sql
SELECT dept, SUM(sal) FROM emp GROUP BY dept;                -- 3 rows
SELECT name, SUM(sal) OVER (PARTITION BY dept) FROM emp;     -- 5 rows, each with its dept total
```

```
OVER ( PARTITION BY dept      -- which group I belong to
       ORDER BY sal DESC      -- order inside the group
       ROWS ... )             -- which slice of the group to add up
```

Think of the rows as an array sorted by the ORDER BY, with you standing at index `i`:

- `PRECEDING`: rows above me (smaller index)
- `FOLLOWING`: rows below me (bigger index)
- `UNBOUNDED`: all the way to the edge
- `CURRENT ROW`: me

```cpp
// sorted by sal DESC: 500, 300, 300, 200, 100
for (int i = 0; i < n; i++) {
    int lo = 0, hi = i;                // ROWS UNBOUNDED PRECEDING   (running total)
    // lo = i > 0 ? i - 1 : 0; hi = i; // ROWS BETWEEN 1 PRECEDING AND CURRENT ROW
    // lo = 0; hi = n - 1;             // UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING
    int s = 0;
    for (int k = lo; k <= hi; k++) s += emp[k].sal;
    out[i] = s;
}
```

- **No ORDER BY**: the frame is the whole partition.
- **ORDER BY with no frame**: the frame is `RANGE BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW`, which takes in tied values.
    - `SUM(sal) OVER (ORDER BY sal DESC)` gives 500, **1100, 1100**, 1300, 1400.
    - With `ROWS UNBOUNDED PRECEDING` it gives 500, **800, 1100**, 1300, 1400.
- `ROWS` counts physical rows. `RANGE` compares values: `RANGE BETWEEN 100 PRECEDING AND CURRENT ROW` means sal within 100 of mine.
- **LAST_VALUE trap:** with the default frame it returns the current row's value (or the last of its ties). For the true last value you need `ROWS BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING`.
- Ranking on 500, 300, 300, 200: `RANK` 1, 2, 2, **4** · `DENSE_RANK` 1, 2, 2, **3** · `ROW_NUMBER` 1, 2, 3, 4.
- Row access:
    - `LAG(col, n, default)` / `LEAD(...)`: the value n rows back / ahead
    - `FIRST_VALUE` / `LAST_VALUE`: the value at the start / end of the frame
- Distribution:
    - `NTILE(3)` on 5 rows gives bucket sizes 2, 2, 1. Leftover rows go to the first buckets.
    - `PERCENT_RANK` = (rank − 1) / (n − 1)
    - `CUME_DIST` = (rows ≤ me, ties included) / n
    - `RATIO_TO_REPORT(sal)` = sal / the partition total

### Top-N

- Oracle assigns `ROWNUM` **before** ORDER BY. To get a top 3 by salary, sort in an inline view first, then apply `ROWNUM <= 3` outside it.
- `WHERE ROWNUM = 2` returns nothing: row 1 never qualifies, so there is never a row 2. `ROWNUM <= n` and `ROWNUM = 1` work.
- Oracle 12c+: `ORDER BY sal DESC FETCH FIRST 3 ROWS ONLY` (or `WITH TIES`). `OFFSET 5 ROWS` skips rows.
- SQL Server: `TOP(n)`, `TOP(n) PERCENT`, and `TOP(n) WITH TIES`, which requires ORDER BY.

### Hierarchical queries (CONNECT BY)

`PRIOR` marks a column of **the row I'm standing on**. A column without PRIOR belongs to the candidate next row.

```
PRIOR empno = mgr   →  walks DOWN (boss → staff)
PRIOR mgr = empno   →  walks UP   (staff → boss)
```

```cpp
// CONNECT BY PRIOR empno = mgr: find rows whose mgr is me → my children
void walk(int c, int level) {
    printf("%s %d\n", emp[c].name, level);
    for (int r = 0; r < n; r++)
        if (emp[c].empno == emp[r].mgr)    // PRIOR empno = mgr
            walk(r, level + 1);
}
// START WITH mgr IS NULL → King 1, Lee 2, Park 2, Kim 3, Choi 3
// (CONNECT BY PRIOR mgr = empno from Kim → Kim, Park, King)
```

- `START WITH` picks the roots. Without it, **every** row starts its own tree. LEVEL starts at 1 at the root.
- Row generator: `SELECT LEVEL FROM dual CONNECT BY LEVEL <= 3` returns 1, 2, 3.
- START WITH rows are never tested against the CONNECT BY condition.
- With `START WITH mgr IS NULL`:
    - A condition in **CONNECT BY** cuts the whole branch: `CONNECT BY PRIOR empno = mgr AND name <> 'Park'` returns King, Lee.
    - A condition in **WHERE** runs after the tree is built and removes only that row: `WHERE name <> 'Park'` returns King, Lee, Kim, Choi.
- Pseudo-columns and functions:
    - `CONNECT_BY_ISLEAF = 1`: the row has no children (Lee, Kim, Choi)
    - `CONNECT_BY_ROOT name`: the root's value (King)
    - `SYS_CONNECT_BY_PATH(name,'/')`: a path such as `/King/Park/Kim`
- `ORDER SIBLINGS BY` sorts rows that share a parent without breaking the tree.
- `NOCYCLE` stops at loops, and `CONNECT_BY_ISCYCLE = 1` marks where a loop was found.
- SQL Server has no CONNECT BY. It uses a recursive CTE (`WITH ... UNION ALL`).

### PIVOT and regular expressions

- `PIVOT (SUM(sal) FOR dept IN (10, 20))` turns rows into columns. `UNPIVOT` turns columns into rows.
    - Any source column not used in PIVOT becomes an implicit GROUP BY key, so pivot from an inline view that selects only the columns you need.
    - The alias in `IN (10 AS d10)` becomes the result column name.
    - UNPIVOT drops NULL values unless written `UNPIVOT INCLUDE NULLS`.
- Symbols:
    - `.` any character · `^` start · `$` end · `|` or
    - `*` 0 or more · `+` 1 or more · `?` 0 or 1
    - `{n}` exactly n · `{n,}` n or more · `{n,m}` between n and m
    - `[abc]` one of these · `[^abc]` none of these
    - `\d` a digit, the same as `[[:digit:]]` (also `[[:alpha:]]`, `[[:space:]]`)
    - `\.` a literal dot
    - `\1` back-reference to the first group
- Matching is greedy by default. `*?` and `+?` match as little as possible.
- Functions:
    - `REGEXP_LIKE`: filter rows
    - `REGEXP_SUBSTR`: extract
    - `REGEXP_REPLACE`: replace
    - `REGEXP_INSTR`: position
    - `REGEXP_COUNT`: count matches
- Argument order: `REGEXP_SUBSTR(src, pattern, position, occurrence, match_param)`. match_param `'i'` = case-insensitive.
    - `REGEXP_SUBSTR('a1b22c333','[0-9]+',1,2)` = `'22'` (the 2nd match, starting from position 1)

## Subject 2: management statements

### Command types

```
DDL  CREATE ALTER DROP RENAME TRUNCATE   structure
DML  SELECT INSERT UPDATE DELETE MERGE   data
DCL  GRANT REVOKE                        permissions
TCL  COMMIT ROLLBACK SAVEPOINT           transactions
```

### DML

- `INSERT INTO t (c1, c2) VALUES (...)` or `INSERT INTO t SELECT ...`. Leaving a column out gives it its DEFAULT, or NULL if it has none.
- `UPDATE t SET c = (SELECT ...)` is allowed, including correlated subqueries.
- MERGE:

```sql
MERGE INTO t
USING s ON (t.id = s.id)
WHEN MATCHED     THEN UPDATE SET t.v = s.v  [DELETE WHERE ...]
WHEN NOT MATCHED THEN INSERT (id, v) VALUES (s.id, s.v);
```

- `DELETE WHERE` applies only to rows this MERGE just updated, and tests their **updated** values.

### DDL

| | Oracle | SQL Server |
|---|---|---|
| Change a column | `ALTER TABLE t MODIFY (c VARCHAR2(20))` | `ALTER TABLE t ALTER COLUMN c VARCHAR(20)` |
| Rename a column | `ALTER TABLE t RENAME COLUMN a TO b` | `sp_rename` |
| Add / drop a column | `ADD (c ...)` / `DROP COLUMN c` | `ADD c ...` / `DROP COLUMN c` |

- `CREATE TABLE t2 AS SELECT ...` (CTAS) copies the data and the explicitly declared NOT NULL constraints only: no PK, FK, UNIQUE or CHECK. The NOT NULL that a PK puts on its column is not copied, so that column becomes nullable.
- Names: start with a letter, use only A–Z, 0–9, `_`, `$` and `#`, and can't be reserved words.
- SQL Server auto-number: `id INT IDENTITY(1,1)` (start 1, step 1).
- `DROP TABLE t CASCADE CONSTRAINTS` also drops the FKs that point at t.
- `DELETE` removes rows and can be rolled back until COMMIT. `TRUNCATE` removes all rows but keeps the table, and can't be rolled back in Oracle. `DROP` removes the table itself.
- A view is a stored query and holds no data. Views with joins, GROUP BY or DISTINCT are restricted for DML.

### Constraints

- PK = UNIQUE + NOT NULL, one per table.
- UNIQUE allows NULLs: any number in Oracle, but only **one** in SQL Server.
- FK: the child's value must exist in the parent, or be NULL.
    - `ON DELETE CASCADE`: deleting a parent deletes its children.
    - `ON DELETE SET NULL`: the children's FK becomes NULL.
- `CHECK (sal > 0)` enforces a condition, but **lets NULL through**: the check fails only on FALSE, and NULL gives unknown. `DEFAULT` supplies the value when an INSERT leaves the column out.

### TCL

- Oracle opens a transaction implicitly and waits for COMMIT. **DDL commits automatically**, and that also commits any pending DML before it.
- SQL Server defaults to autocommit, so each statement commits at once. Inside an explicit `BEGIN TRAN`, even DDL and TRUNCATE can be rolled back.
- `ROLLBACK TO sp1` undoes only the work after sp1. A plain `ROLLBACK` undoes everything since the last COMMIT.

### DCL

- `GRANT SELECT ON emp TO u1` gives an object privilege. `GRANT CREATE TABLE TO u1` gives a system privilege.
- `WITH GRANT OPTION` (object privileges): u1 can grant it on, and revoking it from u1 **cascades** to everyone u1 granted it to.
- `WITH ADMIN OPTION` (system privileges): the revoke does **not** cascade.
- A ROLE bundles privileges: `CREATE ROLE r; GRANT ... TO r; GRANT r TO u1`.
