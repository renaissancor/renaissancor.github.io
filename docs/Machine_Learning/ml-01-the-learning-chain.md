---
title: "The Learning Chain: Loss, Gradient Descent, Activation, Backprop"
tags:
  - fundamentals
  - machine-learning
---

# The Learning Chain: Loss, Gradient Descent, Activation, Backprop

The basic formulas do not stick as a list. They stick as a chain where each one exists because the one before it left a problem.

## 1. Model

$$
\hat{y} = w x + b
$$

**Problem:** map an input $x$ to an answer.
**Fix:** a function with knobs, $w$ and $b$. Training only ever changes the knobs.

## 2. Loss

$$
L = \frac{1}{N} \sum_{i=1}^{N} \left( \hat{y}_i - y_i \right)^2
$$

**Problem:** to fix anything, "how wrong" has to be one number.
**Fix:** mean squared error. Squaring removes the sign and punishes large misses harder.

## 3. Gradient descent

$$
w \leftarrow w - \eta \, \frac{\partial L}{\partial w}
$$

**Problem:** which way to turn each knob.
**Fix:** the gradient points uphill in loss, so step the other way. $\eta$ is the step size (learning rate): too big overshoots, too small crawls.

## 4. Activation

$$
\mathrm{ReLU}(x) = \max(0, x) \qquad \sigma(x) = \frac{1}{1 + e^{-x}}
$$

**Problem:** stacked linear layers collapse into one linear layer, since $W_2 (W_1 x) = (W_2 W_1) x$. A line of a line is a line.
**Fix:** a bend between layers (ReLU, sigmoid $\sigma$). Without it, depth means nothing.

## 5. Backprop

$$
\frac{\partial L}{\partial w} = \frac{\partial L}{\partial \hat{y}} \cdot \frac{\partial \hat{y}}{\partial w}
$$

**Problem:** the gradient of a weight far from the output.
**Fix:** the chain rule, applied backwards. Multiply local derivatives from the output inward, reusing the values saved in the forward pass.

## 6. Classification: softmax + cross-entropy

$$
p_k = \frac{e^{z_k}}{\sum_{j} e^{z_j}} \qquad L = -\log p_{\text{correct}}
$$

**Problem:** picking one of $N$ classes (MNIST digits 0–9), where squared error fits badly.
**Fix:** softmax turns $N$ scores $z_k$ into probabilities $p_k$. The loss is 0 when the correct class gets $p = 1$ and grows without bound as $p \to 0$.

## The one line

**Measure the error (loss) → step against it (gradient descent) → bend so depth means something (activation) → pass the correction back through depth (backprop).**

## Recall before re-reading

Answer these without scrolling up. Re-read only where you get stuck.

1. Why does a deep network need an activation function at all?
2. In $w \leftarrow w - \eta \, \partial L / \partial w$, why the minus sign, and what goes wrong when $\eta$ is too large?
3. What does backprop reuse from the forward pass?
