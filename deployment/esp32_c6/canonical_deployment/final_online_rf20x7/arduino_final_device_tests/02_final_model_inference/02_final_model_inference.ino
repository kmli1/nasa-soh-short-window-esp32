#include <Arduino.h>
#include "final_online_rf20x7_model.h"

static volatile float INPUT_X[44] = {
  3.86057901f, 0.00369303883f, 3.85473967f, 3.86641812f, 0.0116784573f, 3.86057901f, 3.86641812f, 3.85473967f, -0.0116784573f, -0.00116784137f, 0.00116784568f, 0.00116801262f, -1.99595892f, 0.000130244138f, -1.9961648f, -1.99575293f, 0.000411868095f, -1.99595892f, -1.99575293f, -1.9961648f, -0.000411868095f, -4.11868095e-05f, 4.11868095e-05f, 4.12464142e-05f, 24.7682838f, 0.0237559266f, 24.7307224f, 24.8058453f, 0.0751228333f, 24.7682838f, 24.7307224f, 24.8058453f, 0.0751228333f, 0.00751228351f, 0.00751228351f, 0.00751304626f, 1.99595892f, 1.99595892f, 1.0f, 7.70555639f, 0.00686833588f, 7.71641541f, 1.0f, 0.0751228333f
};

__attribute__((noinline))
float call_model(
    const volatile float* source) {

  float x[44];

  for (int i = 0; i < 44; ++i) {
    x[i] = source[i];
  }

  return rf20x7_predict(x);
}

void setup() {
  Serial.begin(115200);
  delay(1500);

  const float expected = 82.5912018f;

  Serial.println("FINAL ONLINE RF20x7 - INFERENCE BENCHMARK");

  const float pred = call_model(INPUT_X);
  const float diff = fabsf(pred - expected);

  Serial.printf("Expected prediction: %.9f\n", expected);
  Serial.printf("ESP32 prediction: %.9f\n", pred);
  Serial.printf("Absolute difference pp: %.9f\n", diff);

  Serial.println(
    diff <= 1e-4f
      ? "SANITY PASS"
      : "SANITY FAIL"
  );

  const int WARMUP = 100;
  const int REPEATS = 1000;

  volatile float sink = 0.0f;

  for (int k = 0; k < WARMUP; ++k) {
    sink += call_model(INPUT_X);
  }

  const uint32_t heap_before = ESP.getFreeHeap();

  uint64_t total_us = 0;
  uint32_t min_us = 0xFFFFFFFFu;
  uint32_t max_us = 0;

  for (int k = 0; k < REPEATS; ++k) {
    const uint32_t t0 = micros();
    sink += call_model(INPUT_X);
    const uint32_t dt = micros() - t0;

    total_us += dt;

    if (dt < min_us) min_us = dt;
    if (dt > max_us) max_us = dt;
  }

  const uint32_t heap_after = ESP.getFreeHeap();

  Serial.printf("Repeats: %d\n", REPEATS);

  Serial.printf(
    "Mean inference us: %.6f\n",
    (double)total_us / (double)REPEATS
  );

  Serial.printf("Min inference us: %u\n", min_us);
  Serial.printf("Max inference us: %u\n", max_us);

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
