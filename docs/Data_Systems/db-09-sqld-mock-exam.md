---
title: "SQLD Mock Exam: 50 Original Questions"
---

# SQLD Mock Exam: 50 Original Questions

Fifty original questions in the real split: 10 data modeling, 40 SQL. Each one is built on a trap that recurs in the 2025–2026 rounds (57–61). The patterns came from candidate reviews and third-party restorations of those rounds, and the question text is new. Oracle unless a question says otherwise. Answers and one-line explanations are at the end, and the rules behind them are in the [SQLD cheat sheet](db-08-sqld-cheat-sheet.md).

Give yourself 90 minutes for the full set, as on the real exam.

## Tables

```
EMP                                   DEPT
empno name  dept  sal  mgr            dept dname loc
1     King  10    500  NULL           10   Sales Seoul
2     Lee   10    300  1              20   Dev   Busan
3     Park  20    300  1              30   HR    Seoul
4     Kim   20    200  3
5     Choi  NULL  100  3              SALES
                                      region product amt
                                      East   A       10
                                      East   B       20
                                      West   A       30
```

```
King(1)
├── Lee(2)
└── Park(3)
    ├── Kim(4)
    └── Choi(5)
```

## Subject 1: data modeling

**Q1.** Classify 사원 (employee), 주문 (order) and 주문목록 (order line) by when the entity arises.

- (1) 기본, 중심, 행위
- (2) 중심, 기본, 행위
- (3) 기본, 행위, 중심
- (4) 행위, 중심, 기본

**Q2.** In `ORDER_LINE(order_id, product_id, unit_price, qty, line_amount)`, line_amount is always unit_price × qty. What kind of attribute is line_amount?

- (1) 기본 (basic)
- (2) 설계 (designed)
- (3) 파생 (derived)
- (4) 복합 (composite)

**Q3.** A sequence-generated `order_seq` replaces the composite PK `(order_id, product_id)`. Which is correct?

- (1) order_seq is a 본질식별자 (natural identifier).
- (2) order_seq is an 인조식별자 (artificial identifier), and `(order_id, product_id)` now needs its own unique constraint to stop duplicate business rows.
- (3) order_seq is an 외부식별자 (external identifier).
- (4) An artificial identifier can never be a primary identifier.

**Q4.** In EMP, empno alone is unique, never NULL and never changes. Someone defines the PK as `(empno, name)`. Which primary-identifier property does this violate?

- (1) 유일성 (uniqueness)
- (2) 불변성 (immutability)
- (3) 존재성 (never NULL)
- (4) 최소성 (minimality)

**Q5.** The DEPT (parent) → EMP (child) relationship changes from 비식별 (non-identifying) to 식별 (identifying). What happens?

- (1) dept becomes part of EMP's PK, and the line becomes solid.
- (2) dept becomes an ordinary column in EMP, and the line becomes dotted.
- (3) EMP's PK stays the same and dept is removed.
- (4) DEPT's PK becomes an alternate key of EMP.

**Q6.** STUDENT and COURSE have an M:N relationship. What is the standard logical-model treatment?

- (1) Add course_id to STUDENT as a multi-valued attribute.
- (2) Add an intersection entity ENROLLMENT, giving two 1:M relationships.
- (3) Merge the two into one entity.
- (4) Keep the M:N into the physical model unchanged.

**Q7.** `ORDER_LINE(order_id, product_id, qty, product_name)` has the PK `(order_id, product_id)`, and product_name depends only on product_id. Which normal form does it violate first?

- (1) 1NF
- (2) 2NF
- (3) 3NF
- (4) BCNF

**Q8.** `EMP(empno, name, dept, dname)`, where dname depends on dept. Deleting the last employee of dept 30 also loses the name "HR". Which anomaly is this, and what is the fix?

- (1) Insert anomaly; move to 2NF
- (2) Update anomaly; move to 1NF
- (3) Delete anomaly; split DEPT into its own table (3NF)
- (4) Delete anomaly; denormalize

**Q9.** A DBA moves a table to new storage and adds an index. Neither the conceptual schema nor any application changes. Which property made this possible?

- (1) Logical independence
- (2) Physical independence
- (3) External independence
- (4) Transaction independence

**Q10.** Supertype PAYMENT has subtypes CARD and CASH. It is built as one table holding every column, and the card-only columns are NULL on cash rows. What is this conversion called?

