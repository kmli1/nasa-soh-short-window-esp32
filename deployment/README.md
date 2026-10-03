# ESP32-C6 Deployment

The authoritative public deployment is:

`esp32_c6/canonical_deployment/final_online_rf20x7/`

It corresponds to:

- protocol: `ONLINE_FIXED_START_CAUSAL_10S_V1`
- fit: `FINAL_ONLINE_ALL2521_SEED42_V1`
- RF20x7
- 20 trees
- 3968 total tree nodes
- 2521 training cycles / 34 batteries / 44 features

The previous `arbitrary_single` RF20x7 deployment branch is intentionally excluded from the
final public release to avoid mixing a historical model instance with the frozen online
causal deployment.

## Python pickle

`final_online_rf20x7_pipeline.pkl` is intentionally **not redistributed** in the public
package. Its SHA256 remains recorded in `final_online_artifact_hashes.csv`.

The public release instead provides the canonical training NPZ, manifest, exact feature
order, imputer medians, model parameters/environment evidence, generated C header, and
parity vectors. This avoids requiring users to deserialize an opaque Python pickle while
retaining a hash identity for the original frozen artifact.

## Hardware evidence

`esp32_c6/supporting_hardware_evidence/` contains the final compile/serial and build-identity
evidence used by the publication layer.
