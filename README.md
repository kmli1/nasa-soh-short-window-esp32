# NASA SOH Short-Window ESP32

Resource-Aware Short-Window State-of-Health Estimation for Lithium-Ion Batteries

This repository is the public research artifact for a lithium-ion battery SOH study combining:

- a **fixed nominal 10-s causal V/I/T observation**,
- **strict 34-battery leave-one-battery-out (LOBO)** evaluation,
- adaptation-free operating-condition stress tests with matched references,
- and measured **ESP32-C6** deployment.

## Frozen headline results

| Item | Frozen result |
|---|---:|
| Final cohort | 34 batteries / 2521 cycles |
| Feature count | 44 |
| Strict LOBO macro MAE | 7.096415 pp |
| Temperature OOD matched penalty | +6.996525 pp |
| Incremental inference-sketch Flash | 63.3125 KiB |
| RF-only device latency | 62.302 µs |
| Feature + RF post-acquisition latency | 584.141 µs |
| Device parity | 18 vectors / 792 feature values |

## Important interpretation boundaries

- The nominal observation interval is 10 s, but sparse raw sampling causes later actual
  decision times; timing diagnostics are preserved in `results/frozen_core/`.
- `63.3125 KiB` is **incremental inference-sketch Flash**, not total firmware size.
- `0 B` additional static/global allocation is **not total runtime RAM**.
- `584.141 µs` excludes sensing, acquisition, and resampling.
- 792/792 parity is software numerical parity on frozen pre-resampled windows, **not**
  physical ADC validation.
- The all-data deployed fit is not an unseen-battery generalization estimate.
- Confidence intervals containing zero are not interpreted as equivalence.

## Evidence authority

The authoritative manuscript evidence is:

`results/frozen_core/`

Historical design-search and legacy artifacts were intentionally excluded from the public
repository to avoid stale-result ambiguity. The P09 technical freeze remains identified by:

`19ef95adf99b8055180491b1c77ff25665e1e6d107671685c502bae1374ff9d5`

## Literature

- `literature/P07_CORE_LITERATURE_MATRIX.csv` — frozen core comparator matrix.
- `literature/P07_SCREENED_STUDY_REGISTRY.csv` — saturation-review coverage registry.


The literature matrices under `literature/` and `results/frozen_core/p07_literature/` preserve the P07 screening snapshot dated 3 October 2026. The final manuscript includes additional references identified during subsequent review. For the final literature comparison, consult the manuscript and its cited original publications.


## Final manuscript figures

Only the P08-synchronized manuscript figures are exposed in the public root figure set:

`figures/manuscript_v3/`

This directory contains exactly five figures in PNG and PDF form. Historical design-search,
supplementary, 60-s example, and legacy figures are intentionally excluded from the public
root figure set. Their presence in the immutable frozen evidence, if any, does not make them
authoritative manuscript figures.


## Canonical deployment

Use only:

`deployment/esp32_c6/canonical_deployment/final_online_rf20x7/`

for the final deployed model. Historical `arbitrary_single` deployment artifacts are not
part of this public release.

## Historical gate snapshot note

Files under `results/frozen_core/step35_claim_registry/` are an immutable historical
technical-freeze snapshot created before P07–P10 were closed. Their historical `OPEN`
publication-gate entries must not be interpreted as the current release status. The current
status is `release/FINAL_PUBLIC_RELEASE_STATUS.json`.

## Reproducibility

See `REPRODUCE.md`.

## Citation

See `CITATION.cff`.

## License

- Code: **MIT**
- Original documentation/evidence: **CC BY 4.0**
- Third-party materials retain their original terms.

Repository: https://github.com/kmli1/nasa-soh-short-window-esp32