- (1) One-to-One
- (2) Plus
- (3) Single (all-in-one)
- (4) Arc

## Subject 2: SQL

### NULL

**Q11.**
```sql
SELECT COUNT(*), COUNT(dept), COUNT(DISTINCT sal) FROM emp;
```
- (1) 5, 5, 5
- (2) 5, 4, 5
- (3) 4, 4, 4
- (4) 5, 4, 4

**Q12.**
```sql
SELECT SUM(sal), COUNT(*) FROM emp WHERE dept = 30;
```
- (1) No rows
- (2) 0, 0
- (3) NULL, 0
- (4) NULL, NULL

**Q13.** How many rows does each query return?
```sql
-- A
SELECT name FROM emp WHERE empno NOT IN (SELECT mgr FROM emp);
-- B
SELECT name FROM emp e
WHERE NOT EXISTS (SELECT 1 FROM emp m WHERE m.mgr = e.empno);
```
- (1) 3, 3
- (2) 0, 3
- (3) 3, 0
- (4) 2, 3

**Q14.**
```sql
SELECT NVL2(mgr, 'Y', 'N'), NULLIF(sal, 300), COALESCE(dept, mgr, 0)
FROM emp WHERE empno = 5;
```
- (1) N, 100, 0
- (2) Y, NULL, 3
- (3) Y, 100, 3
- (4) Y, 100, 0

**Q15.**
```sql
SELECT AVG(CASE WHEN dept = 10 THEN sal END),
       AVG(CASE WHEN dept = 10 THEN sal ELSE 0 END)
FROM emp;
```
- (1) 400, 400
- (2) 160, 160
- (3) 400, 160
- (4) 160, 400

**Q16.** Which name comes first?
```sql
SELECT name FROM emp ORDER BY dept DESC, empno;
```
- (1) King
- (2) Park
- (3) Kim
- (4) Choi

**Q17.** What do these two expressions return for Choi?
```sql
CASE dept WHEN NULL THEN 'none' ELSE 'has' END
DECODE(dept, NULL, 'none', 'has')
```
- (1) none, none
- (2) has, has
- (3) has, none
- (4) none, has

### Joins and subqueries

**Q18.** How many rows?
```sql
SELECT * FROM emp e FULL OUTER JOIN dept d ON e.dept = d.dept;
```
- (1) 4
- (2) 5
- (3) 7
- (4) 6

**Q19.** How many rows does each query return?
```sql
-- A
SELECT e.name, d.dname
FROM emp e LEFT JOIN dept d ON e.dept = d.dept AND d.loc = 'Seoul';
-- B
SELECT e.name, d.dname
FROM emp e LEFT JOIN dept d ON e.dept = d.dept
WHERE d.loc = 'Seoul';
```
- (1) 5, 5
- (2) 2, 2
- (3) 5, 2
- (4) 2, 5

**Q20.** Which ANSI query is equivalent to this one?
```sql
SELECT e.name, d.dname
FROM emp e, dept d
WHERE e.dept = d.dept(+) AND d.loc(+) = 'Seoul';
```
- (1) `emp e LEFT JOIN dept d ON e.dept = d.dept WHERE d.loc = 'Seoul'`
- (2) `emp e LEFT JOIN dept d ON e.dept = d.dept AND d.loc = 'Seoul'`
- (3) `emp e RIGHT JOIN dept d ON e.dept = d.dept AND d.loc = 'Seoul'`
- (4) `emp e JOIN dept d ON e.dept = d.dept AND d.loc = 'Seoul'`

**Q21.** Which statement raises an error?

- (1) `SELECT name, dname FROM emp NATURAL JOIN dept;`
- (2) `SELECT dept, dname FROM emp JOIN dept USING (dept);`
- (3) `SELECT e.dept, d.dname FROM emp e NATURAL JOIN dept d;`
- (4) `SELECT e.name, d.dname FROM emp e JOIN dept d ON e.dept = d.dept;`

**Q22.** How many rows does each query return?
```sql
-- A
SELECT name FROM emp WHERE sal > ALL (SELECT sal FROM emp WHERE dept = 20);
-- B
SELECT name FROM emp WHERE sal > ALL (SELECT sal FROM emp WHERE dept = 30);
```
- (1) 1, 0
- (2) 1, 5
- (3) 3, 0
- (4) 3, 5

