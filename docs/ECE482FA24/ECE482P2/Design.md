# System Design

Software schema overview and description 


## Navigation
- [Home](Home.md)
- [Project Roadmap](RoadMap.md)
- [System Design](Design.md)
- [Performance Improvements](Performance.md)
- [Multi-threading Challenges](MultiThreading.md)
- [Developer Guide](DeveloperGuide.md)

## System Overview

In our system, we employ a shared locking strategy for managing concurrent access to resources.
Read locks, often implemented in shared locks, allow multiple threads to access the same resource
concurrently for reading purposes.
Write locks, on the other hand, do not permit concurrent access but can operate concurrently
on different segments of data.
Thus, as long as the operations do not modify the same segment, write locks can also effectively
function as shared locks.
The only one exception to this shared lock usage is during a dump operation,
which requires exclusive access.

To do a "multi-shared-mutex", we've developed a class called "AdvancedLock".
This class combines two standard shared locks,
allowing the efficient management of lock states during complex operations.

An issue in multi-threading is the race condition,
which arises when multiple threads attempt to modify shared data simultaneously.
We address this challenge by the "Advanced Mutex".
Thus threads either gain full required access to the resource or are put on hold,
thereby preventing race conditions and ensuring data integrity and consistency.

## Architecture

`./src` Contains the source code for an old version of lemnonDB.
The code was recovered from crash site. As far as we know the
original developer used CMake as their building system.

- `./bin` Contains the lastest binary that survived the crash.
- `./db` Contains sample database files.
- `./sample` Sample inputs and outputs
- `./build`: Build LemonDB by compile and run code inside folder  
- `./src/query`: Query-related logic 
- `./src/query/execute`: Execution layer, managing threading and parallelism.
- `./src/query/management`: Queries for managing the system state (e.g., table operations).
- `./src/utils`: Utility functions and error handling.

## Main Components 

###   Parse Query 

**Purpose**: Parse the user input, validate syntax, and create query objects. 

1. Tokenize input.
2. Validate syntax.
3. Use QueryBuilder chain to create and return a query object

```text
Function ParseQuery(input)
    If no QueryBuilder exists:
        Throw Error
    Tokenize input
    If tokens are empty:
        Throw Error
    Clear QueryBuilder state
    Return QueryBuilder.extractQuery(tokens)
```

### Execute Query

- Lock necessary resources.
- Execute query based on its type:
  - Read-only: Process safely in parallel.
  - Write-only: Process with required locks.
- Unlock resources and return results. 

```text
Function ExecuteQuery(queryObject)
    Lock resources
    If query is read-only:
        Do read operation
    Else if query is write-only:
        Do write operation
    Unlock resources
    Return results
```

### Thread Life Cycle 

1. Task assignment.
2. Thread execution.
3. Thread termination or reuse.

```text
Function ThreadLifeCycle()
    While thread is active:
        Wait for task
        Execute task
        Mark thread as available
```

### Data Flow 

1. Parse SQL Queries. 
2. Assign Unique IDs.
3. Distribute Queries to Threads.

```text
Function DataFlow()
    Parse queries
    Assign unique IDs
    Distribute to threads
    Collect and return results
```

Tree Structure for system 

