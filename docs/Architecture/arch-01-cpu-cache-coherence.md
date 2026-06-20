---
title: "CPU Cache, Coherence, and Performance Timing"
---

# CPU Cache, Coherence, and Performance Timing

Modern CPU performance is dominated not by raw clock speed but by how efficiently data flows between memory and compute. Cache hierarchy, cache-friendly access patterns, coherence protocols, and measurement methodology are the four pillars that determine whether code runs fast or stalls.

---

## Cache Hierarchy and Cache Lines

CPUs ship with three levels of cache, each trading size for speed. A representative modern desktop configuration looks like:

| Level | Size per core | Shared? |
|-------|--------------|---------|
| L1 (inst) | 32 KB | No — per core |
| L1 (data) | 32 KB | No — per core |
| L2 | 256 KB – 1 MB | No — per core |
| L3 | 6 MB+ | Intel: shared; AMD: per-core linked by mesh |

The fundamental unit of transfer is the **cache line**: 64 bytes. Every cache read or write operates on a 64-byte block, not individual bytes. This is the hardware mechanism behind **spatial locality** — accessing one element of an array pulls the surrounding 64 bytes into cache for free, so sequential iteration is fast.

**Temporal locality** is the complementary principle: data accessed recently is likely to be accessed again. The cache hierarchy exploits both.

### Address Decomposition: Tag / Index / Offset

The CPU locates a cache line by splitting a memory address into three fields. For a 32-bit address with a 32 KB, 8-way L1 data cache:

```
[ 32-bit Address ]
|  Tag (20b) | Index (6b) | Offset (6b) |
|____________|___________|_____________|
                  |              |
            Which set?    Byte within line
```

- **Offset (6 bits)**: selects one of the 64 bytes inside the cache line (`2^6 = 64`).
- **Index (6 bits)**: selects one of the 64 cache sets (`2^6 = 64`).
- **Tag (upper bits)**: validates that the data in that set actually belongs to this address.

The lower bits are used for index and offset because nearby stack and heap allocations share upper bits — this maximizes the likelihood of a cache hit for accesses within the same ~64 KB working set.

### Cache Mapping Strategies

**Direct mapped cache** assigns each memory block to exactly one cache set. Simple and fast to look up, but a single collision evicts the resident line — catastrophic for programs that touch heap, `.data`, `.rodata`, and stack simultaneously.

**Fully associative cache** allows any block into any line. Collision-free but impractical: checking all lines for a hit requires complex circuitry that does not scale.

**N-way set associative cache** is the practical middle ground. Each set holds N cache lines. With 8-way associativity, the 64 sets each hold 8 lines — a memory address can occupy any of 8 slots within its set. Eviction uses **LRU (Least Recently Used)**, replacing the line with the oldest `__rdtsc()` timestamp.

A 32 KB 8-way cache has `32768 / (64 × 64) = 8` ways, or equivalently `32768 / 64 = 512` total lines across 64 sets.

---

## VIPT and the Page Offset Constraint

L1 cache on modern CPUs is **Virtually-Indexed, Physically-Tagged (VIPT)**:

- The virtual address supplies index and offset bits — no TLB lookup required to find the set.
- The physical address tag (from TLB/MMU) validates the hit.

This works correctly only when index + offset fits within the **page offset** (12 bits for 4 KB pages), because virtual and physical addresses share the same low 12 bits. If the index field spilled past bit 11, two different physical frames could map to the same virtual index, causing aliasing.

This is why L1 data caches are stuck at **64 sets maximum** at 4 KB pages: `6 bits (offset) + 6 bits (index) = 12 bits = page offset`. Growing to 128 sets would require 7 index bits, pushing the total to 13 bits — past the page boundary, requiring full MMU involvement or risking aliasing bugs. The constraint lifts only if the OS moves to 8 KB pages (bumping the page offset to 13 bits), which would allow 128 sets without aliasing.

---

## Write Policies

| Policy | Behavior | Used for |
|--------|----------|----------|
| **Write-through** | Writes to cache and RAM simultaneously | L1 instruction cache |
| **Write-back** | Writes to cache only; flushes to RAM on eviction | L1/L2 data cache |

Write-back is faster for write-heavy workloads but requires coherence management when multiple cores cache the same line.

---

## MESI Cache Coherence Protocol

With L1 and L2 per-core, the hardware must guarantee that two cores never see inconsistent views of the same memory. MESI assigns one of four states to every cache line (granularity: 64 bytes):

| State | Meaning |
|-------|---------|
| **M — Modified** | Line is dirty; only this core has it; RAM is stale |
| **E — Exclusive** | Line is clean; only this core has it; matches RAM |
| **S — Shared** | Line is clean; multiple cores hold copies; matches RAM |
| **I — Invalid** | Line is not usable |

### Coherence Transition on a Write

When Core A writes to a line currently in **Shared** state across multiple cores:

1. Core A's request is broadcast on the cache bus (snooping).
2. Core A's line transitions from **S → E** (it is now the sole owner).
3. All other cores holding that line transition their copy **S → I** (invalidated).
4. Core A writes the data; the line is now **E → M** (modified, dirty).
5. When another core later requests the line, Core A must **write back** the dirty data to RAM before the request can be served — this write-back latency is the primary coherence cost.

The key performance implication: **false sharing** — two cores writing different fields of the same 64-byte cache line — triggers this entire invalidation cycle repeatedly, even though the cores are logically operating on independent data.

---

## Cache Miss Root Causes and Mitigations

The cache simulator built around the MESI/LRU model identifies two dominant miss sources:

