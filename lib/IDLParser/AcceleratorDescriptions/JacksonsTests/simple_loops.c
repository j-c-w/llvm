/** Sum arrays */
int sum(int *arr, int len) {
  int sum = 0;
  for (int i = 0; i < len; i ++) {
    sum += arr[i];
  }

  return sum;
}

double sum_double(double *arr, int len) {
  double sum = 0;
  for (int i = 0; i < len; i ++) {
    sum += arr[i];
  }

  return sum;
}

float sum_float(float *arr, int len) {
  float sum = 0;
  for (int i = 0; i < len; i ++) {
    sum += arr[i];
  }

  return sum;
}

/** Increment arrays */
void inc(int *arr, int len) {
  for (int i = 0; i < len; i ++) {
    arr[i] += 1;
  }
}

void inc_double(double *arr, int len) {
  for (int i = 0; i < len; i ++) {
    arr[i] += 1;
  }
}

void inc_float(float *arr, int len) {
  for (int i = 0; i < len; i ++) {
    arr[i] += 1;
  }
}
