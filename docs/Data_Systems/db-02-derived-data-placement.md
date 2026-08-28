# Where Derived Data Belongs

## Regenerable output is a build artifact, so store files and let the database be a catalog

When a pipeline produces a large body of output, the tempting question is whether the
database can hold it. Usually it can, and that is the wrong question. The deciding
property is not size but whether the data can be **regenerated**: output of a
deterministic transform over a frozen input is a build artifact, and a transactional
database exists for things that cannot be rebuilt.

Costs follow from that framing. Loading regenerable data writes WAL proportional to its
size on every run, and repeated runs mean bulk delete plus insert, so you pay the
durability machinery to protect something a re-run would recreate. Large values are also
pushed out of line into TOAST and compressed, which makes the storage cheap but does not
make the write amplification go away.

There is a workflow cost too. While a transform is still being validated, the inspection
loop is `grep`, `diff` between two runs, and reading one record by eye. Moving output
behind SQL makes the primary verification workflow worse at exactly the stage it matters
most.

What the database should hold is the catalog:

```
id · batch_id · path · sha256 · byte_size
transform_version · input_snapshot_id · produced_at
validation_status
```

Small, indexable, and sufficient for the questions actually asked — what changed between
runs, what failed validation, what is missing. The content hash is the important column:
it turns "diff two large runs" into a single query rather than a filesystem walk.

The exception is data that stops being derived. Anything a human has reviewed and signed
off on is authored, not regenerable, and belongs in the database with the transactional
guarantees that implies.

**Open:** the measured TOAST compression ratio for large text, and which properties
besides regenerability (mutability? access granularity?) should decide blob-in-DB versus
path-in-DB.