**Same-index collision (capacity/conflict miss)**: Any two allocations whose addresses differ by a multiple of `sets × line_size` (e.g., 4 KB with 64 sets × 64 B) land on the same cache set and evict each other. Solution: align or pad hot structures with `alignas` to break the stride.

**Spatial locality violations**: Accessing fields of scattered objects (OOP pointer-heavy graphs) brings in 64-byte lines where only a few bytes are used. **Data-Oriented Design (DOD)** — grouping homogeneous fields into contiguous arrays (struct-of-arrays rather than array-of-structs) — maximizes the useful bytes per cache line fetch.

### Row-major vs. Column-major Traversal

For a 2D array stored in row-major order (C/C++ default), iterating by row is cache-friendly: consecutive reads advance through a single cache line. Column traversal jumps by `n × sizeof(element)` per step — for large `n`, each access lands on a different (and likely uncached) line.

---

## Cache Simulator: Measured Hit Rates

A software cache simulator modeled an 8-way LRU set-associative cache (64 sets, 64-byte lines) and measured hit rates for matrix operations at increasing sizes. Two access patterns were compared per size — the first column approximates row-major (cache-friendly), the second column approximates column-major (cache-hostile):

```
   n =      16, n^2  =     256
Row-major:  Hits =     244, Miss =  12, Ratio = 95.31%
Col-major:  Hits =     256, Miss =   0, Ratio = 100.00%    ← fits entirely in cache

   n =     192, n^2  =   36864
Row-major:  Hits =  52850, Miss =   398, Ratio = 99.25%
Col-major:  Hits =  36533, Miss =   331, Ratio = 99.10%    ← still mostly resident

   n =     448, n^2  =  200704
Row-major:  Hits = 319356, Miss =  28804, Ratio = 91.73%
Col-major:  Hits =  35647, Miss = 165057, Ratio = 17.76%   ← cliff begins

   n =     896, n^2  =  802816
Row-major:  Hits = 804318, Miss = 588322, Ratio = 57.75%
Col-major:  Hits =       0, Miss = 802816, Ratio =  0.00%  ← total miss

   n =    1024, n^2  = 1048576
Row-major:  Hits = 1032967, Miss = 937209, Ratio = 52.43%
Col-major:  Hits =    6024, Miss = 1042552, Ratio =  0.57%
```

Key observations:

- For small matrices (n ≤ ~192), both patterns fit in cache and hit rates are near-identical and high.
- The cache-hostile pattern falls off a **cliff around n = 448**: hit rate drops from ~92% to 17% in one step, reaching **0% at n = 896** — every single access is a miss.
- The cache-friendly pattern degrades more gradually, holding above 50% even at n = 1024.
- For large n (2048, 4096, 8192), the row-major pattern recovers to ~78% because sequential access within a row still exploits spatial locality within the working set, while the column-major pattern stays below 1%.

The simulator used `__rdtsc()` (the CPU's timestamp counter) for LRU timestamps and extracted the cache set index as `(addr >> 6) & 0x3f`.

---

## Performance Timing Methodology

Measuring cache effects requires sub-millisecond timing. The options, ordered by resolution:

| Function | Resolution | Notes |
|----------|-----------|-------|
| `GetTickCount()` | ~10–16 ms | Granularity far coarser than the unit suggests |
| `clock()` / `CLOCKS_PER_SEC` | ~1 ms | Process clock; C standard |
| `QueryPerformanceCounter()` | ~100 ns | High-resolution Windows API |
| `__rdtsc()` | 1 CPU cycle (~0.3 ns @ 3 GHz) | Direct timestamp counter; no syscall overhead |
| `std::chrono::high_resolution_clock` | Platform-dependent | Wraps OS primitives |

### Noise and Multi-tasking

The OS runs ~200 processes and ~2000 threads concurrently. Any timing measurement is subject to:

- **Context switches** flushing TLB entries and polluting L1/L2 cache.
- **OS interrupt handlers** adding unpredictable latency spikes.
- **Timer resolution** mismatches — `GetTickCount()` returning millisecond-looking values that actually update every 10–16 ms.

For functions faster than ~10 ms, a single measurement is not meaningful. Two reliable strategies:

**Method 1 — Averaging over millions of runs**: Execute the target operation 1,000,000+ times, measure total elapsed time with `QueryPerformanceCounter` or `__rdtsc()`, divide. Amortizes context-switch noise.

**Method 2 — Batch repetition until measurable**: If a single run returns 0 or 1 ticks, wrap it in a loop of sufficient count (e.g., 10,000 iterations) so the total duration exceeds timer resolution. Report `total / count`.

The `__rdtsc()` approach used in the cache simulator is ideal for cache benchmarking specifically because it reads the hardware cycle counter with no kernel call, making the measurement itself invisible to the OS scheduler.

---

## Summary

| Concept | Key number |
|---------|-----------|
| Cache line size | 64 bytes |
| Typical L1 data sets | 64 (constrained by 4 KB page offset) |
| Typical L1 ways (8-way) | 8 |
| MESI states | M / E / S / I — per cache line |
| Cache-hostile cliff (simulator) | n ≈ 448 (hit rate: 91% → 17%) |
| Total-miss threshold | n = 896 (hit rate: 0%) |
| Best timing resolution | `__rdtsc()` (~1 cycle) |

Cache behavior is not a low-level curiosity — it is the single largest determinant of memory-bound workload performance. Understanding address decomposition, set-associative eviction, MESI coherence transitions, and timing methodology gives the tools to measure and reason about it empirically rather than by intuition.
