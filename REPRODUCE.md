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


## 6. Arduino dependency preparation

The final device-test sketches are located under:

`deployment/esp32_c6/canonical_deployment/final_online_rf20x7/arduino_final_device_tests/`

The deployed model header, `final_online_rf20x7_model.h`, is provided in
the `02_final_model_inference` folder. The third sketch also requires
this same header.

Before running the device tests:

1. Use a device-test workspace outside the public repository directory.
   Copy the `01_matched_baseline`, `02_final_model_inference`, and
   `03_final_online_end_to_end_parity` folders into that workspace.
2. Copy `final_online_rf20x7_model.h` from the prepared
   `02_final_model_inference` folder into the prepared
   `03_final_online_end_to_end_parity` folder without modifying its contents.
3. Open each `.ino` file from its corresponding sketch folder.

The prepared third sketch folder must contain:

- `03_final_online_end_to_end_parity.ino`
- `final_online_rf20x7_model.h`
- `final_online_feature_extractor.h`
- `final_online_raw_parity_vectors.h`

This preparation uses the existing deployed model, feature extractor,
and test vectors. No model retraining or regeneration of test inputs is required.

Use Arduino-ESP32 core 3.3.5, the ESP32C6 Dev Module board profile,
and a 160 MHz CPU. The recorded setup uses USB CDC enabled,
Hardware CDC/JTAG, and no PSRAM. Paths in the historical
`README_D1B_DEVICE_RUN.txt` refer to the original local project layout;
the public repository path is given above.

The third sketch checks numerical consistency using 18 preserved
V/I/T windows and 792 feature comparisons. The absolute validation
tolerances are 1e-4 in each feature's native units and 1e-4 percentage
points for predictions.

Its runtime benchmark covers feature extraction and RF inference
from previously resampled inputs. Sensor acquisition, ADC processing,
online buffering, and resampling are outside the measured scope.

Keep the prepared workspace outside the public repository because
`scripts/verify_manifest.py` checks the exact release file set.
Use `release/FINAL_RELEASE_MANIFEST_SHA256.csv` to verify the current
public repository files.
