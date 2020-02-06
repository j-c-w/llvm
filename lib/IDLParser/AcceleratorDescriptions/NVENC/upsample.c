void upsample_1(int *in, int in_dim, int *out, int ratio) {
  for (int i = 0; i < in_dim; i += 1) {
    for (int j = 0; j < ratio; j ++) {
      out[i + j] = in[i];
    }
  }
}

void upsample_float_1(float *in, int in_dim, float *out, int ratio) {
  for (int i = 0; i < in_dim; i += 1) {
    for (int j = 0; j < ratio; j ++) {
      out[i + j] = in[i];
    }
  }
}

void upsample_double_1(double *in, int in_dim, double *out, int ratio) {
  for (int i = 0; i < in_dim; i += 1) {
    for (int j = 0; j < ratio; j ++) {
      out[i + j] = in[i];
    }
  }
}
