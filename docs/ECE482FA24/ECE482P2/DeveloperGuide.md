# Developer Setup 

Full setup and test guide from beginning to end 


## Navigation
- [Home](Home.md)
- [Project Roadmap](RoadMap.md)
- [System Design](Design.md)
- [Performance Improvements](Performance.md)
- [Multi-threading Challenges](MultiThreading.md)
- [Developer Guide](DeveloperGuide.md)

## Prerequisites 

1. C++ compiler: Clang >= 6.0
2. CMake >= 2.7

## Download Program Files 

First, clone the git repository or download the file.

```shell
git clone ssh://git@focs.ji.sjtu.edu.cn:2222/ece482-24fa/p2team-02.git 
cd p2team-02 
```

Download required files and locate them from SJTU school server. 

Required files are `db`, `sample`, `sample_stdout`. 

Value `ID` is student ID like 512212345678. 

```shell
scp -r ID@111.186.59.59:/opt/lemondb/db ./ 
scp -r ID@111.186.59.59:/opt/lemondb/sample ./
scp -r ID@111.186.59.59:/opt/lemondb/sample_stdout ./
```

After download, move downloaded folder files into corresponding locations inside project 

```shell
cp -r ~/Downloads/db ~/path/to/p2team-02 
cp -r ~/Downloads/sample ~/path/to/p2team-02/build
cp -r ~/Downloads/sample_stdout ~/path/to/p2team-02/build 
cp -r ~/Downloads/stdout ~/path/to/p2team-02/build 
```

## Compile

For the first compile, make directory `build`. Compile and run code at the `build` folder. 

```shell
mkdir build && cd build
cmake -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DCMAKE_BUILD_TYPE=Release ../src
cmake --build . -- -j8
cd ..
```

If compile success, you will have a lemondb binary file at `./build/lemondb`. 

Note: You can use `CMAKE_BUILD_TYPE` to switch between debug and release.

Recompile inside `build` folder. 

```shell
cmake -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DCMAKE_BUILD_TYPE=Release ../src
cmake --build . -- -j8
```

## Test by running code, measuring input and output locally 

```shell
cd build
mkdir stdout  
time ./lemondb --listen ./sample/test.query > stdout/test.out
```

## Segmentation Error Debugging Test

```shell
cd ./build 
lldb ./lemondb
run --listen ./sample/test.query
bt
```

## Difference Check

```shell
diff -n ./sample_stdout/test.out ./stdout/test.out
```

This terminal command will show difference between standard output and test output 

## Navigation
- [Home](Home.md)
- [Project Roadmap](RoadMap.md)
- [System Design](Design.md)
- [Performance Improvements](Performance.md)
- [Multi-threading Challenges](MultiThreading.md)
- [Developer Guide](DeveloperGuide.md)
