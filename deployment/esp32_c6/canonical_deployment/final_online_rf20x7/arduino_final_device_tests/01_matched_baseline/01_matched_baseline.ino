#include <Arduino.h>

static volatile float INPUT_X[44];

__attribute__((noinline))
float baseline_predict(
    const volatile float* x) {
  return x[0];
}

void setup() {
  Serial.begin(115200);
  delay(1500);

  for (int i = 0; i < 44; ++i) {
    INPUT_X[i] = 0.001f * (float)(i + 1);
  }

  volatile float sink = baseline_predict(INPUT_X);

  Serial.println("FINAL ONLINE RF20x7 - MATCHED BASELINE");
  Serial.printf("Warm result: %.9f\n", (double)sink);
  Serial.printf("Heap total: %u\n", ESP.getHeapSize());
  Serial.printf("Free heap: %u\n", ESP.getFreeHeap());
  Serial.printf("Minimum free heap: %u\n", ESP.getMinFreeHeap());
  Serial.printf("Maximum alloc heap: %u\n", ESP.getMaxAllocHeap());
}

void loop() {}
