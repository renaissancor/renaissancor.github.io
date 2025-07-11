
LLP64 VS LP64 

char short int long pointer 
WORD 
DWORD 

BYTE 
WORD 
DWORD (Double Word) 
DWORD is simply unsigned long 
QWORD (Quadra Word)

HANDLE 
Kernel Object ID 

Kernel Object 
커널 오브젝트 

CreateProcess 기반 프로세스 생성 
Windows OS가 요구사항에 맞게 Process 생성
즉 Process 생성 관리 소멸 주체는 OS 

Process Status Info 
Running Blocked Ready 
Process Priority Info 

프로세스 상태 정보 : Running Blocked Ready
프로세스 우선순위 정보 
이 두 정보 갱신 시 Process Scheduler 가 프로세스 관리 
프로세스 관리하는 이런 구조체가 존재할 것 

프로세스 내부에서 생성된 스레드도 커널 오브젝트 
IPC (Inter Process Communication) 파이프 / 메일 슬롯도 커널 오브젝트 
그래야, OS가 관리 

Windows 에서는 파일 생성 시에도 Kernel Object 생성, 왜냐하면 
파일도 Windows Kernel 의 관리 대상 

그리고 Kernel Object Type 종류별로 모양도 다름 

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

이런 1대 1 대응 방식 
그리고 Windows OS에서 Kernel Object 를 조작하는 방법은 
HANDLE 을 이용하는 것 예를 들어, 
어떤 KO 의 우선순위 변경하는 함수 
```cpp
BOOL SetPriorityClass (
    HANDLE hProcess, 
    DWORD dwPriorityClass 
);
```
여기서 hProcess 는 Process 의 HANDLE 
dwPriorityClass 는 우선순위 값 



