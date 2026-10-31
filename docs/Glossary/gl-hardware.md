---
title: "Glossary: Hardware"
tags:
  - glossary
  - hardware
---

# Glossary: Hardware

The words you must recognize instantly, not the theory behind them.

### Processors

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| ISA | Instruction Set Architecture | the contract between software and CPU: x86-64, Arm, RISC-V | |
| ALU / FPU | Arithmetic Logic / Floating-Point Unit | integer / floating-point execution units | |
| SIMD | Single Instruction, Multiple Data | one instruction, many values | "that loop didn't vectorize" |
| AVX | Advanced Vector Extensions | x86 SIMD; AVX-512 is the 512-bit version | |
| SMT | Simultaneous Multithreading | multiple hardware threads per core, usually two on x86; Intel calls it Hyper-Threading | "turn SMT off" |
| IPC | Instructions Per Cycle | CPU efficiency. In OS talk, inter-process communication | |
| NPU / TPU | Neural / Tensor Processing Unit | ML accelerators; TPU is Google's | |
| ASIC | Application-Specific Integrated Circuit | chip built for one purpose | |
| FPGA | Field-Programmable Gate Array | rewirable logic | |
| SoC | System on Chip | CPU, GPU and more on one chip. Several chips in one package is strictly a SiP. Not Security Operations Center | |
| TDP | Thermal Design Power | rated power for cooling design; varies by vendor. Intel: sustained at base frequency, turbo can exceed it. GPU figures: a maximum | |

### Memory

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| SRAM / DRAM | Static / Dynamic RAM | fast cache cells / main memory that needs refresh | |
| L1 / L2 / L3 | Level 1 / 2 / 3 cache | smaller and faster toward the core | |
| LLC | Last-Level Cache | the final cache before DRAM | "LLC misses" |
| MESI | Modified, Exclusive, Shared, Invalid | cache-coherence states | |
| NUMA | Non-Uniform Memory Access | memory on the other socket costs more | "pin it to one NUMA node" |
| DIMM | Dual In-line Memory Module | a RAM stick | "one DIMM is throwing errors" |
| ECC | Error-Correcting Code | memory that detects and corrects bit errors; SECDED fixes one bit and detects two | "ECC errors on GPU 2" |
| HBM | High Bandwidth Memory | stacked DRAM beside the GPU die | |
| GDDR | Graphics Double Data Rate | graphics-card DRAM: narrower and faster per pin than HBM, lower total bandwidth | |
| VRAM | Video RAM | GPU memory, whatever the technology | "doesn't fit in VRAM" |

### Storage

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| SSD / HDD | Solid-State / Hard Disk Drive | flash / spinning disk | |
| NVMe | Non-Volatile Memory Express | SSD protocol, normally over PCIe; NVMe-oF carries it over networks | |
| RAID | Redundant Array of Independent (originally Inexpensive) Disks | disks combined for redundancy or speed | "the RAID is degraded" |
| IOPS | I/O Operations Per Second | storage load | "we hit the IOPS cap" |

### Buses and links

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| PCIe | Peripheral Component Interconnect Express | point-to-point host interconnect for cards. Gen5 x16 is generation and lane count | "is it running at x16?" |
| CXL | Compute Express Link | memory and device coherence on the PCIe physical layer | |
| IOMMU | Input-Output MMU | address translation and isolation for devices | |
| SR-IOV | Single Root I/O Virtualization | one PCIe device appears as many | |
| NVLink | | NVIDIA's high-speed GPU interconnect. Carries GPU-to-GPU traffic instead of PCIe. On x86 systems the host link stays PCIe; Grace superchips reach the CPU over NVLink-C2C | |
| NVSwitch | | switch chip giving all GPUs full NVLink bandwidth | |

### Server and GPU packaging

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| SXM | NVIDIA's module form factor | GPU module mounted on a baseboard, not a PCIe card | "SXM or PCIe cards?" |
| HGX | NVIDIA product name | baseboard platform of SXM GPUs linked by NVLink; the 8-GPU boards add NVSwitch | |
| OAM | OCP Accelerator Module | open mezzanine accelerator module, playing the same role as SXM | |
| OCP | Open Compute Project | open datacenter hardware specs | |
| BMC | Baseboard Management Controller | small computer for out-of-band server control | "reboot it through the BMC" |
| IPMI | Intelligent Platform Management Interface | protocol to talk to the BMC | |
| BIOS / UEFI | Basic Input/Output System / Unified Extensible Firmware Interface | firmware that boots the machine | |
| PSU | Power Supply Unit | | "a PSU failed, the node stayed up on the other one" |
| RU / U | Rack Unit | 1.75 inches of rack height | "a 4U box" |
