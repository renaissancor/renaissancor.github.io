---
title: "Glossary: ML and ML Systems"
tags:
  - glossary
  - machine-learning
  - llm-serving
---

# Glossary: ML and ML Systems

The words you must recognize instantly, not the theory behind them.

### Training

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| Parameter / weight | | numbers the training learns | "a 31B model" means roughly 31 billion parameters |
| Hyperparameter | | settings you choose: learning rate, depth | |
| Loss | | the number training minimizes | "the loss stopped dropping" |
| Cross-entropy | | the standard classification loss | |
| Gradient | | direction and size of the loss increase per parameter | |
| Backprop | Backpropagation | chain rule from the loss back to each parameter | |
| SGD | Stochastic Gradient Descent | update from a small sample of data | |
| Adam | Adaptive Moment Estimation | SGD with momentum and per-parameter step sizes | |
| LR | Learning Rate | step size | "lower the LR" |
| Epoch / batch | | one pass over the data / one sample group | |
| Overfitting | | good on train data, bad on new data | |
| Regularization | | penalties or noise against overfitting | |
| Fine-tuning | | continue training a trained model on your data | |

### Model parts

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| Logit | | raw score before softmax | "compare the logits" |
| Softmax | | turns logits into probabilities summing to 1 | |
| Logprob | log probability | log of a token's probability | "logprobs are read after the mask" |
| Activation | | non-linearity between layers | |
| ReLU | Rectified Linear Unit | max(0, x) | |
| Embedding | | a vector representing a token or item | |
| MLP | Multi-Layer Perceptron | stacked fully connected layers | |
| CNN | Convolutional Neural Network | shared local filters, mostly for images | |
| RNN / LSTM | Recurrent Neural Network / Long Short-Term Memory | carries state through a sequence | |
| Attention | | each token weighs the tokens it can see | |
| Transformer | | attention-based architecture behind LLMs | |
| ViT | Vision Transformer | image patches treated as tokens | |
| MoE | Mixture of Experts | only some sub-networks are active per token | |

### LLM usage

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| LLM / VLM | Large Language / Vision-Language Model | text / text and image model | |
| Token | | the unit a model reads and emits, not a word | "that's over the context limit in tokens" |
| BOS / EOS | Beginning / End Of Sequence (or Sentence) | special marker tokens | "it never emitted EOS" |
| Context window | | input plus output token limit | |
| Temperature | | flattens or sharpens the sampling distribution | |
| Top-k / top-p | | sample from k best / from the smallest set reaching probability p | |
| Constrained decoding | | server masks tokens to enforce a grammar or schema | |
| RAG | Retrieval-Augmented Generation | fetch documents, then answer from them | |

### Classifier metrics

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| Precision | | of flagged items, how many were right | |
| Recall | | of real positives, how many were caught | "recall dropped on the new set" |
| F1 | | harmonic mean of the two | |
| FPR / FNR | False Positive / Negative Rate | of real negatives, how many were flagged / of real positives, how many were missed | |
| ROC / AUC | Receiver Operating Characteristic / Area Under the Curve | threshold-free ranking quality; can flatter imbalanced data | |

### Serving

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| Inference | | running a trained model | |
| Prefill | | process the whole prompt once. Usually compute-bound | "prefill blows up with image tokens" |
| Decode | | generate one token at a time. Memory-bandwidth-bound | |
| KV cache | Key-Value cache | stored attention keys and values for tokens already seen | "KV cache usage hit 90%" |
| TTFT | Time To First Token | wait until output starts | |
| TPOT | Time Per Output Token | gap between tokens after the first. Often called ITL (inter-token latency), but tools define them differently | |
| Continuous batching | | requests join and leave the batch at each step | |
| Preemption | | the server pauses a request and frees its KV cache when space runs out, then recomputes it later | |
| TP | Tensor Parallelism | each layer's weights split across GPUs | "TP=4" |
| vLLM | | popular open-source LLM serving engine | |
| NCCL | NVIDIA Collective Communications Library | GPU all-reduce and friends | |
| CUDA | originally Compute Unified Device Architecture, now just the name | NVIDIA's GPU programming platform | |
| MIG | Multi-Instance GPU | one physical GPU split into isolated slices | |

### Numbers and formats

| term | stands for | meaning | you'll hear |
|---|---|---|---|
| Quantization | | store numbers in fewer bits, or lower precision | "the FP8 quant" |
| FP32 / FP16 / BF16 / FP8 | 32-bit, 16-bit, bfloat16, 8-bit float | precision formats | |
| INT8 / INT4 | 8-bit / 4-bit integer | integer quantization | |
| FLOP / FLOPS | Floating-Point Operation(s) / per Second | a count / a rate. Easy to mix up | |
| GEMM | General Matrix Multiply | the core kernel of deep learning | |
| Distillation | | train a small model to mimic a big one | |
| Safetensors | | safe, memory-mappable weight file format | |
