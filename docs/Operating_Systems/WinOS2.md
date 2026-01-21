# Thread Kernel 

## 1. Thread Structures & GS Register (쓰레드 구조체와 GS 레지스터)

* **TCB (Thread Control Block) & PCB (Process Control Block):** Managed by the Kernel. These contain essential scheduling info. (커널이 관리하며, 스케줄링에 필요한 핵심 정보를 담고 있습니다.)
* **TEB (Thread Environment Block):** Located in User Mode RAM. It contains Thread ID, TLS (Thread Local Storage), etc. (유저 모드 메모리에 있으며, 쓰레드 ID나 TLS 정보를 담고 있습니다.)
* **GS Register:** In 64-bit Windows, the GS register points to the TEB. Assembly instructions use this to access thread-specific data quickly. (64비트 윈도우에서 GS 레지스터는 TEB를 가리키며, 어셈블리 명령어가 이를 통해 쓰레드 데이터에 빠르게 접근합니다.)

---

## 2. Context Switching vs. Kernel Mode Switch (문맥 교환 vs 커널 모드 전환)

* **Kernel Mode Switch:** Occurs during System Calls or Interrupts. The thread is still "Running," but its privilege level rises. (시스템 콜이나 인터럽트 시 발생하며, 쓰레드는 여전히 실행 상태이지만 권한 레벨만 상승합니다.)
* **Context Switching:** Occurs when the **Quantum (Time Slice)** expires or a thread enters a **Blocked** state. This involves saving/loading TCBs. (퀀텀 만료나 블록 상태 진입 시 발생하며, TCB를 저장하고 교체하는 무거운 작업입니다.)
* **Overhead (오버헤드):** Switching between threads in **different processes** is much costlier due to **Address Space changes**, leading to **TLB Misses** and **Cache Misses**. (타 프로세스 간 교체는 주소 공간 변경으로 인해 TLB 및 캐시 미스가 발생하여 훨씬 더 무겁습니다.)

---

## 3. Interrupts & The Scheduler (인터럽트와 스케줄러)

* **Timer Interrupt (IRQ 0):** The **Heartbeat of the OS**. The Scheduler is effectively the **Timer Interrupt Service Routine (ISR)**. (OS의 심장 박동과 같습니다. 스케줄러는 사실상 타이머 인터럽트 서비스 루틴입니다.)
* **Interrupt Service Routine (ISR):** Executed immediately by the CPU upon hardware signal. It must be extremely fast (< 25μs). (하드웨어 신호 즉시 CPU가 실행하며, 매우 빨라야 합니다.)
* **DPC (Deferred Procedure Call):** Handles non-critical, longer tasks deferred from the ISR. It has its own queue per CPU. (ISR에서 미뤄진 덜 급한 작업을 처리합니다. CPU마다 전용 큐를 가집니다.)
* **Dispatcher:** A Software Interrupt that performs the actual **Context Switch**. (실제 문맥 교환을 수행하는 소프트웨어 인터럽트입니다.)

---

## 4. Interrupt Request Level (IRQL) & Priority

Windows manages execution priority through **IRQL**. Higher IRQL tasks can preempt lower ones. (윈도우는 IRQL을 통해 실행 우선순위를 관리하며, 높은 레벨이 낮은 레벨을 선점합니다.)

| Level | Description |
| --- | --- |
| **Hardware** | Physical device signals (Keyboard, Disk, NIC) |
| **DPC/Dispatcher** | Deferred tasks and Context Switching |
| **APC** | Asynchronous Procedure Calls |
| **User** | Normal thread execution (가장 낮은 순위) |

* **Thread Priority:** Only matters within the **User** level. Even a high-priority thread can be paused by a **Hardware Interrupt**. (쓰레드 우선순위는 유저 레벨 내에서만 유효하며, 하드웨어 인터럽트가 발생하면 무조건 멈춥니다.)

---

## 5. Kernel Stack (커널 스택)

* Every thread has a **separate Kernel Stack** (typically 4KB~). (모든 쓰레드는 별도의 커널 스택을 가집니다.)
* **Why per thread?** Since Context Switching can happen even while in Kernel Mode, each thread needs its own space to save its state. (커널 모드 상태에서도 문맥 교환이 일어날 수 있으므로, 각자의 상태를 저장할 독립된 공간이 필요합니다.)

--- 

* **TimeBeginPeriod(1):** Raising timer resolution makes `Sleep()` more accurate but increases Context Switching frequency, potentially lowering overall system throughput. (타이머 해상도를 높이면 정밀도는 올라가지만, 스케줄링이 너무 자주 발생해 전체 성능은 떨어질 수 있습니다.)
* **Hyper-Threading:** It shares L1/L2 caches. If two threads on the same physical core fight for cache, it can cause **Stalls**. In high-performance servers, sometimes it's better to disable it or use **Core Affinity**. (하이퍼쓰레딩은 캐시를 공유하므로 경합이 발생하면 성능이 저하될 수 있습니다. 고성능 서버에서는 코어 선호도를 수동으로 관리하기도 합니다.)

Windows XP, Vista OS 의 경우 Quantum을 상수로 두고, ISR / DPC 를 처리할 경우에도 일관적으로 
스레드의 Quantum 을 상수로 1씩 차감했기 때문에 실제로는 스레드가 ISR / DPC 처리에 사용한 시간을 보상받지 못 
했고, Windows 7 이후 OS 는 이제 Quantum 을 Timer Interrupt Count 대신 CPU Tick 기반으로 
부여하고, ISR / DPC 의 시작이나 종료 시간을 레지스터에 저장한 후 Quantum 에 보충하는 식으로 해당 스레드의 
부여받은 퀀텀 CPU Frequency 만큼 보상함. Timer Interrupt 는 이제 해당 스레드가 Quantum 틱을 모두 
소진하였는지만 확인하고 소진 완료 시 Dispatcher 로 TCB 노드를 만든 후 레디 큐에 집어넣으면서 다른 TCB를 레디 
큐에서 가져오는 방식으로 컨텍스트 스위칭을 진행함. 
