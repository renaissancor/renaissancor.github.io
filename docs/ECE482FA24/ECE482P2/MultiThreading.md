# Multi-threading Challenges

Support concurrent query execution using multi-threading.  


## Navigation
- [Home](Home.md)
- [Project Roadmap](RoadMap.md)
- [System Design](Design.md)
- [Performance Improvements](Performance.md)
- [Multi-threading Challenges](MultiThreading.md)
- [Developer Guide](DeveloperGuide.md)


## Features

### 1. Multi-Threaded Execution:
- Parallel execution of queries using a thread pool.
- Thread-safe resource management with fine-grained locks.

### 2. Query Types:
- Read-Only: SELECT, COUNT, MAX, MIN, SUM
- Write-Only: INSERT, DELETE, UPDATE, LOAD, COPY_TABLE

### 3. Concurrency Control: 
- Shared locks for read operations.
- Exclusive locks for write operations.
- Deadlock prevention through ordered locking.


## Challenges 

### Deadlocks
- **Issue**: Circular dependencies between locks might trigger threads to wait indefinitely  
- **Solution**: Break "hold and wait" by retrying when locks cannot be acquired 

### Race Conditions
- **Issue**: Concurrent access to shared resources causes inconsistent states 
- Used atomic operations and counters 
- Enforce ordered locking to prevent cyclic dependencies

### Thread Safety
- Use `std::mutex` for exclusive locks 
- Use `std::shared_mutex` for shared locks.
- Use `std::condition_variable` for coordinate threads.


## Original Approach: Locking Each Function

During the first 2 weeks, our approach for multi threading system lock was individual lock.
Each query-related function (e.g., `SELECT`, `SWAP`, `MAX`) had its own lock 
to protect the critical sections within that function.
Locking within a function was the easiest method to implement. 
Also, as each function handled its own locking, 
detailed control over resource protection was possible.
It also allowed minimal interference between unrelated operations. 

However, this method was not good approach to handle speed optimization. 
There were large number of independent locks, making the system harder to manage and debug. 
Moreover, higher risk of deadlocks when functions accessed shared resources followed. 
Since original design's goal was simplicity and isolation, so that
no function could accidentally handle shared resources without proper protection. 
Thus, the operation speed was unacceptable, and new design was required. 


## Current Approach: Locks Per Table 

In the current design, locks are applied per table instead of per function. 
Based on the query type: Each table has a shared or exclusive lock. 

- Shared Locks (`std::shared_mutex`) allow multiple threads to perform read operations concurrently.
- Exclusive Locks  (`std::mutex`) block all other operations while a write operation is executed.

Compared to the original design without enough consideration, table lock has several advantages. 

- Read queries (`SELECT`, `COUNT`, etc.) can proceed in parallel without blocking each other.
- Write queries (`INSERT`, `DELETE`, etc.) need exclusive locks, ensuring data consistency.
- Ordered lock ensures threads lock in same order, preventing circular dependencies, meaning deadlock.
- Locks are managed at the table level rather than function level, reducing total lock/unlock operations. 
- Allows unrelated queries on different tables to proceed simultaneously. 


## Comparison between two methods 

| **Metric**            | **Lock Per Function**                              | **Per-Table Lock**                            |
|-----------------------|----------------------------------------------------|-----------------------------------------------|
| **Concurrency**       | Low (locks on functions block threads)             | High (multiple threads can read concurrently) |
| **Deadlock Risk**     | High (independent locks cause cyclic dependencies) | Low (ordered locking and fewer locks)         |
| **Overhead**          | High (many lock/unlock calls)                      | Lower (fewer, coarser locks)                  |
| **Scalability**       | Poor (limited by function locks)                   | Better (independent locks per table)          |
| **Complexity**        | Simple (isolated locking)                          | Moderate (lock coordination required)         |

It was able to accomplish following by implementation of lock per table 

- **Resource Optimization**: Concurrent read queries on the same table
- **Higher Throughput**: Write queries block only specific table in work, avoid bottlenecks
- **Reduced Lock Contention**: Per-table locks is reduced compared to per-function locks 
- **Simplified Debugging**: Lock-per-table approach centralizes lock to table-specific resources
- **Future Scalability**: Supports scaling to larger systems with more tables and threads


## Navigation
- [Home](Home.md)
- [Project Roadmap](RoadMap.md)
- [System Design](Design.md)
- [Performance Improvements](Performance.md)
- [Multi-threading Challenges](MultiThreading.md)
- [Developer Guide](DeveloperGuide.md)
