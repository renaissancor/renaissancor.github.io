

32 Bit OS VS 84 Bit OS 
IO Bus 32Bit or 64 Bit 

명령어 Fetch 를 
IO BUS 를 통해서 데이터를 가져가게 됨 
데이터 이동 한번에 32비트를 움직이냐 64비트를 움직이냐 


HW Perspective 

Step 1 Fetch Command From IO Bus to Bus Interface 
Step 2 Decode Command by Control Unit 
Step 3 Execute in ALU (Arithmetic Logic Unit) 

Programmer Perspective 32 Bit VS 64 Bit 
체감이 크지 않지만 

만약 4Bit OS면, 0000 부터 1111까지 16가지 
16가지 포인터밖에 표현 못함 ... 

윗부분과 아랫부분의 Addressing Type는 일치해야 함 

32Bit OS Pointer 는 2^32 = 4294967296 ~= 4GB 
64Bit OS Pointer 는 2^64 = 18446744073709551616 ~= 

포인터가 크면 메모리 접근 범위도 커짐 

If Pointer Size is 64Bit and IO Bus is 32 Bit, then 
IO Bus should be used twice to send address, meaning 
performance decrease. 

So, Memory is best when same size of Bus memory transfer maximum. 


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



