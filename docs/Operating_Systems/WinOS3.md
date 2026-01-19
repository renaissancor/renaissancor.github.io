# OS Interrupts 

## 1. Dispatcher & Context Switching (디스패처와 문맥 교환)

디스패처는 "어떤 스레드를 실행할지" 결정하고 실제로 "교체"를 수행하는 커널의 핵심 실행부입니다.

* **Context Switching = Dispatcher:** 스케줄러가 정책(누가 돌 것인가)을 세운다면, 디스패처는 실제 **SW Interrupt**를 발생시켜 현재 CPU의 레지스터들을 TCB(Thread Context Block)에 백업하고 새로운 TCB로 복원합니다.
* **The Transition Point:** 스레드 A가 멈추고 B가 시작되는 지점은 정확히 디스패처가 작동하는 구간입니다. 스레드 B 입장에서는 과거에 디스패처 내부에서 멈췄던 지점부터 다시 실행되는 것입니다.
* **Stealing Threads (Ready Queue):** Windows 8/10 이후, 특정 CPU의 레디 큐가 비어있으면 해당 CPU의 디스패처가 다른 CPU의 레디 큐에 있는 스레드를 가져와 실행(Wandering)합니다. 이는 부하 분산을 위한 설계입니다.

---

## 2. Advanced Hardware Interrupts (현대적 인터럽트 처리)

과거의 물리적 핀 신호 방식에서 메시지 기반의 효율적인 방식으로 진화했습니다.

* **MSI (Message Signaled Interrupts):** 하드웨어가 특정 메모리 주소에 메시지를 써서 인터럽트를 알리는 방식입니다. PCI 장치 관리자에서 MSI로 표기되며, 과거보다 정교한 CPU 배정이 가능합니다.
* **RSS (Receive Side Scaling):** 네트워크 트래픽(NIC)을 해시 함수를 통해 여러 CPU에 분산 처리하는 기술입니다.
* **주의:** 특정 CPU에 부하를 고정(Affinity)하고 싶다면 RSS를 꺼야 할 수도 있습니다. 하지만 RSS는 DPC 레벨에서 병렬성을 극대화하기 때문에 무작정 끄는 것이 성능 향상을 보장하지는 않습니다.


* **NIC Performance:** 메인보드 내장 인텔/리얼텍 칩셋은 최적화가 잘 되어 있으나, 저가형 PCI 슬롯 기반 카드는 1Gbps 대역폭을 다 쓰지 못하고 패킷 유실이 발생할 수 있습니다.

---

## 3. Starvation & Balance Set Manager (기아 현상 방지)

선점형 OS에서 우선순위가 낮은 스레드가 영원히 굶는 것을 방지하는 메커니즘입니다.

* **Balance Set Manager:** 1초에 한 번씩 깨어나는 시스템 스레드입니다.
* **Priority Boosting:** 레디 큐를 스캔하여 **4초 이상** 대기 중인 스레드를 찾으면, 우선순위를 **최대치(15)**로 올리고 퀀텀을 부여합니다. 수행이 끝나면 원래 순위로 돌아갑니다.
* **Limit:** 한 번에 모든 스레드를 검사하지 않고 16개씩 스캔하며, 최대 10개까지만 부스팅하여 시스템 부하를 조절합니다.

---

## 4. APIC & Quantum Accounting (APIC와 정밀한 시간 계산)

* **LAPIC (Local APIC):** 멀티코어 환경에서 각 코어마다 존재하는 타이머 인터럽트 컨트롤러입니다. 퀀텀 차감은 각 코어의 LAPIC에서 독립적으로 관리됩니다.
* **Vista 이후의 변화:** XP까지는 HW 인터럽트 처리 시간을 유저 스레드의 퀀텀 소비로 계산(유저 손해)했으나, **Vista 이후**에는 실제 CPU 사이클(Clock)을 측정하여 인터럽트 시간은 퀀텀에서 제외합니다. 유저 모드 코드가 더 공정하게 퀀텀을 보장받게 되었습니다.

---

## 5. Kernel Stability & IRQL (커널 안정성과 IRQL)

커널 모드에서 코드가 실행될 때 가장 주의해야 할 부분입니다.

* **NPP (Non-Paged Pool) 필수:** ISR이나 DPC 레벨에서는 **Page Fault를 처리할 수 없습니다.** (Page In을 담당하는 APC 레벨보다 높기 때문). 따라서 이 구간의 코드는 반드시 물리 메모리에 고정된 NPP에서만 돌아야 하며, 위반 시 블루스크린(BSOD)이 발생합니다.
* **Atomic Operations:** 싱글 코어 시절에는 인터럽트 마스킹만으로 동기화가 가능했으나, 멀티코어에서는 인터럽트 차단만으로는 부족하며 **Lock/Atomic 연산**이 필수입니다.

---

## 6. Deadlock (교착 상태) 4대 조건

면접에서 반드시 물어보는 내용이며, 이를 방지하기 위한 설계적 접근이 필요합니다.

1. **Mutual Exclusion (상호 배제):** 자원은 한 번에 한 프로세스만 사용 가능.
2. **Hold and Wait (점유와 대기):** 자원을 가진 채 다른 자원을 기다림.
3. **No Preemption (비선점):** 할당된 자원을 강제로 뺏을 수 없음.
4. **Circular Wait (순환 대기):** 대기 고리가 원형을 이룸.

---

