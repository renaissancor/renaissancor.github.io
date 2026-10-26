---
title: "The Learning Chain: Loss, Gradient Descent, Activation, Backprop"
tags:
  - fundamentals
  - machine-learning
---

# The Learning Chain: Loss, Gradient Descent, Activation, Backprop

The basic formulas do not stick as a list. They stick as a chain where each one exists because the one before it left a problem.

## 1. Model: `y = wx + b`

**Problem:** map an input to an answer.
**Fix:** a function with knobs, `w` and `b`. Training only ever changes the knobs.

## 2. Loss: `L = (y_pred − y)²`

**Problem:** to fix anything, "how wrong" has to be one number.
**Fix:** mean squared error. Squaring removes the sign and punishes large misses harder.

## 3. Gradient descent: `w ← w − η · ∂L/∂w`

**Problem:** which way to turn each knob.
**Fix:** the gradient points uphill in loss, so step the other way. `η` is the step size: too big overshoots, too small crawls.

## 4. Activation: `ReLU(x) = max(0, x)`

**Problem:** stacked linear layers collapse into one linear layer. A line of a line is a line.
**Fix:** a bend between layers (ReLU, sigmoid). Without it, depth means nothing.

## 5. Backprop: `∂L/∂w = ∂L/∂y · ∂y/∂w`

**Problem:** the gradient of a weight far from the output.
**Fix:** the chain rule, applied backwards. Multiply local derivatives from the output inward, reusing the values saved in the forward pass.

## 6. Classification: softmax + cross-entropy, `L = −log(p_correct)`

**Problem:** picking one of N classes (MNIST digits 0–9), where squared error fits badly.
**Fix:** softmax turns N scores into probabilities. The loss is 0 when the correct class gets probability 1 and grows without bound as it nears 0.

## The one line

**Measure the error (loss) → step against it (gradient descent) → bend so depth means something (activation) → pass the correction back through depth (backprop).**

## Recall before re-reading

Answer these without scrolling up. Re-read only where you get stuck.

1. Why does a deep network need an activation function at all?
2. In `w ← w − η · ∂L/∂w`, why the minus sign, and what goes wrong when `η` is too large?
3. What does backprop reuse from the forward pass?
