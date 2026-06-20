---
title: "Windows Virtual Memory, Heap, and Memory-Mapped Files"
---

# Windows Virtual Memory, Heap, and Memory-Mapped Files

Windows exposes three distinct layers of memory management to application programmers: raw virtual-address space (controlled through `VirtualAlloc`), heap allocators that sit on top of pages (`HeapCreate`/`HeapAlloc`), and memory-mapped files that project file data directly into an address range. Understanding all three — and why each exists — is essential for writing correct, efficient Win32 code.

## Virtual Memory and the Page Model

Every process on a 32-bit Windows system sees a flat, 4 GB address space regardless of how much physical RAM is installed. On 64-bit Windows the space is vastly larger, but the mechanics are the same. The CPU and the OS collaborate through a **page table** to translate virtual addresses into physical RAM addresses, one page at a time.

The fundamental unit is the **page** — 4 KB (2¹²) on x86/x86-64. A 32-bit address space therefore contains up to one million distinct pages. Each page can be in exactly one of three states:

| State | Meaning |
|-------|---------|
| **Free** | Not associated with any physical frame; access causes an access violation. Shown as `?? ??` in the VS debugger memory view. |
| **Reserved** | Address range is claimed (no other allocation can overlap it), but no physical frame is mapped. No RAM is consumed. |
| **Committed** | Backed by a physical frame (or page-file slot). Reads and writes are valid. |

The `Reserve` state is the key insight. Consider a process whose working set is 1 MB 99 % of the time, but which might briefly need up to 4 GB. Jumping straight from Free to Committed would force the OS to hold physical frames for the entire possible range. Reserving the address space costs nothing in RAM while guaranteeing that a contiguous virtual range is available when the process eventually commits pages inside it — critical for data structures that require sequential addresses (arrays, ring buffers).

You can query page and granularity sizes at runtime:

```cpp
SYSTEM_INFO si;
GetSystemInfo(&si);
DWORD pageSize          = si.dwPageSize;              // 4 096 bytes
DWORD allocGranularity  = si.dwAllocationGranularity; // 65 536 bytes (64 KB)
```

Allocations begin on `dwAllocationGranularity` (64 KB) boundaries even though individual pages are 4 KB. Commit can later be done in 4 KB increments inside a reserved region.

## VirtualAlloc and VirtualFree

`malloc`/`free` allocate on the default heap and always produce committed memory — you cannot use them to reserve address space without consuming RAM. For fine-grained control you call the page-level API directly:

```cpp
LPVOID VirtualAlloc(
    LPVOID lpAddress,       // preferred base address (NULL = let OS choose)
    SIZE_T dwSize,          // size in bytes (rounded up to page boundary)
    DWORD  flAllocationType,// MEM_RESERVE | MEM_COMMIT (or just one of them)
    DWORD  flProtect        // PAGE_READWRITE, PAGE_NOACCESS, etc.
);

BOOL VirtualFree(
    LPVOID lpAddress,
    SIZE_T dwSize,
    DWORD  dwFreeType       // MEM_DECOMMIT or MEM_RELEASE
);
```

Typical two-phase pattern — reserve a large region up front, commit only what is needed now:

```cpp
// Reserve 16 MB of virtual address space without touching RAM
LPVOID pBase = VirtualAlloc(NULL, 16 * 1024 * 1024,
                            MEM_RESERVE, PAGE_NOACCESS);

// Later: commit the first 4 KB when the data actually arrives
VirtualAlloc(pBase, 4096, MEM_COMMIT, PAGE_READWRITE);

// Release the physical frame (page goes back to Reserved)
VirtualFree(pBase, 4096, MEM_DECOMMIT);

// Release the entire reserved region (back to Free)
VirtualFree(pBase, 0, MEM_RELEASE);
```

`MEM_DECOMMIT` transitions Committed → Reserved (keeps the address reservation, frees the physical frame). `MEM_RELEASE` transitions any state → Free (the address range is fully relinquished and must be passed with `dwSize = 0`).

## Heap Management

Calling `VirtualAlloc` for every small allocation is impractical — it wastes memory (minimum allocation is one 4 KB page) and causes fragmentation. The **heap** is an allocator layer that acquires pages from the OS in bulk and then sub-divides them into arbitrary-sized blocks.

Every process starts with a **default heap**. `malloc`, `new`, and `LocalAlloc` all draw from it. Windows also lets you create **custom heaps** with `HeapCreate`:

