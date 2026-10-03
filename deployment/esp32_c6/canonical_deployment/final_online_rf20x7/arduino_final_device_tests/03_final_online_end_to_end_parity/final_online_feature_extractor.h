#pragma once

#include <Arduino.h>
#include <math.h>

static inline void sort11(const float* x, double* s) {
  for (int k = 0; k < 11; ++k) {
    s[k] = (double)x[k];
  }

  for (int k = 1; k < 11; ++k) {
    const double key = s[k];
    int j = k - 1;

    while (j >= 0 && s[j] > key) {
      s[j + 1] = s[j];
      --j;
    }

    s[j + 1] = key;
  }
}

static inline void channel12(
    const float* x,
    float* out) {

  double sum = 0.0;
  double sumsq = 0.0;
  double mn = (double)x[0];
  double mx = (double)x[0];

  for (int k = 0; k < 11; ++k) {
    const double z = (double)x[k];

    sum += z;
    sumsq += z * z;

    if (z < mn) mn = z;
    if (z > mx) mx = z;
  }

  const double mean = sum / 11.0;

  double variance = (
    sumsq / 11.0
    - mean * mean
  );

  if (variance < 0.0 && variance > -1e-12) {
    variance = 0.0;
  }

  double sorted[11];
  sort11(x, sorted);

  const double sum_t = 55.0;
  const double sum_tt = 385.0;

  double sum_tx = 0.0;

  for (int k = 0; k < 11; ++k) {
    sum_tx += (
      (double)k
      * (double)x[k]
    );
  }

  const double denominator = (
    11.0 * sum_tt
    - sum_t * sum_t
  );

  const double slope = (
    11.0 * sum_tx
    - sum_t * sum
  ) / denominator;

  double sum_abs_diff = 0.0;
  double max_abs_diff = 0.0;

  for (int k = 1; k < 11; ++k) {
    const double d = fabs(
      (double)x[k]
      - (double)x[k - 1]
    );

    sum_abs_diff += d;

    if (d > max_abs_diff) {
      max_abs_diff = d;
    }
  }

  out[0] = (float)mean;
  out[1] = (float)sqrt(variance);
  out[2] = (float)mn;
  out[3] = (float)mx;
  out[4] = (float)(mx - mn);
  out[5] = (float)sorted[5];
  out[6] = x[0];
  out[7] = x[10];
  out[8] = x[10] - x[0];
  out[9] = (float)slope;
  out[10] = (float)(sum_abs_diff / 10.0);
  out[11] = (float)max_abs_diff;
}

static inline float corr11(
    const float* a,
    const float* b) {

  double ma = 0.0;
  double mb = 0.0;

  for (int k = 0; k < 11; ++k) {
    ma += (double)a[k];
    mb += (double)b[k];
  }

  ma /= 11.0;
  mb /= 11.0;

  double va = 0.0;
  double vb = 0.0;
  double cov = 0.0;

  for (int k = 0; k < 11; ++k) {
    const double da = (double)a[k] - ma;
    const double db = (double)b[k] - mb;

    va += da * da;
    vb += db * db;
    cov += da * db;
  }

  if (va < 1e-24 || vb < 1e-24) {
    return 0.0f;
  }

  const double c = cov / sqrt(va * vb);

  if (!isfinite(c)) {
    return 0.0f;
  }

  return (float)c;
}

static inline void extract44(
    const float raw[11][3],
    float out[44]) {

  float v[11];
  float i[11];
  float temp[11];

  for (int k = 0; k < 11; ++k) {
    v[k] = raw[k][0];
    i[k] = raw[k][1];
    temp[k] = raw[k][2];
  }

  channel12(v, &out[0]);
  channel12(i, &out[12]);
  channel12(temp, &out[24]);

  double sum_i2 = 0.0;
  double sum_abs_i = 0.0;
  int active_count = 0;

  double p_sum = 0.0;
  double p_sumsq = 0.0;
  double p_max = 0.0;

  for (int k = 0; k < 11; ++k) {
    const double current = (double)i[k];

    sum_i2 += current * current;
    sum_abs_i += fabs(current);

    if (fabs(current) > 0.1) {
      ++active_count;
    }

    const double discharge = (
      current < 0.0
      ? -current
      : 0.0
    );

    const double p = (
      (double)v[k]
      * discharge
    );

    p_sum += p;
    p_sumsq += p * p;

    if (k == 0 || p > p_max) {
      p_max = p;
    }
  }

  const double p_mean = p_sum / 11.0;

  double p_var = (
    p_sumsq / 11.0
    - p_mean * p_mean
  );

  if (p_var < 0.0 && p_var > -1e-12) {
    p_var = 0.0;
  }

  out[36] = (float)sqrt(sum_i2 / 11.0);
  out[37] = (float)(sum_abs_i / 11.0);
  out[38] = (float)((double)active_count / 11.0);
  out[39] = (float)p_mean;
  out[40] = (float)sqrt(p_var);
  out[41] = (float)p_max;
  out[42] = corr11(v, i);
  out[43] = temp[10] - temp[0];
}
