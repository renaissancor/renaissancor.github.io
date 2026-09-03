---
title: "TLB, Page Faults, and Copy-on-Write"
---

# TLB, Page Faults, and Copy-on-Write

Two corrections from a self-test on virtual memory. Both are places where a plausible mental model is wrong in a way that matters for performance work.

---

## A TLB miss is not a page fault

Three separate structures nest, and each has its own kind of miss.

- **Page table** in RAM: the full virtual-to-physical map, one entry per 4 KB page, built by the OS.
- **TLB**: a cache of a few hundred to ~1,500 recent page table entries inside the MMU. A subset, not "the process's page tables".
- **Data cache** (L1/L2/L3): the bytes themselves. Unrelated to translation.

A load of virtual address X:

1. Check the TLB. Hit: physical address known. **Miss**: on x86 a hardware page walker reads the page table from RAM, fills the TLB, continues. No trap, no OS involvement, a few memory accesses.
2. Entry says *present*: fetch the data, which can itself hit or miss L1/L2/L3.
3. Entry says *not present*: **page fault**. CPU raises exception 14 (`#PF`), the OS loads from disk, allocates a zero page, or kills the process with a segfault.

| Miss | Fixed by | Cost |
|---|---|---|
| TLB miss | Hardware page walk | nanoseconds |
| Data cache miss | Hardware fetch from RAM | tens of ns |
| Page fault | OS handler | µs to ms |

"Miss" sounds like failure. It means "not cached". The TLB never decides validity; the page table entry does. The wrong model, where a TLB miss traps to the OS, describes software-managed TLBs (MIPS, older SPARC), not x86 or ARM.

**Open:** how much do 2 MB huge pages cut `dTLB-load-misses` for a process with tens of GB of resident weights?

---

## fork copies nothing, but in CPython reading is writing

After `fork`, the child does not share memory logically. It has its own address space. The OS points both page tables at the same physical frames and marks every page read-only. Extra RAM right after fork is the new page tables, a few MB for an 8 GB parent.

The first **write** to a page from either side faults on the read-only bit. The OS copies that one 4 KB page and gives the writer its own. This is copy-on-write.

The Python-specific trap: a worker that only *reads* still increments a refcount in every object header it touches. That is a write. A read-only worker therefore copies every page holding an object it looks at, and slowly duplicates most of the parent. This is why `multiprocessing` with a large loaded model bloats, and why the fixes are fork *before* loading, shared memory, or `gc.freeze()`.

**Open:** measure it. Fork an 8 GB Python process, walk a list in the child, watch RSS.

---

## Two answers that were right, with one refinement each

**mmap of a 10 GB file barely moves RSS.** Demand paging: the mapping creates page table entries marked not present. First touch of each page faults, the OS reads that 4 KB, RSS grows one page.

**write() returns, power dies one second later.** Data is in the page cache, not on disk, unless `fsync` (`FlushFileBuffers` on Windows) was called. Refinement: the kernel flushes dirty pages on a timer, roughly every 30 s by default, not only on eviction. The window is seconds, not indefinite. Databases fsync on every commit for exactly this reason.