**Q23.**
```sql
SELECT name FROM emp
WHERE (dept, sal) IN (SELECT dept, MAX(sal) FROM emp GROUP BY dept);
```
- (1) King, Park, Choi
- (2) King, Lee, Park
- (3) King
- (4) King, Park

**Q24.** Which statement raises an error?

- (1) `SELECT name, (SELECT dname FROM dept d WHERE d.dept = e.dept) FROM emp e;`
- (2) `SELECT name, (SELECT dname FROM dept WHERE loc = 'Seoul') FROM emp;`
- (3) `SELECT name FROM emp WHERE sal = (SELECT MAX(sal) FROM emp);`
- (4) `SELECT name FROM emp WHERE dept IN (SELECT dept FROM dept WHERE loc = 'Seoul');`

### Set operators and group functions

**Q25.** In Oracle, how many rows?
```sql
SELECT dept FROM emp
UNION
SELECT dept FROM dept
INTERSECT
SELECT dept FROM dept WHERE loc = 'Seoul';
```
- (1) 2
- (2) 3
- (3) 5
- (4) 4

**Q26.** Row counts of `UNION ALL`, `UNION` and `MINUS`:
```sql
SELECT dept FROM emp  UNION ALL SELECT dept FROM dept;
SELECT dept FROM emp  UNION     SELECT dept FROM dept;
SELECT dept FROM dept MINUS     SELECT dept FROM emp;
```
- (1) 8, 5, 1
- (2) 7, 4, 1
- (3) 8, 4, 2
- (4) 8, 4, 1

**Q27.** On SALES, how many rows does each of `GROUP BY ROLLUP(region, product)`, `CUBE(region, product)` and `GROUPING SETS(region, product)` return?

- (1) 6, 8, 4
- (2) 6, 8, 5
- (3) 5, 8, 4
- (4) 6, 9, 4

**Q28.** In the result of this query, what is `GROUPING_ID` on the row (NULL, 'A')?
```sql
SELECT region, product, GROUPING_ID(region, product)
FROM sales GROUP BY CUBE(region, product);
```
- (1) 0
- (2) 1
- (3) 2
- (4) 3

**Q29.** How many result rows have dept = NULL, and how many of those have `GROUPING(dept) = 1`?
```sql
SELECT dept, COUNT(*), GROUPING(dept) FROM emp GROUP BY ROLLUP(dept);
```
- (1) 1, 1
- (2) 2, 1
- (3) 2, 2
- (4) 1, 0

### Window functions and Top-N

**Q30.** What is Lee's value?
```sql
SELECT name, SUM(sal) OVER (ORDER BY sal) FROM emp;
```
- (1) 600
- (2) 300
- (3) 1400
- (4) 900

**Q31.** How many rows?
```sql
SELECT * FROM (
  SELECT name, RANK() OVER (PARTITION BY dept ORDER BY sal DESC) r FROM emp
) WHERE r = 1;
```
- (1) 1
- (2) 2
- (3) 3
- (4) 4

**Q32.**
```sql
SELECT name,
       sal - LAG(sal) OVER (ORDER BY empno) AS d,
       LAG(sal, 1, 0) OVER (ORDER BY empno) AS l
FROM emp;
```
Which is correct?

- (1) King: d = NULL, l = 0. Lee: d = -200.
- (2) King: d = 0, l = 0. Lee: d = 200.
- (3) King: d = NULL, l = NULL. Lee: d = -200.
- (4) King: d = 500, l = 0. Lee: d = -200.

**Q33.** With `NTILE(3) OVER (ORDER BY sal DESC)`, which bucket is Kim in, and what are the bucket sizes?

- (1) 3; 2, 2, 1
- (2) 2; 2, 2, 1
- (3) 2; 1, 2, 2
- (4) 3; 1, 2, 2

**Q34.** What is King's value?
```sql
SELECT name, LAST_VALUE(sal) OVER (ORDER BY sal DESC) FROM emp;
```
- (1) 100
- (2) 500
- (3) 1400
- (4) NULL

**Q35.** Which query correctly returns the two lowest-paid employees?

- (1) `SELECT name FROM emp WHERE ROWNUM <= 2 ORDER BY sal;`
- (2) `SELECT name FROM emp WHERE ROWNUM BETWEEN 1 AND 2 ORDER BY sal;`
- (3) `SELECT name FROM (SELECT name, ROWNUM rn FROM emp ORDER BY sal) WHERE rn <= 2;`
- (4) `SELECT name FROM (SELECT name FROM emp ORDER BY sal) WHERE ROWNUM <= 2;`

