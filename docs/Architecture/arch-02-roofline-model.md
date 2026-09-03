---
title: "Roofline Model"
---

# Roofline Model

One correction from a self-test: I had the right instinct that LLM decode is bandwidth-bound, but named the wrong bandwidth and the wrong axes.

## Axes and ceilings

- **x axis: arithmetic intensity**, FLOPs per byte moved from memory.
- **y axis: achieved FLOP/s.**
- Two ceilings. A flat line at peak compute. A sloped line at peak bandwidth, since FLOP/s ≤ bandwidth × intensity. The roof is the min of the two.

Where a kernel's intensity lands tells you which ceiling it hits, before you measure.

## GEMM versus decode

- **Large GEMM:** intensity in the hundreds. Under the flat roof. Compute-bound.
- **LLM decode at batch 1:** every weight is read once per token and used for one multiply-add. Intensity about 1 FLOP per byte. Far left, on the slope. Bandwidth-bound. A 70B model in FP8 means reading 70 GB per generated token.

## The bandwidth that matters

Not PCIe, not "loading data into VRAM". Weights are already resident. The limit is **HBM to the compute units**, on-device, because every token re-reads all of them.

## Why batching works

32 requests share one read of the weights. Intensity goes up 32×. The kernel slides right along the roof toward compute-bound. That is the entire economic argument for continuous batching in inference servers.

**Open:** compute the actual intensity for one decode step of a served model and place it on an H200 roofline (4.8 TB/s HBM, ~2 PFLOP/s FP8). At what batch size does it cross to compute-bound, and how much do KV-cache reads shift it at long context?
