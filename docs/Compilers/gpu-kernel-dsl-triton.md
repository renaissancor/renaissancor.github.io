---
title: "Triton: Python-Syntax GPU Kernels Without CUDA C++"
---

# Triton: Python-Syntax GPU Kernels Without CUDA C++

## Triton does not generate CUDA C++ — it compiles straight to PTX

A common mental model is *Python → CUDA C++ → nvcc → binary*. There is no C++ step:

```
@triton.jit Python function   (source is parsed, never run by the interpreter)
  → Triton IR (MLIR)           (tiling, coalescing, shared-memory planning)
  → LLVM IR
  → PTX                        (NVIDIA's GPU assembly — where CUDA C++ also lands)
  → ptxas → cubin              (the same assembler nvcc uses)
```

So Triton replaces *authoring* kernels in CUDA C++, not CUDA itself: launch, memory and the driver are still the CUDA stack. Kernels are JIT-compiled on first call and cached (`~/.triton/cache`). Because the backend is LLVM, the same kernel can also target AMD GPUs.

## It is an embedded DSL, not "Python on the GPU"

Scala and Java share JVM bytecode the way Triton and CUDA C++ share PTX — but the analogy breaks: Scala is a full language, Triton is not. Triton borrows Python's **grammar** and supplies its own **semantics**: `x + y` adds whole tiles, and dicts, classes and arbitrary library calls are not allowed. You write per-tile (`tl.arange(0, BLOCK)`, `tl.load` of a block) instead of CUDA's per-thread model, and the compiler maps tiles onto threads and memory. The closer analogy is SQL embedded in a Java program.

## Where it shows up in LLM serving

New layer types get Triton kernels before anyone hand-tunes CUDA for them. vLLM exposes `--gdn-prefill-backend {flashinfer,triton}` for Gated DeltaNet (the linear-attention layers in hybrid models such as Qwen3.5+); the Triton path is a documented workaround for a FlashInfer prefill deadlock (vllm#38916). Triton's first-call compilation and autotuning are also part of why a freshly started server is slow on its first requests.

**Open:** throughput cost of the Triton GDN prefill path vs FlashInfer; what the autotuner actually searches (block sizes, warps, pipeline stages).
