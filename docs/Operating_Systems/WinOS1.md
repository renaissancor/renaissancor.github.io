# OS & Scheduling (운영체제와 스케줄링)

## 1. **Preemptive vs Non-Preemptive (선점형 vs 비선점형)**

* **Perspective (관점):** 주도권이 OS에 있는가, 실행 중인 작업(Task)에 있는가?
* **Preemptive OS (선점형):** OS가 강제로 쓰레드를 중단시키고 코어 자원을 회수할 수 있습니다. 현대 OS(Windows, Linux 등)의 표준입니다.
* **Non-Preemptive OS (비선점형):** 쓰레드가 스스로 CPU를 양보(Yield)할 때까지 방치합니다. 악의적인 코드 하나에 시스템 전체가 마비될 수 있어 현대 멀티태스킹 환경에서는 불가능합니다.

### **Quantum & Round Robin (할당 시간과 라운드 로빈)**

* **Quantum (Timeslice):** 쓰레드가 한 번에 CPU를 점유할 수 있는 시간(약 15~20ms)입니다.
* **Blocking (블로킹):** `Sleep`, `I/O (send/recv, printf)` 등은 남은 퀀텀을 자발적으로 포기하고 대기 상태로 들어가는 행위입니다. 할 일이 없다면 반드시 블로킹하여 CPU 100% 점유를 막아야 합니다.

---

## 2. Multi-Threading Principles (멀티쓰레딩 원칙)

### **Why Multi-Thread? (멀티쓰레딩의 목적)**

1. **Performance (성능):** 병렬 처리를 통한 속도 향상.
2. **Structural Clarity (구조적 명확성):** 복잡한 작업을 독립적인 쓰레드(재생/버퍼링/통신)로 분리하여 설계.
3. **Responsiveness (응답성):** 무거운 작업 중에도 UI나 메인 루프가 멈추지 않게 함.
4. **Stability (안정성):** 특정 쓰레드의 예외(Exception)가 프로세스 전체의 종료로 이어지지 않게 격리.

### **The Golden Rule (황금률)**

* **Minimize Shared Resources:** 공유 자원을 최소화하는 것이 설계의 핵심입니다. 공유 자원이 많아질수록 **Interlock(경합)**과 **Lock(동기화)**으로 인한 대기 시간이 늘어나 코어가 많아져도 오히려 느려질 수 있습니다.

---

## 3. High Availability & Server Stability (고가용성 및 서버 안정성)

### **HA (High Availability, 고가용성)**

* **Goal:** 24/7/365 서비스 중단 없는 운영.
* **Active-Standby:** 메인 서버 장애 시 즉각 대기 서버로 전환하는 이중화 전략입니다.

### **Cause of Server Down (서버 다운의 원인)**

* 실제 현업에서 하드웨어 결함이나 외부 공격으로 서버가 죽는 경우는 매우 희박합니다.
* **99.99%는 개발자의 코드 실수(Logic Error)** 또는 잘못된 부하 계산(Traffic 예측 실패) 때문입니다. 따라서 안정적인 코드를 짜는 것이 최고의 안정성 확보 수단입니다.

---

## 4. Context Switching Overhead (문맥 교환 비용)

* **Intra-Process (프로세스 내 교체):** 같은 메모리 공간을 공유하므로 상대적으로 가볍습니다.
* **Inter-Process (프로세스 간 교체):** 가상 메모리 테이블(CR3)을 교체해야 하며, **TLB Miss** 및 **CPU Cache Miss**가 발생하여 성능 손해가 막대합니다.
* **Affinity (선호도):** 고성능이 필요한 경우 특정 쓰레드를 특정 코어에 고정하여 캐시 효율을 극대화하기도 합니다.

---

### **Key Summary (핵심 요약)**

* **Thread = Stack + CPU State:** 쓰레드는 실제 작업 단위이며 각자의 스택과 레지스터 상태를 가집니다.
* **Never Make CPU 100%:** 서버 모니터링을 위해 CPU 자원은 언제나 약간의 여유(90% 권장)를 두어야 합니다.
* **Divide and Conquer:** 문제가 생길 가능성이 높은 작업은 프로세스를 분리하거나 독립된 쓰레드로 설계하여 영향을 최소화하세요.
