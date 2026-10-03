#include <Arduino.h>

#include "final_online_rf20x7_model.h"
#include "final_online_feature_extractor.h"
#include "final_online_raw_parity_vectors.h"

static constexpr float FEATURE_TOL = 1e-4f;
static constexpr float PRED_TOL = 1e-4f;

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println("FINAL ONLINE RF20x7 - END-TO-END PARITY");

  int vector_pass = 0;
  int feature_pass = 0;

  double feature_abs_sum = 0.0;
  uint64_t feature_count = 0;

  float global_max_feature_diff = 0.0f;
  float global_max_prediction_diff = 0.0f;

  for (int s = 0; s < FINAL_RAW_N; ++s) {
    float feat[44];

    extract44(
      FINAL_RAW[s],
      feat
    );

    int local_pass = 0;
    float local_max_feature_diff = 0.0f;

    for (int j = 0; j < 44; ++j) {
      const float d = fabsf(
        feat[j]
        - FINAL_EXPECTED_FEATURES[s][j]
      );

      feature_abs_sum += d;
      ++feature_count;

      if (d <= FEATURE_TOL) {
        ++local_pass;
        ++feature_pass;
      }

      if (d > local_max_feature_diff) {
        local_max_feature_diff = d;
      }

      if (d > global_max_feature_diff) {
        global_max_feature_diff = d;
      }
    }

    const float pred = rf20x7_predict(feat);

    const float prediction_diff = fabsf(
      pred
      - FINAL_EXPECTED_PRED[s]
    );

    if (
      prediction_diff
      > global_max_prediction_diff
    ) {
      global_max_prediction_diff = prediction_diff;
    }

    const bool pass = (
      local_pass == 44
      && prediction_diff <= PRED_TOL
    );

    if (pass) {
      ++vector_pass;
    }

    Serial.printf(
      "TEST %02d | Features %d/44 | maxFeatDiff %.9g | pred %.9f | expected %.9f | predDiff %.9g | %s\n",
      s,
      local_pass,
      (double)local_max_feature_diff,
      pred,
      FINAL_EXPECTED_PRED[s],
      (double)prediction_diff,
      pass ? "PASS" : "FAIL"
    );
  }

  Serial.println();
  Serial.println("PARITY SUMMARY");

  Serial.printf(
    "Vector pass count: %d/%d\n",
    vector_pass,
    FINAL_RAW_N
  );

  Serial.printf(
    "Feature pass count total: %d/%d\n",
    feature_pass,
    FINAL_RAW_N * 44
  );

  Serial.printf(
    "Mean feature abs diff: %.12g\n",
    feature_count
      ? feature_abs_sum / (double)feature_count
      : 0.0
  );

  Serial.printf(
    "Global max feature abs diff: %.12g\n",
    (double)global_max_feature_diff
  );

  Serial.printf(
    "Global max prediction abs diff pp: %.12g\n",
    (double)global_max_prediction_diff
  );

  Serial.println(
    vector_pass == FINAL_RAW_N
      ? "FINAL ONLINE END-TO-END PARITY PASSED"
      : "FINAL ONLINE END-TO-END PARITY FAILED"
  );

  const int WARMUP = 100;
  const int REPEATS = 1000;

  volatile float sink = 0.0f;

  for (int k = 0; k < WARMUP; ++k) {
    float feat[44];

    extract44(
      FINAL_RAW[0],
      feat
    );

    sink += rf20x7_predict(feat);
  }

  const uint32_t heap_before = ESP.getFreeHeap();

  uint64_t feature_sum = 0;
  uint64_t inference_sum = 0;
  uint64_t total_sum = 0;

  uint32_t feature_min = 0xFFFFFFFFu;
  uint32_t feature_max = 0;

  uint32_t inference_min = 0xFFFFFFFFu;
  uint32_t inference_max = 0;

  uint32_t total_min = 0xFFFFFFFFu;
  uint32_t total_max = 0;

  for (int k = 0; k < REPEATS; ++k) {
    float feat[44];

    const uint32_t t0 = micros();

    extract44(
      FINAL_RAW[0],
      feat
    );

    const uint32_t t1 = micros();

    sink += rf20x7_predict(feat);

    const uint32_t t2 = micros();

    const uint32_t feature_dt = t1 - t0;
    const uint32_t inference_dt = t2 - t1;
    const uint32_t total_dt = t2 - t0;

    feature_sum += feature_dt;
    inference_sum += inference_dt;
    total_sum += total_dt;

    if (feature_dt < feature_min) feature_min = feature_dt;
    if (feature_dt > feature_max) feature_max = feature_dt;

    if (inference_dt < inference_min) inference_min = inference_dt;
    if (inference_dt > inference_max) inference_max = inference_dt;

    if (total_dt < total_min) total_min = total_dt;
    if (total_dt > total_max) total_max = total_dt;
  }

  const uint32_t heap_after = ESP.getFreeHeap();

  Serial.println();
  Serial.println("POST-ACQUISITION COMPUTE BENCHMARK");

  Serial.printf("Repeats: %d\n", REPEATS);

  Serial.printf(
    "Mean feature extraction us: %.6f\n",
    (double)feature_sum / (double)REPEATS
  );

  Serial.printf(
    "Feature extraction min-max us: %u %u\n",
    feature_min,
    feature_max
  );

  Serial.printf(
    "Mean RF inference us: %.6f\n",
    (double)inference_sum / (double)REPEATS
  );

  Serial.printf(
    "RF inference min-max us: %u %u\n",
    inference_min,
    inference_max
  );

  Serial.printf(
    "Mean total post-acquisition us: %.6f\n",
    (double)total_sum / (double)REPEATS
  );

  Serial.printf(
    "Total min-max us: %u %u\n",
    total_min,
    total_max
  );

  Serial.printf("Heap before: %u\n", heap_before);
  Serial.printf("Heap after: %u\n", heap_after);

  Serial.printf(
    "Heap delta: %d\n",
    (int)heap_before - (int)heap_after
  );

  Serial.printf(
    "Minimum free heap: %u\n",
    ESP.getMinFreeHeap()
  );

  Serial.printf(
    "Ignore sink: %.6f\n",
    (double)sink
  );
}

void loop() {}