```text
.
├── CHANGELOG.md
├── Develop.md
├── INSTALL.md
├── README.md
├── build
│   ├── CMakeCache.txt
│   ├── CMakeFiles
│   │   ├── 3.30.5
│   │   │   ├── CMakeCCompiler.cmake
│   │   │   ├── CMakeCXXCompiler.cmake
│   │   │   ├── CMakeDetermineCompilerABI_C.bin
│   │   │   ├── CMakeDetermineCompilerABI_CXX.bin
│   │   │   ├── CMakeSystem.cmake
│   │   │   ├── CompilerIdC
│   │   │   │   ├── CMakeCCompilerId.c
│   │   │   │   ├── a.out
│   │   │   │   └── tmp
│   │   │   └── CompilerIdCXX
│   │   │       ├── CMakeCXXCompilerId.cpp
│   │   │       ├── a.out
│   │   │       └── tmp
│   │   ├── CMakeConfigureLog.yaml
│   │   ├── CMakeDirectoryInformation.cmake
│   │   ├── Makefile.cmake
│   │   ├── Makefile2
│   │   ├── TargetDirectories.txt
│   │   ├── cmake.check_cache
│   │   ├── lemondb.dir
│   │   │   ├── DependInfo.cmake
│   │   │   ├── build.make
│   │   │   ├── cmake_clean.cmake
│   │   │   ├── compiler_depend.internal
│   │   │   ├── compiler_depend.make
│   │   │   ├── compiler_depend.ts
│   │   │   ├── db
│   │   │   │   ├── Database.cpp.o
│   │   │   │   ├── Database.cpp.o.d
│   │   │   │   ├── Table.cpp.o
│   │   │   │   └── Table.cpp.o.d
│   │   │   ├── depend.make
│   │   │   ├── flags.make
│   │   │   ├── link.txt
│   │   │   ├── main.cpp.o
│   │   │   ├── main.cpp.o.d
│   │   │   ├── progress.make
│   │   │   └── query
│   │   │       ├── Query.cpp.o
│   │   │       ├── Query.cpp.o.d
│   │   │       ├── QueryBuilders.cpp.o
│   │   │       ├── QueryBuilders.cpp.o.d
│   │   │       ├── QueryParser.cpp.o
│   │   │       ├── QueryParser.cpp.o.d
│   │   │       ├── QueryResult.cpp.o
│   │   │       ├── QueryResult.cpp.o.d
│   │   │       ├── data
│   │   │       │   ├── AddQuery.cpp.o
│   │   │       │   ├── AddQuery.cpp.o.d
│   │   │       │   ├── CountQuery.cpp.o
│   │   │       │   ├── CountQuery.cpp.o.d
│   │   │       │   ├── DeleteQuery.cpp.o
│   │   │       │   ├── DeleteQuery.cpp.o.d
│   │   │       │   ├── DuplicateQuery.cpp.o
│   │   │       │   ├── DuplicateQuery.cpp.o.d
│   │   │       │   ├── InsertQuery.cpp.o
│   │   │       │   ├── InsertQuery.cpp.o.d
│   │   │       │   ├── MaxQuery.cpp.o
│   │   │       │   ├── MaxQuery.cpp.o.d
│   │   │       │   ├── MinQuery.cpp.o
│   │   │       │   ├── MinQuery.cpp.o.d
│   │   │       │   ├── SelectQuery.cpp.o
│   │   │       │   ├── SelectQuery.cpp.o.d
│   │   │       │   ├── SubQuery.cpp.o
│   │   │       │   ├── SubQuery.cpp.o.d
│   │   │       │   ├── SumQuery.cpp.o
│   │   │       │   ├── SumQuery.cpp.o.d
│   │   │       │   ├── SwapQuery.cpp.o
│   │   │       │   ├── SwapQuery.cpp.o.d
│   │   │       │   ├── UpdateQuery.cpp.o
│   │   │       │   └── UpdateQuery.cpp.o.d
│   │   │       ├── execute
│   │   │       │   ├── QueryExecuter.cpp.o
│   │   │       │   ├── QueryExecuter.cpp.o.d
│   │   │       │   ├── QueryExecuterHelper.cpp.o
│   │   │       │   ├── QueryExecuterHelper.cpp.o.d
│   │   │       │   ├── QueryThreadPool.cpp.o
│   │   │       │   └── QueryThreadPool.cpp.o.d
│   │   │       └── management
│   │   │           ├── CopyTableQuery.cpp.o
│   │   │           ├── CopyTableQuery.cpp.o.d
│   │   │           ├── DropTableQuery.cpp.o
│   │   │           ├── DropTableQuery.cpp.o.d
│   │   │           ├── DumpTableQuery.cpp.o
│   │   │           ├── DumpTableQuery.cpp.o.d
│   │   │           ├── ListTableQuery.cpp.o
│   │   │           ├── ListTableQuery.cpp.o.d
│   │   │           ├── LoadTableQuery.cpp.o
│   │   │           ├── LoadTableQuery.cpp.o.d
│   │   │           ├── PrintTableQuery.cpp.o
│   │   │           ├── PrintTableQuery.cpp.o.d
│   │   │           ├── QuitQuery.cpp.o
│   │   │           ├── QuitQuery.cpp.o.d
│   │   │           ├── TruncateTableQuery.cpp.o
│   │   │           └── TruncateTableQuery.cpp.o.d
│   │   ├── pkgRedirects
│   │   └── progress.marks
│   ├── Makefile
│   ├── cmake_install.cmake
│   ├── lemondb
│   ├── sample
│   │   ├── ...
│   │   └── test.query
│   ├── sample_stdout
│   │   ├── ...
│   │   └── test.out
│   └── stdout
│       └── test.out
├── db
│   ├── fTable0.tbl
│   ├── ... 
│   ├── mTable49.tbl
│   ├── t1.tbl
│   ├── t2.tbl
│   └── tlarge.tbl
├── src
│   ├── CMakeLists.txt
│   ├── db
│   │   ├── AdvancedLock.h
│   │   ├── Database.cpp
│   │   ├── Database.h
│   │   ├── Table.cpp
│   │   └── Table.h
│   ├── main.cpp
│   ├── query
│   │   ├── Query.cpp
│   │   ├── Query.h
│   │   ├── QueryBuilders.cpp
│   │   ├── QueryBuilders.h
│   │   ├── QueryParser.cpp
│   │   ├── QueryParser.h
│   │   ├── QueryResult.cpp
│   │   ├── QueryResult.h
│   │   ├── data
│   │   │   ├── AddQuery.cpp
│   │   │   ├── AddQuery.h
│   │   │   ├── CountQuery.cpp
│   │   │   ├── CountQuery.h
│   │   │   ├── DeleteQuery.cpp
│   │   │   ├── DeleteQuery.h
│   │   │   ├── DuplicateQuery.cpp
│   │   │   ├── DuplicateQuery.h
│   │   │   ├── InsertQuery.cpp
│   │   │   ├── InsertQuery.h
│   │   │   ├── MaxQuery.cpp
│   │   │   ├── MaxQuery.h
│   │   │   ├── MinQuery.cpp
│   │   │   ├── MinQuery.h
│   │   │   ├── SelectQuery.cpp
│   │   │   ├── SelectQuery.h
│   │   │   ├── SubQuery.cpp
│   │   │   ├── SubQuery.h
│   │   │   ├── SumQuery.cpp
│   │   │   ├── SumQuery.h
│   │   │   ├── SwapQuery.cpp
│   │   │   ├── SwapQuery.h
│   │   │   ├── UpdateQuery.cpp
│   │   │   └── UpdateQuery.h
│   │   ├── execute
│   │   │   ├── QueryExecuter.cpp
│   │   │   ├── QueryExecuter.h
│   │   │   ├── QueryExecuterHelper.cpp
│   │   │   ├── QueryExecuterHelper.h
│   │   │   ├── QueryThreadPool.cpp
│   │   │   └── QueryThreadPool.h
│   │   └── management
│   │       ├── CopyTableQuery.cpp
│   │       ├── CopyTableQuery.h
│   │       ├── DropTableQuery.cpp
│   │       ├── DropTableQuery.h
│   │       ├── DumpTableQuery.cpp
│   │       ├── DumpTableQuery.h
│   │       ├── ListTableQuery.cpp
│   │       ├── ListTableQuery.h
│   │       ├── LoadTableQuery.cpp
│   │       ├── LoadTableQuery.h
│   │       ├── PrintTableQuery.cpp
│   │       ├── PrintTableQuery.h
│   │       ├── QuitQuery.cpp
│   │       ├── QuitQuery.h
│   │       ├── TruncateTableQuery.cpp
│   │       └── TruncateTableQuery.h
│   └── utils
│       ├── formatter.h
│       └── uexception.h
└── test
    ├── run.sh
    ├── run_wr.sh
    ├── testAdd.query
    ├── testMax.query
    ├── testMin.query
    ├── testSum.query
    └── testSwap.query

```

## Navigation
- [Home](Home.md)
- [Project Roadmap](RoadMap.md)
- [System Design](Design.md)
- [Performance Improvements](Performance.md)
- [Multi-threading Challenges](MultiThreading.md)
- [Developer Guide](DeveloperGuide.md)
