# IPC Inter Process Communication 


## IPC Meaning 

Inter-Process Communication 
프로세스 사이의 통신 

통신 Communication, 데이터 주고받기 
프로세스들은 서로 만날 수 없음 공간이 독립적으로 분리

```txt
 Process A Process B
|--------| |--------|
|  DATA  | |  DATA  |
|--------| |--------|
|  HEAP  | |  HEAP  |
|~~~~~~~~| |~~~~~~~~|
| STACK  | |  STACK |
|--------| |--------|
```

두 다른 프로세스의 메모리 구조 
서로 만날 수 없도록 디자인 

## Mail Slot Based IPC 

Windows OS는 다양한 IPC 기법 제공 
메일 슬롯 Mail Slot 도 그중 하나 
편지를 넣을 수 있는 우체통 입구 
우체통은 Mail Box 

IPC를 위한 
Kernel Object MailSlot 을 먼저 만들어야 함 
```cpp
HANDLE CreateMailSlot (
    LPCTSTR lpName, 
    DWORD nMaxMessageSize,
    DWORD lReadTimeOut, 
    LPSECURITY_ATTRIBUTES lpSecurityAttributes
); 
```
if function `CreateMailSlot` fails, return value is `INVALID_HANDLE_VALUE`


## Singnaled VS Non-Signaled 

IPC 커널 오브젝트 상태 

Signaled 상태 (신호를 받은 상태)

Non-Signaled 상태 (신호를 받지 않은 상태) 

Kernel Object status change from 
- Signaled to Non-Signaled 
- Or Non-Signaled to Signaled 

Kernel Object state info 

Process end 
Non Signaled to Signaled or Vice Versa 

