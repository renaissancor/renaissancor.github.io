# Namespaces and System Catalogs

## MySQL's "database" is Postgres's "schema", not Postgres's "database"

The SQL standard names three levels: `catalog.schema.object`. Engines collapse them
differently, and the difference is structural rather than cosmetic.

| | catalog | schema |
|---|---|---|
| MySQL | does not exist | "database" — the two words are synonyms |
| PostgreSQL | database | real schemas |
| DuckDB | attached database file | real schemas |

So a MySQL database sits at the level Postgres calls a schema. Postgres adds a level
*above* it that MySQL has no equivalent for.

That extra level is a hard boundary. One Postgres connection sees one database, and
you cannot join across two databases on the same server — you need FDW or dblink. In
MySQL, `SELECT ... FROM db1.t JOIN db2.t` simply works, because there is only one
namespace. The closest MySQL analogue to a second Postgres database is therefore a
second *server*, not a second database.

DuckDB sits in between: it has the catalog level, but `ATTACH` makes cross-catalog
joins work, so it reads like Postgres and behaves like MySQL.

**Open:** does DuckDB's catalog level impose any boundary at all, or is it purely an
alias?

## `pg_catalog` and `information_schema` are shipped, not yours

Every Postgres database starts with both, plus an empty `public` schema. They are easy
to mistake for something you or a migration created.

- `pg_catalog` is the real catalog, and Postgres is self-describing through it: every
  table, column and index is itself a row (`pg_class`, `pg_attribute`, …).
- `information_schema` is SQL-standard *views* over `pg_catalog`. MySQL has it too.
- `public` is the default target for any `CREATE TABLE` without a schema prefix.

Worth knowing because GUI clients cache introspection: schemas created outside the
client do not appear until an explicit refresh, so a tool's tree can disagree with
`\dn` and the tree is the one that is wrong. Verify structure from the catalog, not
from the sidebar.

**Open:** how `search_path` resolves unqualified names, and how it fails across schemas.
