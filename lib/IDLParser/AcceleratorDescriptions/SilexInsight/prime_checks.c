#include <math.h>

int is_prime(int n) {
  for (int i = 2; i < sqrt(n); i ++) {
    if (n % i == 0) {
      return 0;
    }
  }

  return 1;
}

int is_prime_while(int n) {
  int upto = sqrt(n);
  int i = 2;

  while (i < upto) {
    if (n % i == 0) {
      return 1;
    }
    i ++;
  }

  return 0;
}
