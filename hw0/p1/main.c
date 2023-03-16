#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIGINT_LEN 256

typedef short* BigInt;

int larger(BigInt a, BigInt b);
int iszero(BigInt n);
void multiply_by_2(BigInt n);
void divide_by_2(BigInt n);
void minus(BigInt m, BigInt n);
void print_bigint(BigInt bi);
BigInt s_to_bigint(char* s);
BigInt gcd_binary(BigInt a, BigInt b);

int main() {
  // Read input
  char as[BIGINT_LEN + 1]; // add an extra space for '\0' (i.e., 256 + 1)
  char bs[BIGINT_LEN + 1];
  scanf("%s %s", &as, &bs);

  // Parse to an integer array
  BigInt a = s_to_bigint(as);
  BigInt b = s_to_bigint(bs);

  // Solve GCD
  BigInt ans = gcd_binary(a, b);

  // Print the answer
  print_bigint(ans);
  printf("\n");

  free(a);
  free(b);
}

BigInt s_to_bigint(char* s) {
  if (strlen(s) > BIGINT_LEN) {
    printf("The length of %s should not exceed %d\n", s, BIGINT_LEN);
    exit(1);
  }

  BigInt bigint = (BigInt)calloc(BIGINT_LEN, sizeof(short));
  char* p = s;
  short* q = bigint + strlen(s) - 1;
  while (*p) {
    if (*p >= '0' && *p <= '9') {
      *q = *p - '0';
    } else {
      printf("There shouldn't be a non-integer char in s.\n");
      exit(1);
    }
    p++;
    q--;
  }

  return bigint;
}

int larger(BigInt a, BigInt b) {
  int i;
  for (i = BIGINT_LEN - 1; i >= 0; i--) {
    if (a[i] > b[i]) {
      return 1;
    } else if (a[i] < b[i]) {
      return 0;
    }
  }
  return 0;
}

int iszero(BigInt n) {
  int i;
  for (i = 0; i < BIGINT_LEN; i++) {
    if (n[i] != 0) return 0;
  }
  return 1;
}

void multiply_by_2(BigInt n) {
  int i;
  short carry = 0;
  for (i = 0; i < BIGINT_LEN; i++) {
    short prod = n[i] * 2 + carry;
    n[i] = prod % 10;
    carry = prod / 10;
  }
}

void divide_by_2(BigInt n) {
  int i;
  short carry = 0;
  for (i = BIGINT_LEN - 1; i >= 0; i--) {
    short dividend = carry * 10 + n[i];
    n[i] = dividend / 2;
    carry = dividend % 2;
  }
}

void minus(BigInt m, BigInt n) {
  int i;
  short borrow = 0;
  for (i = 0; i < BIGINT_LEN; i++) {
    short dif = m[i] - n[i] - borrow;
    if (dif < 0) {
      dif += 10;
      borrow = 1;  // for subtracting m[i+1]
    } else {
      borrow = 0;
    }
    m[i] = dif;
  }
}

BigInt gcd_binary(BigInt a, BigInt b) {
  BigInt ans = (BigInt)calloc(BIGINT_LEN, sizeof(short));
  // n ← min(a, b), m ← max(a, b), ans ← 1
  BigInt n = larger(a, b) ? b : a;
  BigInt m = larger(a, b) ? a : b;
  ans[0] = 1;

  BigInt temp;
  while ((!iszero(n)) && (!iszero(m))) {
    if ((n[0] % 2 == 0) && (m[0] % 2 == 0)) {
      multiply_by_2(ans);
      divide_by_2(n);
      divide_by_2(m);
    } else if (n[0] % 2 == 0) {
      divide_by_2(n);
    } else if (m[0] % 2 == 0) {
      divide_by_2(m);
    }
    if (larger(n, m)) {  // swap(n, m)
      temp = n;
      n = m;
      m = temp;
    }
    minus(m, n);
  }

  // mutiply ans by n
  while (ans[0] % 2 == 0) {
    multiply_by_2(n);
    divide_by_2(ans);
  }
  free(ans);
  return n;
}

void print_bigint(BigInt bi) {
  int i;
  int flag = 0;
  for (i = BIGINT_LEN - 1; i >= 0; i--) {
    if (bi[i] != 0) flag = 1;
    if (flag) printf("%d", bi[i]);
  }
}
