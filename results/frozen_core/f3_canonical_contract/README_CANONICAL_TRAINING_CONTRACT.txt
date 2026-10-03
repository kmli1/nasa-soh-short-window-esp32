NASA SOH — CANONICAL TRAINING CONTRACT

Protocol:
ONLINE_FIXED_START_CAUSAL_10S_V1

Canonical X:
${PROJECT_ROOT}\deployment\final_online_rf20x7\final_online_training_features.npz
array=X_features
dtype=float32

Canonical y:
${PROJECT_ROOT}\deployment\final_online_rf20x7\final_online_training_manifest.csv
column=soh_rated_pct
dtype=float64

Do NOT use the legacy NPZ float32 y array as the canonical retraining target.

Canonical portable combined artifact:
${PROJECT_ROOT}\results\27_F3_canonical_target_environment_contract\final_online_training_canonical_v2.npz
SHA256:
4472effc044206968c759c3f133d8216efb6dec7abae02074972f15951318962

Final fitted pipeline:
${PROJECT_ROOT}\deployment\final_online_rf20x7\final_online_rf20x7_pipeline.pkl
SHA256:
2ad2b2ca89ab0a5aa09cffe3df663bbbb49d3e198903e3e6939c96f555f295e2

Verified scikit-learn:
1.6.1

Verification:
canonical target tree matches = 20/20
legacy float32 target tree matches = 0/20
canonical LOBO macro MAE = 7.096415349426 pp
max battery MAE diff vs frozen = 2.309e-14 pp
