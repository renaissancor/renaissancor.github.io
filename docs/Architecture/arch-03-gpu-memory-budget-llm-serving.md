---
title: "GPU Memory Budget in LLM Serving"
---

# GPU Memory Budget in LLM Serving

An inference server like vLLM treats a GPU as one fixed budget, decided at startup:

```
total VRAM × --gpu-memory-utilization
  = weights + activations / CUDA graphs + KV cache (whatever is left)
KV tokens ÷ tokens per request ≈ concurrency before preemption
```

## Two models can share one GPU

`--gpu-memory-utilization` is a fraction of **total** VRAM, claimed up front and never exceeded. So two servers on one GPU coexist as long as their fractions sum to under about 0.95. I ran a dense 31B and a 26B-A4B MoE, both FP8 at 16k context, side by side on one 141 GB H200: 12 evaluation runs with zero connection or parsing errors.

The cost is not stability. Each model gets a much smaller KV pool, so it starts preempting and re-prefilling at a lower concurrency than it would alone.

## Start order matters

The slice is sized as total × utilization, but the startup check compares that against memory that is free *at that moment*. Start the inference servers first, then any process that allocates GPU memory directly. Otherwise the launch fails with a "free memory < desired" error even though the GPU is nominally big enough.

## The server's context length reserves KV, not the client's `max_tokens`

Measured on one image-classification workload:

| Change | Throughput |
|---|---|
| Baseline, server `--max-model-len 16384`, client `max_tokens 8192` | 5.63 img/s |
| Client `max_tokens` → 5120 only | 5.58 img/s (no change) |
| Also server `--max-model-len` → 10240 | 6.19 img/s (+11%) |

Shrinking the output budget in the request does nothing for capacity. The ceiling is set by the server launch flag.

**Open:** measure how the utilization split between two co-located models trades one model's throughput against the other's.