**Q36.** In Oracle 12c or later, how many rows?
```sql
SELECT name FROM emp ORDER BY sal DESC FETCH FIRST 2 ROWS WITH TIES;
```
- (1) 1
- (2) 2
- (3) 3
- (4) 5

### Hierarchical queries

**Q37.** There is no START WITH. How many rows?
```sql
SELECT COUNT(*) FROM emp CONNECT BY PRIOR empno = mgr;
```
- (1) 5
- (2) 9
- (3) 11
- (4) Error

**Q38.** Which is correct?
```sql
SELECT name, LEVEL, CONNECT_BY_ISLEAF, SYS_CONNECT_BY_PATH(name, '/')
FROM emp
START WITH empno = 3
CONNECT BY PRIOR empno = mgr;
```
- (1) 3 rows. Kim: LEVEL 2, ISLEAF 1, path `/Park/Kim`.
- (2) 3 rows. Kim: LEVEL 3, ISLEAF 1, path `/King/Park/Kim`.
- (3) 2 rows: Park, King.
- (4) 3 rows. Park: ISLEAF 1.

**Q39.** Which rows come back?
```sql
SELECT name FROM emp START WITH name = 'Choi' CONNECT BY PRIOR mgr = empno;
```
- (1) Choi
- (2) Choi, Park, King
- (3) Choi, Kim
- (4) Choi, Park

### Functions, LIKE, regular expressions

**Q40.**
```sql
SELECT SUBSTR('DATABASE', -4, 2), LTRIM('xxABx', 'x'), ROUND(1234.5, -2) FROM dual;
```
- (1) BA, ABx, 1200
- (2) AS, AB, 1200
- (3) BA, ABx, 1300
- (4) SE, ABx, 1200

**Q41.** Column `d` is a DATE that includes a time of day. Which condition **misses** a row with d = 2026-01-31 14:00?

- (1) `d >= TO_DATE('20260101','YYYYMMDD') AND d < TO_DATE('20260201','YYYYMMDD')`
- (2) `TRUNC(d) BETWEEN TO_DATE('20260101','YYYYMMDD') AND TO_DATE('20260131','YYYYMMDD')`
- (3) `TO_CHAR(d, 'YYYYMM') = '202601'`
- (4) `d BETWEEN TO_DATE('20260101','YYYYMMDD') AND TO_DATE('20260131','YYYYMMDD')`

**Q42.** A column holds `'A_1'`, `'AB1'` and `'A%1'`. Which values match `code LIKE 'A\_%' ESCAPE '\'`?

- (1) `A_1` only
- (2) `A_1`, `AB1`
- (3) All three
- (4) `A_1`, `A%1`

**Q43.**
```sql
SELECT REGEXP_SUBSTR('a1b22c333', '[0-9]+', 1, 2),
       REGEXP_COUNT('1.2.3', '.'),
       REGEXP_COUNT('1.2.3', '\.')
FROM dual;
```
- (1) 22, 5, 2
- (2) 2, 5, 2
- (3) 22, 2, 2
- (4) 333, 5, 2

**Q44.**
```sql
SELECT REGEXP_SUBSTR('<a><b>', '<.+>'),
       REGEXP_SUBSTR('<a><b>', '<.+?>'),
       REGEXP_REPLACE('2026-11-30', '(\d+)-(\d+)-(\d+)', '\3/\2/\1')
FROM dual;
```
- (1) `<a>`, `<a>`, `30/11/2026`
- (2) `<a><b>`, `<a>`, `30/11/2026`
- (3) `<a><b>`, `<a><b>`, `2026/11/30`
- (4) `<a>`, `<a><b>`, `30/11/2026`

### PIVOT and UNPIVOT

**Q45.** What does this return?
```sql
SELECT *
FROM (SELECT mgr, dept, sal FROM emp)
PIVOT (SUM(sal) FOR dept IN (10 AS d10, 20 AS d20));
```
- (1) 1 row; columns D10, D20
- (2) 3 rows; columns MGR, D10, D20
- (3) 5 rows; columns MGR, D10, D20
- (4) 3 rows; columns D10, D20

**Q46.** Table Q(id, q1, q2) holds (1, 10, NULL) and (2, 20, 30). How many rows does `SELECT * FROM q UNPIVOT (amt FOR qtr IN (q1, q2))` return, and how many with `UNPIVOT INCLUDE NULLS`?

