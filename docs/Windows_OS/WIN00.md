# CMakeLists Setup 

This is written during concern about 
writing multiple CMakeLists.txt files inside the 
folder structure to study systems programming. 

There are several `.cpp` files including `main` functions. 
Meaning that several `exe` files will be generated. 

Compile and run are possible by powershell CLI, 
but the problem is using VS2022 debug functions 
while maintaining complicated CMakeLists setup hierarchy. 

Here is my scratch Directory hierarchy

```txt
C:.
├── CMakeLists.txt              <-- Root CMakeLists.txt
├── CH03/
│   ├── Example3_1.cpp
│   ├── Example3_2.cpp
│   └── CMakeLists.txt          <-- CH03's CMakeLists.txt
├── CH05/
│   ├── CreateProcess.cpp
│   ├── AdderProcess.cpp
│   ├── ThreadSync.cpp
│   └── CMakeLists.txt          <-- CH05's CMakeLists.txt
└── CH06/
    ├── MutexExample.cpp
    └── CMakeLists.txt          <-- CH06's CMakeLists.txt
```

So, to compile and debug inside VS2022 with CMakeLists.txt 
switching into multiple different .cpp .exe files 

First, configuration of `CMakeLists.txt` in root folder is like this 

### Root `CMakeLists.txt` File 

```cmake 
# Root CMakeLists.txt 
cmake_minimum_required(VERSION 3.15)
project(SystemProgrammingPractice)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(CH03)
add_subdirectory(CH05)
add_subdirectory(CH06) 
```

Second, configuration of `CMakeLists.txt` in chapter folder is like this 

### Chapter `CMakeLists.txt` File 


```cmake


```