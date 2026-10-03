# P07 Research Gap Freeze

## Status
**P07_PASS: True**

## Track status
- A — Short / partial observation: **CLOSED**
- B — Unseen-battery generalization: **CLOSED**
- C — Condition shift / OOD: **CLOSED**
- D — Embedded / MCU: **CLOSED**

## Full research-gap statement
> Prior studies have independently demonstrated partial-window SOH estimation, unseen-cell validation, cross-condition generalization, and resource-constrained embedded implementation. However, the reviewed literature does not establish these elements jointly under a fixed 10-s causal V/I/T observation, strict all-battery leave-one-out evaluation, adaptation-free held-out operating-condition tests quantified against matched unseen-battery references, and measured physical MCU execution.

## Short IEEE contribution statement
> This work addresses the intersection of extreme observation constraint, battery-level generalization, operating-condition shift, and embedded deployment rather than claiming novelty in any one of these elements individually.

## Safe contribution set
- Fixed nominal 10-s causal V/I/T observation after active-discharge detection and 5-s stabilization.
- Strict 34-battery LOBO generalization; RF20x7 macro MAE = 7.096415 pp.
- Adaptation-free held-out condition-family tests with matched unseen-battery references.
- Temperature matched OOD penalty = +6.996525 pp; battery bootstrap CI [4.116840, 9.712289] pp.
- Physical ESP32-C6 deployment: 63.3125 KiB incremental inference-sketch Flash.
- Measured device timing: RF-only 62.302 µs; feature+RF post-acquisition 584.141 µs.
- Numerical parity: 18 vectors × 44 features = 792/792 feature values plus prediction parity.

## Interpretation guardrails
- Do not claim novelty for partial-window SOH by itself.
- Do not claim novelty for unseen-battery validation by itself.
- Do not claim novelty for cross-condition validation by itself.
- Do not claim novelty for MCU/TinyML deployment by itself.
- Do not describe CI including zero as equivalence.
- Do not compare accuracy values across papers as a ranking when targets, splits, information budgets, or metrics differ.
- `63.3125 KiB` is incremental inference-sketch Flash, not total firmware size.
- `0 B` static/global compile delta is not total runtime RAM.
- `584.141 µs` excludes sensing/acquisition/resampling.
- 792/792 parity is software numerical parity on frozen pre-resampled windows, not physical ADC validation.
- The all-data deployed fit is not an unseen-battery generalization estimate.