- (1) 4, 4
- (2) 3, 4
- (3) 3, 3
- (4) 2, 4

### Management statements

**Q47.** Table T starts empty. After this sequence, which rows remain?
```sql
INSERT INTO t VALUES ('A');
COMMIT;
INSERT INTO t VALUES ('B');
SAVEPOINT s1;
INSERT INTO t VALUES ('C');
ROLLBACK TO s1;
INSERT INTO t VALUES ('D');
CREATE TABLE t2 (x NUMBER);
INSERT INTO t VALUES ('E');
ROLLBACK;
```
- (1) A
- (2) A, D
- (3) A, B, D, E
- (4) A, B, D

**Q48.** T holds (1,'a') and (2,'b'). S holds (2,'x') and (3,'y'). What is in T afterwards?
```sql
MERGE INTO t
USING s ON (t.id = s.id)
WHEN MATCHED THEN
  UPDATE SET t.v = s.v
  DELETE WHERE t.v = 'x'
WHEN NOT MATCHED THEN
  INSERT (id, v) VALUES (s.id, s.v);
```
- (1) (1,a), (2,x), (3,y)
- (2) (1,a), (3,y)
- (3) (1,a), (2,b), (3,y)
- (4) (2,x), (3,y)

**Q49.** After this sequence, who can still SELECT from u1.emp?
```sql
-- as u1
GRANT SELECT ON emp TO u2 WITH GRANT OPTION;
-- as u2
GRANT SELECT ON u1.emp TO u3;
-- as u1
REVOKE SELECT ON emp FROM u2;
```
- (1) u2 and u3
- (2) u3 only
- (3) u2 only
- (4) Neither u2 nor u3

**Q50.** Which statement is **wrong**?

- (1) `CREATE TABLE emp2 AS SELECT * FROM emp` copies explicitly declared NOT NULL constraints but not the PK.
- (2) TRUNCATE keeps the table structure and cannot be rolled back.
- (3) A UNIQUE column can hold several NULLs.
- (4) `CHECK (sal > 0)` rejects an INSERT whose sal is NULL.

## Answers

✓ means the answer was run in DuckDB. **O** means the behavior is Oracle-specific, so the answer was worked out by hand against Oracle semantics. **R** means a regex result checked against Oracle's regular-expression documentation. Every answer was also re-solved by an independent reviewer.

