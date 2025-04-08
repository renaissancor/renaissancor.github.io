# Project 2 LemonDB Multi Threading Database

2024FA ECE4820 Introduction to Operating Systems


## Navigation
- [Home](Home.md)
- [Project Roadmap](RoadMap.md)
- [System Design](Design.md)
- [Performance Improvements](Performance.md)
- [Multi-threading Challenges](MultiThreading.md)
- [Developer Guide](DeveloperGuide.md)


## Overview

LemonDB is a multi-threaded database system project.
It assigns each processes queries in parallel for faster execution.

Project 2, named as LemonDB in this fictional home-brewed project of Lemonion.inc,
is a team based assignment to program database from source code of previous system.
Based on the concepts taught in the class, our team programmed the most efficient and
optimized version of multi-threading database by considering parallel multi threading.
By the time project is completed, the new implementation will be able to handle parallel
query execution by concurrency, allowing heavy workload of multiple query execution in
short period of time by optimized scheduling.

Also, during the software development, team members will try to follow the principles
for the agile development, clean documentation for better maintainability in the future,
and software testing and benchmarking compared to the original system.
Tasks will be divided and distributed as evenly as possible,
but more competent member will contribute more to the project.


## Team Members, Roles and contributions

Jae Heung Park

- Implementation of `MAX` `MIN` `SWAP` functions in single thread.
- Implementation of read  only `MAX` `MIN` `SUM` `SELECT` `COUNT` queries in multi thread.
- Documentation of `README` file and Wiki for project release.
- Compiling and debugging codes to run test cases in local environment.
- Detecting segmentation error and finding problematic queries.
- Discovering no implementation of `COPYTABLE` `TRUNCATE` and fix those.
- Supportive contribution to the project with effort

He Wentao

- Project basic setup from given source code and functions.
- Compiling and debugging codes to run test cases in local environment.
- Code testing from server to check optimization and performance.
- Single thread functions implementation `ADD` `SUB` `COUNT` `DELETE`
- Algorithm design implementation for multi threading
- Changing entire query functions to apply optimization algorithm
- Implementation of write queries in multi thread.
- High contribution to the project with great skills and effort

Wang Rui

- Project template update, comment, and developer guide documentation
- Single thread functions implementation `SUM` `DELETE` `DUPLICATE`
- Implementation of read  only `MAX` `MIN` `SUM` `SELECT` `COUNT` queries in multi thread.
- Algorithm design research and implementation for multi threading
- Implementation of write queries in multi thread.
- Compiling and debugging codes to run test cases in local environment.
- High contribution to the project with great skills and effort


## Navigation
- [Home](Home.md)
- [Project Roadmap](RoadMap.md)
- [System Design](Design.md)
- [Performance Improvements](Performance.md)
- [Multi-threading Challenges](MultiThreading.md)
- [Developer Guide](DeveloperGuide.md)
