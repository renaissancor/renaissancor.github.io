# Kernel Object and HANDLE 


## About Kernel Object 

CreateProcess let Windows OS make Process 

Process Status Info 

- Running 
- Blocked
- Ready 

Process Priority Info 

OS Process Scheduler will handle these two info : 

- Process Status Info 
- Process Priority Info 

In Windows File generation of Kernel Object 
Files are also under Windows Kernel's control 

1 to 1, kernerl object per resource 

```txt
 Kernel Object       Resource 
|------------|      |---------|
| Pipe KO    | <--> | Pipe    |
|------------|      |---------|
| Process KO | <--> | Process |
|------------|      |---------|
| Thread KO  | <--> | Thread  |
|------------|      |---------|
```

In Windows OS, Kernel Object is controlled by HANDLE 

For instance Priority change of Process, which is Windows Kernel Object, by 

```cpp
BOOL SetPriorityClass (
    HANDLE hProcess, 
    DWORD dwPriorityClass 
);
```

so Process is accessible by HANDLE datatype 

Compile and Run then 

```shell 
cl Operation1.cpp
cl Operation2.cpp
.\Operation1.cpp 
```

Result is similar to 

```txt 
Operation2.exe
Operation2.exe
Operation2.exe
...
Operation2.exe
Operation2.exe
Operation2.exe
Operation2.exe
Operation2.exe
Operation1.exe
Operation2.exe
Operation2.exe
Operation2.exe
```

Kernel Objects are Subordinate to OS, not Process, while 

HANDLE , aka handle table, are subordinate to Process, not OS 

Types of Kernel Objects include  

- Process 
- Thread 
- Mutex 
- Semaphore 
- Event 
- File / Pipe / Socket 
- Timer / Job Object / Completion Port 

The core difference between `Operation1.cpp` and `KerObjShare.cpp` are 
Last lines that only exist in `KerObjShare.cpp` code. 
```cpp
++timing; 
		if (timing == 2)
			SetPriorityClass(pi.hProcess, NORMAL_PRIORITY_CLASS);
```

Operation2.exe process set priority class of itself by 
getting its own handle by function `HANDLE GetCurrentProcess()` 
which returns `HANDLE` value of process Operation2.exe 


Structure of PROCESS_INFORMATION is following 

```cpp

typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread; 
    DWORD dwProcessId; 
    DWORD dwThreadId; 
} PROCESS_INFORMATION; 
```

Access to Process, which is Kernel Object, by HANDLE 

```txt
|-----------------| 
| Operation2.exe  | 
|-----------------|
        ||
 SetPriorityClass (HANDLE) 
 to HIGH_PRIORITY_CLASS  
        ||
	    \/  
|-----------------| 
|  Kernel Object  | 
|---------------- |
        /\
        ||
 SetPriorityClass (HANDLE) 
 to NORMAL_PRIORITY_CLASS  
        ||
|-----------------| 
| KerObjShare.exe | 
|-----------------|

```

For cases to change Process priority, HANDLE is used 
to access Process, which is Kernel Object. 

```cpp 
// Operation2.cpp
SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS); 
// KerObjShare.cpp 
PROCESS_INFORMATION pi; 
/* ... codes ... */ 
SetPriorityClass(pi.hProcess, NORMAL_PRIORITY_CLASS); 
```

A Process creates B Process, B Process gets its priority higher 
then A Process turn B Process priority to lower again  

## Kernel Object & HANDLE 

HANDLE 은 프로세스 종속적이다 의 의미 
그러면 HANDLE 은 모든 커널 오브젝트 (Thread, Mutex, Semaphore, etc) 등 
다 가리킬 수 있는데  왜 프로세스 종속적이라고 표현하는가? 

HANDLE 이 프로세스 종속적이라는 말은 어떠한 커널 오브젝트를 가리키냐와는 상관 없고, 
그 HANDLE 값 자체가 소속된 프로세스의 HANDLE TABLE 에서만 유요하다는 의미 
A 프로세스에서 생성 된 HANDLE 값으로는 B C D 프로세스의 HANDLE TABLE 에서 무효함 
그래소 HANDLE 값은 PROCESS 종속적 
HANDLE 값이 Kernel Object 만 가리킨다는 뜻이 절대로 아님 
A 프로세스와 B 프로세스가 같은 C 커널 으브젝트를 HANDLE 값으로 접근할 때 
A 프로세스에서 만든 HANDLE 값과 B 프로세스에서 만든 C 커널 오브젝트의 핸들 값은 다르지만 
두 핸들 값 모두 동일한 C 커널 오브젝트를 가리킴 


## Kernel Object & Usage Count 

