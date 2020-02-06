void downsample_1(int *in, int in_dim, int *out, int ratio) {
  for (int i = 0; i < in_dim; i += ratio) {
    int sum = 0;
    for (int j = 0; j < ratio; j ++) {
      sum += in[i + j];
    }
    out[i] = sum / ratio;
  }
}

void downsample_2(int *in, int in_dim, int *out, int ratio) {
  int count = 0;
  int sum = 0;
  for (int i = 0; i < in_dim; i += 1) {
    if (count == ratio) {
      out[i / ratio] = sum / ratio;
      sum = 0;
    }
    sum += ratio;
    count ++;
  }
}

void downsample_float_1(float *in, int in_dim, float *out, int ratio) {
  for (int i = 0; i < in_dim; i += ratio) {
    float sum = 0;
    for (int j = 0; j < ratio; j ++) {
      sum += in[i + j];
    }
    out[i] = sum / ratio;
  }
}

void downsample_float_2(float *in, int in_dim, float *out, int ratio) {
  int count = 0;
  float sum = 0;
  for (int i = 0; i < in_dim; i += 1) {
    if (count == ratio) {
      out[i / ratio] = sum / ratio;
      sum = 0;
    }
    sum += ratio;
    count ++;
  }
}

void downsample_double_1(double *in, int in_dim, double *out, int ratio) {
  for (int i = 0; i < in_dim; i += ratio) {
    double sum = 0;
    for (int j = 0; j < ratio; j ++) {
      sum += in[i + j];
    }
    out[i] = sum / ratio;
  }
}

void downsample_double_2(double *in, int in_dim, double *out, int ratio) {
  int count = 0;
  double sum = 0;
  for (int i = 0; i < in_dim; i += 1) {
    if (count == ratio) {
      out[i / ratio] = sum / ratio;
      sum = 0;
    }
    sum += ratio;
    count ++;
  }
}
