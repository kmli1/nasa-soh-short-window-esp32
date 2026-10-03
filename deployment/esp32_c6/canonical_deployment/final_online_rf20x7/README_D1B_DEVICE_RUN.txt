D1B FINAL ONLINE RF20x7 DEVICE RUN
==================================

EXACT FROZEN IDENTITY
---------------------

Protocol:
ONLINE_FIXED_START_CAUSAL_10S_V1

Fit:
FINAL_ONLINE_ALL2521_SEED42_V1

Pipeline SHA256:
2ad2b2ca89ab0a5aa09cffe3df663bbbb49d3e198903e3e6939c96f555f295e2

C header SHA256:
e7720cf63f0056c34dbede64db8b1716552633437edcae2bda3a21a6ab250977

Feature order SHA256:
91981ac596a0a0b0f7354af9db98ee0993924cfed33c3352ecf164f0286a9851

Imputer medians SHA256:
fdb4a1fe61787a19623aee01d43bf8d18241fefa150b1a88c5b80495d03d6f17

ARDUINO SETTINGS
----------------

Keep identical for ALL three projects:

Board:
ESP32C6 Dev Module

CPU:
160 MHz

USB CDC On Boot:
Enabled

USB Mode:
Hardware CDC and JTAG

Arduino ESP32 core:
3.3.5

PSRAM:
None

Keep the SAME partition scheme and build/optimization settings.

STEP 1 - MATCHED BASELINE
-------------------------

Open:
${PROJECT_ROOT}\deployment\final_online_rf20x7\arduino_final_device_tests\01_matched_baseline\01_matched_baseline.ino

Save complete compile result:
${PROJECT_ROOT}\deployment\final_online_rf20x7\device_logs\01_matched_baseline_compile.txt

Upload and save Serial output:
${PROJECT_ROOT}\deployment\final_online_rf20x7\device_logs\01_matched_baseline_serial.txt

Needed:
- sketch bytes
- global-variable bytes

STEP 2 - FINAL FITTED RF20x7 INFERENCE
--------------------------------------

Open:
${PROJECT_ROOT}\deployment\final_online_rf20x7\arduino_final_device_tests\02_final_model_inference\02_final_model_inference.ino

Save complete compile result:
${PROJECT_ROOT}\deployment\final_online_rf20x7\device_logs\02_final_model_inference_compile.txt

Upload and save Serial output:
${PROJECT_ROOT}\deployment\final_online_rf20x7\device_logs\02_final_model_inference_serial.txt

Required:
- SANITY PASS
- 1000 repeats
- mean/min/max inference
- heap before/after
- minimum free heap

STEP 3 - FINAL REAL-ONLINE END-TO-END PARITY
--------------------------------------------

Open:
${PROJECT_ROOT}\deployment\final_online_rf20x7\arduino_final_device_tests\03_final_online_end_to_end_parity\03_final_online_end_to_end_parity.ino

Save complete compile result:
${PROJECT_ROOT}\deployment\final_online_rf20x7\device_logs\03_final_online_parity_compile.txt

Upload and save Serial output:
${PROJECT_ROOT}\deployment\final_online_rf20x7\device_logs\03_final_online_parity_serial.txt

Required:
- 18/18 vector PASS
- 792/792 feature PASS
- max feature diff
- max prediction diff
- feature extraction mean/min/max
- RF inference mean/min/max
- total post-acquisition mean/min/max
- heap measurements

MATCHED SIZE EQUATIONS
----------------------

Compiled inference-only RF Flash overhead:
model_sketch_bytes - baseline_sketch_bytes

Additional statically allocated global RAM:
model_global_bytes - baseline_global_bytes

Do NOT call the latter "total RAM cost".

D1 is closed only when all device evidence above is bound
to the exact model/header hashes in this file.
