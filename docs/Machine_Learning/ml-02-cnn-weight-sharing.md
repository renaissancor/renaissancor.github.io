---
title: "CNN: One Filter, Reused Everywhere"
tags:
  - fundamentals
  - machine-learning
---

# CNN: One Filter, Reused Everywhere

**CNN = Convolutional Neural Network.** A neural network whose main layers are *convolutions*: one small grid of weights (a filter) slid over the whole input, instead of a separate weight for every pixel.

| abbreviation | stands for |
|---|---|
| CNN | Convolutional Neural Network |
| MLP | Multi-Layer Perceptron: every layer fully connected |
| FC | Fully Connected layer: every input linked to every output by its own weight |
| MAC | Multiply-ACcumulate: one `a·b + sum` step, the unit of work counted below |
| LeNet-5 | the classic 1998 CNN for handwritten digits (MNIST, 0–9), used for all numbers here |

## Key questions

1. What does one convolution step compute, and what stays the same as the filter moves?
2. How does the shape change layer by layer, and why does a deeper layer see more of the image?
3. Why does weight sharing cut parameters, and why does it make conv compute-bound?

---

## Figures

### One filter, sliding (5×5 input, 3×3 filter, stride 1, no padding)

```text
        input X (5×5)            filter K (3×3)      output Y (3×3)
+---+---+---+---+---+       +---+---+---+       +---+---+---+
| 0*| 0*| 9*| 9 | 9 |       |-1 | 0 | 1 |       |27 |27 | 0 |
+---+---+---+---+---+       +---+---+---+       +---+---+---+
| 0*| 0*| 9*| 9 | 9 |   *   |-1 | 0 | 1 |   =   |27 |27 | 0 |
+---+---+---+---+---+       +---+---+---+       +---+---+---+
| 0*| 0*| 9*| 9 | 9 |       |-1 | 0 | 1 |       |27 |27 | 0 |
+---+---+---+---+---+       +---+---+---+       +---+---+---+
| 0 | 0 | 9 | 9 | 9 |
+---+---+---+---+---+          * = window for Y[0][0]
| 0 | 0 | 9 | 9 | 9 |
+---+---+---+---+---+

Y[0][0] = (-1·0 + 0·0 + 1·9) × 3 rows = 27      <- edge inside the window
Y[0][2] = (-1·9 + 0·9 + 1·9) × 3 rows =  0      <- flat region, no edge
```

- The window moves one cell right, then wraps to the next row: 3 × 3 = 9 positions.
- **The same 9 weights are used at all 9 positions.** That is weight sharing.
- High output means "the pattern in K is here". This K is a vertical-edge detector.

### Shape through LeNet-5 (one 28×28 digit)

```text
  C × H × W
  1 × 28 × 28     input digit
      │  conv 5×5, 6 filters, pad 2
      ▼
  6 × 28 × 28     6 feature maps
      │  max-pool 2×2
      ▼
  6 × 14 × 14     shrinks
      │  conv 5×5, 16 filters (each spans 6 channels)
      ▼
 16 × 10 × 10
      │  max-pool 2×2
      ▼
 16 ×  5 ×  5     shrinks
      │  flatten
      ▼
 400 ─FC─▶ 120 ─FC─▶ 84 ─FC─▶ 10    one score per digit
```

- Channels go **up** (1 → 6 → 16): more kinds of pattern.
- Width goes **down** (28 → 14 → 10 → 5): coarser, wider view.

### Receptive field: how many input pixels one unit sees

```text
 after   conv1   pool1   conv2   pool2
 field    5×5     6×6    14×14   16×16   (of a 28×28 image)
```

---

## Formulas

| formula | symbols |
|---|---|
| `W_out = (W − K + 2P) / S + 1` | `W` input width, `K` kernel, `P` padding, `S` stride |
| `params = C_out × (K·K·C_in + 1)` | `C_in` input channels, `C_out` filters, `+1` bias |
| `MACs = W_out·H_out · C_out · K·K·C_in` | multiply-adds for one image |
| `r_out = r_in + (K − 1)·j`, then `j ← j·S` | receptive field `r`, jump `j` starts at 1 |

Applied to LeNet-5:

| layer | `W_out` | params | MACs |
|---|---|---:|---:|
| conv1 | (28 − 5 + 4)/1 + 1 = **28** | 6 × (25·1 + 1) = **156** | 28·28 · 6 · 25 = **117,600** |
| pool1 | (28 − 2)/2 + 1 = **14** | 0 | — |
| conv2 | (14 − 5)/1 + 1 = **10** | 16 × (25·6 + 1) = **2,416** | 10·10 · 16 · 150 = **240,000** |
| pool2 | (10 − 2)/2 + 1 = **5** | 0 | — |
| 3 FC layers | — | **59,134** | **58,920** |

---

## Answers

**1. One step** is a dot product of the 3×3 window with K (Figure 1, `Y[0][0] = 27`). What stays the same is **K itself**: the same weights at every position.

**2. Shape** follows `W_out = (W − K + 2P)/S + 1`: conv keeps or trims the width, pool halves it. A deeper unit sees more because each pool step doubles the jump `j`, so the next 5×5 filter spans 5 cells that each stand for 2×2 or 4×4 input pixels (receptive field 5 → 16).

**3. Weight sharing:** an MLP gives every hidden unit its own weight for every pixel (784 × 300 = 235,200 for the first layer). A conv layer has `C_out × K·K·C_in` weights no matter how big the image is. LeNet-5 totals **61,706** parameters against **266,610** for an MLP 784→300→100→10.
The same table shows the hardware consequence: the conv layers hold **4%** of the parameters and do **86%** of the MACs. Each conv weight is reused ~750× (conv1) or ~100× (conv2) per image, so intensity is high and conv is **compute-bound**. Each FC weight is used **once** per image, so at batch 1 an FC layer is **bandwidth-bound**, the same shape as LLM decode on the [roofline](../Architecture/arch-02-roofline-model.md).

---

## Gotchas

| plausible model | what actually happens |
|---|---|
| A CNN zooms into regions | It **slides** one filter over every position, and **shrinks** the map with pooling |
| Deeper layers see more because they are bigger | They are **smaller**; they see more because each cell stands for a wider input patch |
| Most compute is where most parameters are | The opposite in LeNet: FC has 96% of params, conv has 86% of MACs |
| "Convolution" flips the kernel | Deep-learning conv is cross-correlation, no flip. `F.conv2d` gave `+27` above with K as written |

---

<details>
<summary>Check (PyTorch 2.8)</summary>

```python
import torch, torch.nn.functional as F
X = torch.tensor([[0, 0, 9, 9, 9]] * 5, dtype=torch.float32)
K = torch.tensor([[-1, 0, 1]] * 3, dtype=torch.float32)
print(F.conv2d(X[None, None], K[None, None])[0, 0])
# tensor([[27., 27., 0.], [27., 27., 0.], [27., 27., 0.]])

out = lambda W, K, P, S: (W - K + 2 * P) // S + 1
print(out(28, 5, 2, 1), out(28, 2, 0, 2), out(14, 5, 0, 1), out(10, 2, 0, 2))
# 28 14 10 5
```

</details>
