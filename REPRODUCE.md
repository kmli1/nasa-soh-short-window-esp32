# Reproduce / Verify

This public release separates **scientific evidence verification** from full raw-data rebuilding.

## 1. Verify the release integrity

Run:

```bash
python scripts/verify_manifest.py
```

The command must report `PASS`.

## 2. Authoritative scientific evidence

Use only:

`results/frozen_core/`

for manuscript headline claims.

## 3. Included scientific notebooks

The `notebooks/` directory contains final scientific notebooks copied from the technical
freeze when they were available in the release source. Release-builder and publication-status
notebooks are intentionally excluded.

## 4. Raw-data rebuild scope

Raw NASA source data are not redistributed in this repository. Rebuilding from the original
raw source requires obtaining the dataset from its provider and reconstructing the same local
input layout used by the study.

The frozen technical package is identified by SHA256:

`19ef95adf99b8055180491b1c77ff25665e1e6d107671685c502bae1374ff9d5`

## 5. Frozen numerical contract

Do not alter the following when reproducing the final deployment path:

- protocol: `ONLINE_FIXED_START_CAUSAL_10S_V1`
- cohort: 2521 cycles / 34 batteries
- features: 44
- model: RF20x7, seed 42
- canonical grid cast: float32 before feature extraction
- target: capacity / 2.0 Ah × 100

A reproduced result should be compared to the frozen tables and hashes under
`results/frozen_core/`, not to historical design-search outputs.