자식 프로세스의 종료코드는 커널 오브젝트에 저장 
자식 프로세스 종료 시점에 커널 오브젝트도 동시에 소멸되면 
부모 프로세스는 종료코드를 얻을 수 없음 
그래서 프로세스 종료 -> 커널 오브젝트 소멸을 안 함 

그러면 언제 커널 오브젝트를 소멸 ? 
해당 Kernel Object 참조 대상이 하나도 없을 때 소멸 
이 소멸시기 결정을 위하여 Usage Count (UC) 참조 횟수 사용 
Usage Count 가 0이 되면 해당 Kernel Object 소멸 

Process 생성과 동시에 Kernel Object Usage Count 는 1 
UC가 0으로 최초설정되면, 생성과 동시에 소멸될 것 
이후 Kernel Object 접근 가능한 대상이 늘어날수록 UC 증가 
그래서 Child UC는 2, 왜냐하면 
부모 프로세스가 CreateProcess로 자식 Process HANDLE 을 얻음 
PROCESS_INFORMATION pi; // In naming convention 을 통해서 

```txt 
|---------|           |---------|
| Parent  |  Create   | Child   |
| Process | --------> | Process |
|---------|           |---------|
      \                    /\
	   \                   ||
	    \  HANDLE          ||
		 \ return          ||
		  \                \/
		   \____ |---------------|
		         | Child Process |
				 | Kernel Object |
				 |---------------|     
```

이렇게 UC가 2개로 기본 생성 
`OperationStateParent.cpp` 

CloseHandle 함수에 전달되는 자식 프로세스 핸들 
```cpp
CloseHandle(pi.hProcess); 
```
CloseHandle 함수는 핸들을 반환 
커널 오브젝트의 Usage Count UC를 하나 감소시키는 기능 
UC가 0이 되면, OS가 해당 KO 를 알아서 제거 
또한, 프로세스 자체가 종료될 때 일어나는 일도 UC가 하나 적어지는 것 


CloseHandle 함수와 Process 종료는 완전히 별개 
프로세스 종료 함수는 TerminateProcess 강제 종료 요청, 무조건 종료시켜라 
그래서 부모 프로세스여도, 자식 프로세스를 강제 종료시키는 것은 좋지 않음 
강제 종료 시 일반적 종료와 달리 문제 생김 
파일 입 출력 과정 프로세스 갑자기 종료 상황 
프로세스가 자기 일 마치고 마지막 return 문으로 종료하는 게 제일 안정적, 
TerminateProcess 사용은 비추천 

`Calculator.cpp `

커널 오브젝트는 Usage Count UC 값을 가지고 있음 
UC 값은 해당 커널 오브젝트를 참조하는 HANDLE 개수 

만약에 자식 프로세스를 만들고 닫아주기를 반복하는데, 
```cpp
case ELSE:
	ZeroMemory(&pi, sizeof(pi));
	CreateProcess(
		NULL, command, NULL, NULL,
		TRUE, 0, NULL, NULL, &si, &pi
	);
	// CloseHandle(pi.hProcess); 
	// CloseHandle(pi.hThread); 
	break;
```
이렇게 핸들을 닫아버리지 않으면, UC가 여전히 부모 프로세스가 참조하고 있는 1이 됨 
만약 핸들을 계속 닫지 않는다면, 
자식 계산기 프로세스 자체는 끝나도 생성되었던 자식 프로세스 UC가 계속 누적 

```txt 
|---------|       |---------|
| Parent  | ----> | Parent  |
| Process | ---|  | Knl Obj |
| Exist   | -| |  | UC = 1  |
|---------|  | |  |---------|
             | |
|---------|  | |  |---------|
| Child   |  | |  | Child   |
| Process |  | |->| Knl Obj |
| Deleted |  |    | UC = 1  |
|---------|  |    |---------|
             |
|---------|  |    |---------|
| Child   |  |    | Child   |
| Process |  |--->| Knl Obj |
| Deleted |       | UC = 1  |
|---------|       |---------|
```

이런 방식으로, CloseHandle 누락한 상태로 반복적으로 Child Process를 생성한다면 
Child Process는 모두 종료되어 있지만, 해당 Process 의 Kernel Object는 
계속 사라지지 않고 누적됨. Parent Process에서 생성한 HANDLE을 계속 들고 있기 때문  

바탕화면은 프로세스 
Ctrl Alt Delete > Windows 작업 관리자 
Explorer.exe // 바탕 화면 (탐색기) 
프로세스는 기본적으로 생성과 동시에 Usage Count 가 2가 됨 

## Command Prompt Project Functions 

명령 프롬프트 프로젝트 기능 


