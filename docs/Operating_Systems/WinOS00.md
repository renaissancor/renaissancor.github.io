# Windows Operating Systems Voca 

## 1. Thread & Process Architecture (쓰레드 및 프로세스 구조)

* **TCB (Thread Control Block):** 커널이 관리하는 쓰레드 제어 블록. 쓰레드의 상태, 우선순위, 레지스터 컨텍스트 등을 보관.
* **TEB (Thread Environment Block):** 유저 모드에서 접근 가능한 쓰레드 정보 블록. Thread ID, TLS 주소 등을 포함하며 64비트 Windows에서는 **GS 레지스터**가 이를 가리킴.
* **PCB (Process Control Block):** 프로세스 제어 블록. 주소 공간 정보, 열린 핸들 테이블 등 프로세스 전체 자원을 관리.
* **TLS (Thread Local Storage):** 쓰레드별 독립적인 전역 저장 공간. `_beginthreadex` 사용 시 CRT가 이를 적절히 초기화함.
* **Fiber (파이버):** Windows에서 제공하는 **유저 모드 쓰레드**. OS가 아닌 프로그래머가 직접 스케줄링(Cooperative Scheduling)해야 함.

---

## 2. Scheduling & Execution (스케줄링 및 실행)

* **Quantum (퀀텀/Timeslice):** 쓰레드가 한 번에 CPU 코어를 점유할 수 있는 최대 시간 단위 (Windows 기준 보통 15~20ms).
* **Context Switch (문맥 교환):** 현재 실행 중인 쓰레드의 상태(TCB)를 저장하고 다음 쓰레드를 코어에 올리는 과정. **Dispatcher**가 수행.
* **Preemption (선점):** OS가 강제로 실행 중인 쓰레드를 중단시키고 다른 쓰레드에 CPU를 할당하는 행위.
* **Starvation (기아 현상):** 낮은 우선순위의 쓰레드가 높은 우선순위 쓰레드들에 밀려 영원히 CPU를 할당받지 못하는 상태.
* **Balance Set Manager:** Windows의 시스템 쓰레드로, 4초 이상 대기한 쓰레드의 우선순위를 일시적으로 15(최고치)로 올려 기아 현상을 방지함.

---

## 3. Interrupt & Hardware Interaction (인터럽트 및 하드웨어 제어)

* **ISR (Interrupt Service Routine):** 하드웨어 인터럽트 발생 시 CPU가 즉시 실행하는 함수. 매우 짧고 빨라야 함(권장 25μs 이내).
* **DPC (Deferred Procedure Call):** ISR에서 즉시 처리하기 힘든 무거운 작업을 미뤄서 처리하기 위한 커널 메커니즘. CPU마다 큐(Queue)가 존재함.
* **IRQL (Interrupt Request Level):** Windows 커널의 우선순위 레벨. 높은 IRQL은 낮은 IRQL의 코드를 중단시킬 수 있음 (예: Hardware > DPC/Dispatcher > APC > User).
* **APIC (Advanced Programmable Interrupt Controller):** 현대 멀티코어 시스템에서 인터럽트를 각 CPU 코어에 분배하고 관리하는 장치.
* **MSI (Message Signaled Interrupts):** 하드웨어가 메모리에 메시지를 쓰는 방식으로 인터럽트를 알리는 현대적 방식.
* **RSS (Receive Side Scaling):** NIC(네트워크 카드)에 들어오는 트래픽을 여러 CPU 코어에 분산시켜 DPC 처리를 병렬화하는 기술.

---

## 4. Memory & Performance (메모리 및 성능)

* **NPP (Non-Paged Pool):** 물리 메모리에 항상 상주하며 디스크로 페이지 아웃(Page Out)되지 않는 메모리 영역. **ISR/DPC** 코드는 반드시 여기서 실행되어야 함.
* **TLB (Translation Lookaside Buffer):** 가상 주소를 물리 주소로 변환할 때 사용하는 고속 캐시. 프로세스가 바뀌면 이 캐시가 무효화(Flush)되어 성능 저하 발생.
* **Affinity (선호도):** 특정 쓰레드나 인터럽트 처리를 특정 CPU 코어에 고정하는 설정. 캐시 효율을 높이기 위해 사용.
* **Syscall (System Call):** 유저 모드에서 커널 기능을 호출하기 위한 인터페이스. `INT 2E`나 `SYSCALL` 어셈블리 명령어로 수행.

---

## 5. Critical Issues & Debugging (주요 이슈 및 디버깅)

* **Deadlock (교착 상태):** 두 개 이상의 쓰레드가 서로가 가진 자원을 기다리며 무한 대기에 빠지는 현상.
* **Race Condition (경합 조건):** 여러 쓰레드가 공유 자원에 동시에 접근하여 실행 순서에 따라 결과가 달라지는 오류.
* **BSOD (Blue Screen of Death):** 커널 레벨에서 복구 불가능한 오류(예: IRQL_NOT_LESS_OR_EQUAL) 발생 시 시스템 보호를 위해 중단되는 화면.
* **Atomic Operation (원자적 연산):** 중간에 인터럽트나 다른 쓰레드가 개입할 수 없는 최소 단위의 연산.

---

### **요약 가이드 (Cheat Sheet)**

| 구분 | 유저 모드 (User Mode) | 커널 모드 (Kernel Mode) |
| --- | --- | --- |
| **권한** | 제한적 (Ring 3) | 무제한 (Ring 0) |
| **스택** | User Stack (1MB 기본) | Kernel Stack (작고 고정됨) |
| **데이터 구조** | TEB, CRT Heap | TCB, PCB, NPP/PP |
| **종료** | `return`, `exit()` | `_endthreadex` (정리용) |
| **중단** | `Sleep(0)`, `yield` | Preemption, Interrupt |

---