| Q | Ans | Why |
|---|---|---|
| 1 | 1 | Employee is 기본 (basic, exists on its own). Order is 중심 (central, arises from basic entities). Order line is 행위 (action, arises from two or more parents). |
| 2 | 3 | Computable from other attributes, so it's 파생 (derived). |
| 3 | 2 | An artificial key doesn't stop the same business row being inserted twice, so the natural key needs a unique constraint. |
| 4 | 4 | empno alone is already unique, so adding name breaks minimality. |
| 5 | 1 | Identifying relationship: the parent's PK moves into the child's PK, drawn as a solid line. |
| 6 | 2 | M:N is resolved with an intersection entity. |
| 7 | 2 | product_name depends on part of the composite key, which is a partial dependency. |
| 8 | 3 | Losing an unrelated fact on delete is a delete anomaly. The fix removes the transitive dependency empno → dept → dname. |
| 9 | 2 | Changing storage without touching the conceptual schema is physical independence. |
| 10 | 3 | One table with NULL-padded subtype columns is the Single (all-in-one) type. |
| 11 | 4 | ✓ 5 rows, 4 non-NULL depts, 4 distinct salaries (500, 300, 200, 100). |
| 12 | 3 | ✓ With no GROUP BY you always get one row. SUM over nothing is NULL, COUNT is 0. |
| 13 | 2 | ✓ The mgr list contains NULL, so NOT IN matches nothing. NOT EXISTS returns Lee, Kim, Choi. |
| 14 | 3 | Choi's mgr is 3, so NVL2 gives Y. NULLIF(100,300) = 100. COALESCE skips the NULL dept and returns 3. (✓ for NULLIF and COALESCE; NVL2 is **O**.) |
| 15 | 3 | ✓ The first AVG skips the NULLs: 800/2. ELSE 0 turns them into zeros: 800/5. |
| 16 | 4 | **O** Oracle sorts NULL as the largest value, so it comes first in DESC. |
| 17 | 3 | **O** `CASE dept WHEN NULL` tests `dept = NULL`, which is never true. DECODE treats NULL = NULL as a match. |
| 18 | 4 | ✓ 4 matched rows + Choi (no dept) + dept 30 (no employees). |
| 19 | 3 | ✓ A filter in ON keeps every left row. The same filter in WHERE drops the NULL-filled rows. |
| 20 | 2 | **O** `(+)` on every condition for d keeps it inside the outer join, which is ANSI ON. |
| 21 | 3 | **O** The NATURAL JOIN column can't take a table prefix (ORA-25155). |
| 22 | 2 | ✓ A: greater than 300 is King only. B: ALL over an empty set is TRUE, so all 5 rows. |
| 23 | 4 | ✓ The (NULL, 100) group never matches, because NULL = NULL is unknown. |
| 24 | 2 | Two departments are in Seoul, so the scalar subquery returns 2 rows (ORA-01427). Option 1 returns NULL for Choi, which is fine. |
| 25 | 1 | **O** Oracle evaluates set operators left to right: {10, 20, 30, NULL} ∩ {10, 30} = 2 rows. SQL Server and DuckDB do INTERSECT first and give 4 (✓). Oracle's docs warn this may change in a future release, so use parentheses in real code. |
| 26 | 4 | ✓ UNION treats the NULLs as duplicates, giving 10, 20, 30, NULL. MINUS leaves dept 30. |
| 27 | 1 | ✓ ROLLUP: 3 + 2 + 1. CUBE adds 2 product subtotals. GROUPING SETS: 2 + 2. |
| 28 | 3 | ✓ region rolled up = bit 1 (value 2), product kept = bit 0. |
| 29 | 2 | ✓ Choi's real NULL group (GROUPING 0) and the grand total (GROUPING 1). |
| 30 | 4 | ✓ The default RANGE frame includes Park's tied 300: 100 + 200 + 300 + 300. |
| 31 | 3 | ✓ One top earner per partition: 10, 20 and the NULL partition. |
| 32 | 1 | ✓ King has no previous row, so the difference is NULL. The default argument 0 fills l. |
| 33 | 2 | ✓ 5 rows into 3 buckets: the remainder goes to the first buckets (2, 2, 1). Kim is the 4th row. |
| 34 | 2 | ✓ The default RANGE frame ends at the last row tied with the current one, so LAST_VALUE is the row's own sal. |
| 35 | 4 | **O** ROWNUM is assigned before ORDER BY, so sort in an inline view first. Option 3 numbers the rows before sorting. |
| 36 | 3 | **O** King (500), plus Lee and Park tied at 300 for 2nd place. |
| 37 | 3 | ✓ (as a recursive CTE) With no START WITH, every row is a root: 5 + 1 + 3 + 1 + 1. |
| 38 | 1 | **O** Park is the root (LEVEL 1), so Kim is LEVEL 2 with the path /Park/Kim. |
| 39 | 2 | **O** `PRIOR mgr = empno` walks up the tree, from staff to boss. |
| 40 | 1 | ✓ (SUBSTR) -4 starts at the 4th character from the end, 'B'. LTRIM strips only from the left. 1234.5 rounds to the hundreds. |
| 41 | 4 | A date with no time is midnight, so 01-31 14:00 is after the upper bound. |
| 42 | 1 | The escaped `_` is a literal underscore. `%` after it matches the rest. |
| 43 | 1 | **R** The 2nd run of digits is '22'. An unescaped `.` matches any character. |
| 44 | 2 | **R** `.+` is greedy and `.+?` is lazy. `\n` refers back to group n. |
| 45 | 2 | ✓ The column left over (mgr) becomes an implicit GROUP BY: 3 distinct mgr values. |
| 46 | 2 | ✓ UNPIVOT drops NULL values unless you write INCLUDE NULLS. |
| 47 | 4 | **O** ROLLBACK TO s1 removes C. The DDL commits A, B, D. The final ROLLBACK removes only E. |
| 48 | 2 | **O** DELETE WHERE checks the updated value, so row 2 becomes 'x' and is then deleted. (✓ for the MERGE without DELETE.) |
| 49 | 4 | **O** Object privileges granted WITH GRANT OPTION are revoked in cascade. |
| 50 | 4 | **O** CHECK rejects only FALSE. NULL > 0 is unknown, so the row is accepted. (Option 1 is true; note that the NOT NULL Oracle adds implicitly for a PK column is not copied.) |