```cpp
HANDLE HeapCreate(
    DWORD  flOptions,       // 0, HEAP_NO_SERIALIZE, or HEAP_GENERATE_EXCEPTIONS
    SIZE_T dwInitialSize,   // bytes to commit immediately
    SIZE_T dwMaximumSize    // 0 = growable; non-zero = fixed ceiling
);
```

Allocate and free within a custom heap:

```cpp
HANDLE hHeap = HeapCreate(0, 0, 0); // growable heap

LPVOID pNode = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(MyNode));
// ... use pNode ...
HeapFree(hHeap, 0, pNode);

// Destroy the entire heap in one shot — no per-node cleanup needed
HeapDestroy(hHeap);
```

The practical payoff of a custom heap:

- **Simplified cleanup.** A linked list, graph, or arena of objects allocated from a dedicated heap can be torn down with a single `HeapDestroy` call instead of walking every pointer.
- **Reduced fragmentation.** Mixing short-lived and long-lived objects on the default heap causes fragmentation; separate heaps keep their lifecycles isolated.
- **Thread isolation.** A heap created per thread (with `HEAP_NO_SERIALIZE`) eliminates lock contention on the allocator.

Under the hood, `HeapAlloc` itself calls `VirtualAlloc` to commit new pages when the heap's current reservation runs out. The heap is thus the bridge between the raw page model and the variable-sized allocations that application code actually needs.

## Memory-Mapped Files

A **memory-mapped file (MMF)** projects a file — or a region of it — directly into the process's virtual address space. Once the mapping is established, reading from and writing to a pointer is equivalent to reading from and writing to the file; the OS handles the I/O in the background.

```
Process virtual address space
┌──────────────────────────────┐
│  ...                         │
│  mapped view  ←─────────────────────┐
│  ...                         │     │  page cache / file
└──────────────────────────────┘     │
                                      └── data.bin on disk
```

Three-step setup:

```cpp
// 1. Open or create the file
HANDLE hFile = CreateFile(
    L"data.bin",
    GENERIC_READ | GENERIC_WRITE,
    0, NULL, OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL, NULL);

// 2. Create a file-mapping object (kernel object describing the mapping)
HANDLE hMap = CreateFileMapping(
    hFile,
    NULL,              // default security
    PAGE_READWRITE,
    0, 0,              // map the whole file
    L"MySharedMap");   // optional name for cross-process sharing

// 3. Map a view into this process's address space
BYTE* pView = (BYTE*)MapViewOfFile(
    hMap,
    FILE_MAP_ALL_ACCESS,
    0, 0,   // offset (high, low)
    0);     // 0 = map entire mapping

// Read / write as if it were an array
pView[0] = 'H';
pView[1] = 'i';

// Flush pending writes to disk explicitly if needed
FlushViewOfFile(pView, 0);

// Teardown
UnmapViewOfFile(pView);
CloseHandle(hMap);
CloseHandle(hFile);
```

### Why use MMF?

**Large-file access without buffering overhead.** Reading a multi-gigabyte file with `ReadFile` in a loop copies data through a kernel buffer into a user buffer. MMF lets you address the file contents directly — the OS pages in only the portions you touch, using the same demand-paging machinery as virtual memory.

**Inter-process shared memory.** If two processes open a file-mapping object with the same name (passing `INVALID_HANDLE_VALUE` as the file handle creates a purely memory-backed mapping), they share the same physical pages. This is the fastest IPC mechanism available on Windows for bulk data.

```cpp
// Process A — create
HANDLE hMap = CreateFileMapping(
    INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
    0, 4096, L"Local\\SharedBuffer");
char* p = (char*)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
strcpy_s(p, 4096, "Hello from A");

// Process B — open by name
HANDLE hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"Local\\SharedBuffer");
char* p = (char*)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
// p now contains "Hello from A"
```

## The Three Layers Together

```
Application code
      │
      │  malloc / new / HeapAlloc
      ▼
   Heap layer          ← sub-divides pages, hides 4 KB granularity
      │
      │  VirtualAlloc (MEM_COMMIT)
      ▼
 Virtual address space  ← Reserve / Commit / Free page states
      │
      │  page-fault handler (demand paging)
      ▼
  Physical RAM / Page file
      │
      │  CreateFileMapping / MapViewOfFile
      ▼
  File on disk          ← MMF bypasses the explicit I/O layer
```

`VirtualAlloc` gives precise, page-granular control over the address space. The heap sits on top of it to make small allocations practical. MMF side-steps the explicit read/write loop and integrates file data into the virtual memory model directly.

---

*Study notes based on 윤성우, 《윈도우즈 시스템 프로그래밍》 (book + lectures).*
