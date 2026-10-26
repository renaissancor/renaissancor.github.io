---
title: "CNN: Slide One Filter Everywhere, Then Shrink"
tags:
  - fundamentals
  - machine-learning
---

# CNN: Slide One Filter Everywhere, Then Shrink

What I recalled first: "a CNN zooms into regions of the image to interpret them." Half right. **Local regions** is correct. **Zooming in** is backwards.

## Slide, don't zoom

A small filter (say 3×3) slides across the image one step at a time, and **the same filter is used at every position**. A vertical-edge filter finds vertical edges top-left and bottom-right alike. A cat is a cat wherever it sits in the photo.

## Depth climbs meaning

Early layers respond to edges and corners. Middle layers combine those into parts: eyes, ears. Late layers respond to whole objects.

## Pooling shrinks

Each stage downsizes the feature map. One filter in a late layer then covers a wider patch of the original image (its receptive field grows). The network sees wider by shrinking, not by zooming.

## Why weight sharing matters: 784 → 9

MNIST: handwritten digits 0–9, 28×28 pixels.

| | weights |
|---|---:|
| one fully connected neuron looking at the whole image | 28 × 28 = **784** |
| one 3×3 filter reused at every position | **9** |

Fewer weights means faster training and less overfitting on small data. The single assumption "the same pattern means the same thing anywhere" buys 784 → 9.

## Recall before re-reading

1. Does a CNN zoom in or slide? What is the same at every position?
2. Why does sharing one filter across the image cut the weight count, and what does that buy?
3. What does pooling do to how much of the original image a late-layer filter sees?
