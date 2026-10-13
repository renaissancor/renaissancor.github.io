---
title: "Constrained Decoding in LLM Serving"
---

# Constrained Decoding in LLM Serving

A served model never "follows" a JSON schema. The server enforces it: at each decode step a grammar engine (xgrammar by default in vLLM) marks which tokens keep the output valid, every other logit is set to −inf, and only then does the sampler pick. Three consequences are not obvious from the docs.

## A schema parameter the server doesn't know is dropped, not rejected

vLLM's OpenAI-compatible request models accept extra fields and only log a warning that they were ignored. `guided_json` was deprecated in v0.11.0 and removed in v0.12.0 (replaced by `structured_outputs: {"json": ...}`), so on any newer server it is silently a no-op. Behind a gateway, the caller never sees the warning.

The symptom is output the grammar would have made impossible. On a classification workload sending `guided_json` to a v0.2x server, 20 of 20 answers came wrapped in a ```` ```json ```` fence. A few broke the JSON outright: one looped inside a string until `max_tokens`, another wrote a second "corrected" object after the first. The OpenAI-standard `response_format: {"type": "json_schema", ...}` is still routed to the same backend and returned bare single objects every time.

**Check:** send a request whose output the schema forbids from starting with a fence. Any fence means the constraint isn't applied.

## The constraint changes answers even where it masks nothing

At a digit position where the schema allows 0–9, the mask removes nothing. Yet re-running the same 20 images with the constraint actually applied flipped 3 of 140 digit scores and moved per-digit probabilities by up to 0.29. Every flip was at a near-coin-flip position (P 0.46–0.65).

The cause is the prefix. Without a fence, the model conditions on a different preceding text. Final labels were unchanged here, but anything read from the probabilities shifts.

## Logprobs are reported after the mask

In vLLM's V1 engine the grammar bitmask is applied to the logits before sampling, and the default `raw_logprobs` mode computes log-softmax from those masked logits. So a returned logprob is a probability over the **allowed** tokens. A forced token reads ≈ 0 regardless of what the model believed.

Two rules follow if you use token probabilities as a confidence signal:

- **Don't narrow a range in the grammar that you want to read probabilities over.** Bounding an integer to 0–3 deletes the mass on 4–9. Validate the range in your own code instead.
- **A multi-token value is decided over several steps.** Some tokenizers split numbers digit by digit (Gemma writes `"15"` as `1`, `5`). P("15") is then P(`1`) × P(`5` | `1`), and the first token's probability is shared with 12 and 18. Keep the values you score single-token.

**Open:** measure the post-mask behaviour directly. Bound an axis below the value the model wants and compare the returned logprobs with the unconstrained run.
