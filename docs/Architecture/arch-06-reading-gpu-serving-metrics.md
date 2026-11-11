---
title: "Reading GPU Serving Metrics"
---

# Reading GPU Serving Metrics

## GPU "util" says a kernel was running, not that the GPU is computing

`nvidia-smi` utilization is the share of the sample period in which at least one kernel was executing. A memory-bound kernel that barely touches the SMs counts as 100%. And the driver may not report it at all: on one 4×H200 server, two GPUs returned `[N/A]` while the other two, same model and driver, reported normally.

What the GPU actually does is in DCGM's profiling fields, which `dcgm-exporter` publishes: `PIPE_TENSOR_ACTIVE` (tensor cores doing math), `GR_ENGINE_ACTIVE` and `DRAM_ACTIVE`. High util with low tensor-active means the GPU is waiting on memory or on kernel launches.

When neither is available, power draw is the honest tie-breaker. An H200 reporting **0% util at 244–334 W** for minutes was working; idle H200s sit at 72–74 W.

## A status built from a gauge and a rate must reconcile their windows

vLLM's `/metrics` mixes two kinds of number. `num_requests_running` and `num_requests_waiting` are gauges: the value at the instant of the scrape. Token throughput is a counter, turned into a rate over the scrape interval.

A request that finished half a second before the scrape reads `0 running · 0 waiting`, yet it still put tokens into the window. A status rule of "idle if nothing is running or waiting" then printed **IDLE next to 1.6k tokens/s**.

Decide which window wins. Here, any tokens in the window count as serving.

The same trap applies to prefix caching: `30.8k in tok/s` with a 94% prefix-cache hit rate is mostly cached prompt, not prefill work.

**Open:** p50/p95 time-to-first-token from Prometheus histogram bucket deltas, and how far a 30 s exporter sample period lags the 1 s NVML numbers.
