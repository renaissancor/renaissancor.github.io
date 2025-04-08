# System Design

Software design schema structure overview and description


## Navigation
- [Home](Home.md)
- [Project Roadmap](RoadMap.md)
- [Risk Assessment](Risk.md)
- [Performance Analysis](Performance.md)
- [Developer Guide](Developer.md)


## Lottery Scheduling

Lottery Scheduling is a probabilistic scheduling algorithm used in multi-tasking operating systems, particularly for CPU resource allocation. The core idea is to assign a number of "lottery tickets" to each process, and the process that wins the lottery (i.e., the one with the randomly drawn ticket) is granted CPU time. This approach ensures fair distribution of CPU time, with the option to adjust the number of tickets to prioritize certain processes.

1. Ticket Allocation: Each process is assigned a certain number of tickets based on its weight or priority. The more tickets one process have, the higher chance to be scheduled.
2. Randomness: CPU allocation is determined by a random lottery draw, ensuring fairness in scheduling.
3. Priority Adjustment: The number of tickets can be adjusted to change the process's priority. More tickets mean higher priority.
4. Use Case: Ideal for scenarios where fair resource distribution and flexible priority adjustment are needed.
