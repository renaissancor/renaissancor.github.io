# Road Map

From beginning to end of the project, timeline and log 


## Navigation
- [Home](Home.md)
- [Project Roadmap](RoadMap.md)
- [System Design](Design.md)
- [Performance Improvements](Performance.md)
- [Multi-threading Challenges](MultiThreading.md)
- [Developer Guide](DeveloperGuide.md)

### Milestone 1 : Single threaded version of the database program
**Input from the standard input, all required instructions working;**

**Single Query  Read Functions Checklist**

- [x] `SELECT` Select and display records from a table based on conditions
- [x] `SUM` Aggregate the sum of fields in records
- [x] `COUNT` Count the number of records based on conditions
- [x] `MIN` Find the minimum values in specified fields based on conditions
- [x] `MAX` Find the maximum values in specified fields based on conditions
- [x] `QUIT` Exit the database program

**Single Query Write Functions Checklist**

- [x] `ADD` Add specified fields and store the result in a destination field
- [x] `SUB` Subtract fields and store the result in a destination field
- [x] `DUMP` Dump an existing table to a file
- [x] `DROP` Delete an existing table to nothing
- [x] `DELETE` Delete records from a table based on conditions
- [x] `INSERT` Insert a new record into a table
- [x] `UPDATE` Update records in a table based on conditions
- [x] `SWAP` Swap values between fields based on conditions
- [x] `DUPLICATE` Duplicate records based on conditions
- [x] `LOAD` Load a new table from a file
- [ ] `COPYTABLE` Copy a table to create new table with same contents and different name
- [ ] `TRUNCATE` Clear all data from a table

Accidentally, `COPYTABLE` and `TRUNCATE` were not implemented in beginning,
so these two functions are discovered and implemented during milestone 2 testing.
During testing, there were some segmentation error issues required for debugging.

### Milestone 2 : Multi-threaded version of the database program,
**Input from the standard input, three-quarter of the instructions implemented;**

For the following, efficient algorithm design for handling multiple parallel query execution
was key concept. There was need to decide Data structure (Bitmap, Balanced Tree, Hash Table, etc...)

**1. Multi-Threading Implementation**:
- [x] Introduced C++ threading libraries including `<thread>`, `<mutex>`, `<condition_variable>`.
- [x] Ensured thread synchronization using mutexes and condition variables.
- [x] Implemented a basic thread pool to manage query execution.

**2. Optimization of Read-Only Queries**:
- [x] Ensured thread-safe execution for queries like `SELECT`, `SUM`, `COUNT`, `MIN`, `MAX`.
- [x] Benchmarked performance improvements over the single-threaded implementation.

**3. Partial Write Functionality**:
- [x] Implemented three-quarters of write-based instructions including `INSERT`, `DELETE`, and `UPDATE`.
- [x] Implemented missing functions `COPYTABLE` and `TRUNCATE`

**4. Testing and Debugging**:
- [x] Conducted multi threaded stress tests to identify race conditions and deadlocks.
- [x] Debugged segmentation fault issues encountered during concurrent execution.


### Milestone 3 : Final submission, fully working database program
**Supporting multi-threading, all required instruction, and command line arguments;**

**1. Finalize Write-Only Queries**:
- [x] Completed implementation of all write-based queries, including `ADD`, `SUB`, `DUPLICATE`, and `SWAP`.

**2. Add Command-Line Arguments**:
- [x] Implemented `--threads=<int>`: Allows users to specify the number of threads or auto-detect available CPU cores when set to 0.
- [x] Implemented `--listen <filename>`: Enables file-based query input.

**3. Performance Optimization**:
- [x] Profiled the system for bottlenecks and improved thread scheduling.
- [x] Conducted final performance benchmarks to ensure superior efficiency compared to the single-threaded version.

**4. Comprehensive Testing**:
- [x] Validated correctness using the provided test suite and custom test scripts.
- [x] Ran stress tests on the school server to ensure stability under heavy workloads.

**5. Documentation**:
- [x] Provided detailed explanations of:
  - **System Design**: Architectural overview of LemonDB.
  - **Threading Model**: Thread pool design, synchronization strategies, and concurrency handling.
  - **Optimizations**: Performance improvements and challenges resolved.
  - **Testing Results**: Summary of test outcomes and benchmarks.

Final programming and documentation continued until the deadline date.


## Changelogs & Meeting records

#### Meeting 1: 2024.10.23
**Discussion**:
Our team met each other and briefly discussed the whole project schema.
Our short term goal was following, to finish basic setup as soon as possible.
- Fix source code compilation
- Write comment for part of source code
- Start writing milestone 1
- Initiate the Agile Development steps on gitea

#### Remarkable Change Logs
- **AddQuery/SubQuery**: Added implementation for handling addition/subtraction operations.
- **CountQuery**: Added implementation for counting entries.
- **DeleteQuery**: Added implementation for deleting records.
- **DuplicateQuery**: Added implementation for duplicating records.
- **InsertQuery**: Added implementation for inserting new rows.
- **MaxQuery/MinQuery**: Added implementation or finding maxima/minima.
- **SelectQuery**: Added implementation for accessing data in a table.
- **SumQuery**: Added implementation for summing records.
- **SwapQuery**: Added implementation for swapping values
- **UpdateQuery**: Added implementation for updating data in a table

- **Table Management**:
  - **CopyTableQuery**: Added implementation for copy a table.
  - **DropTableQuery**: Added implementation for deleting an existing table.
  - **DumpTableQuery**: Added implementation for dumping exisiting table to file.
  - **TruncateTableQuery**: Added implementation for clear an existing table.

#### Meeting 2: 2024.10.30
**Discussion**:
First step is to remove useless files from original LemonDB source code.
Our team accomplished goals in previous meeting and target to do.
- Write comment for part of source code
- Start writing milestone 1

Now, implement all query execution functions for single thread.

#### Remarkable Change Logs
- **QueryExecuter**, **QueryExecuterHelper**, **QueryThreadPool**: Introduced thread pool management. Changed the logic of query execution from single-thread to multithread, used a thread pool to manage the thread.

#### Meeting 3: 2024.11.10
**Discussion**:
- Milestone 1 single-threaded functions were mostly completed but required passing all test cases to mark true completion.
- Focus shifted to designing an efficient algorithm for handling parallel query execution. Key considerations included:
  - Selection of data structures (Bitmap, Balanced Tree, Hash Table, etc.) for concurrent data access.
  - Synchronizing shared resources to prevent race conditions and ensure thread safety.

#### Remarkable Change Logs
- **System Optimization**:
  - Improved resource handling for better memory and time efficiency during query execution.
  - Improved mutex handling for better thread safety.
  - Refined algorithms for query operations to reduce overhead and improve response times.

#### Meeting 4: 2024.11.17
**Discussion**:
- Successfully implemented thread pool for efficient multi-threaded query execution.
- Identified the need to create custom test scripts for testing the program on the school server.
- Evaluated and improved thread locking mechanisms to resolve performance bottlenecks.

## Navigation
- [Home](Home.md)
- [Project Roadmap](RoadMap.md)
- [System Design](Design.md)
- [Performance Improvements](Performance.md)
- [Multi-threading Challenges](MultiThreading.md)
- [Developer Guide](DeveloperGuide.md)
