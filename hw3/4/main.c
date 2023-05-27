#include <stdio.h>
#include <stdlib.h>

#define NUM_PRIMES 10

#define K1 29
#define K2 31
#define K3 37
#define K4 41
#define K5 43
#define K6 47
#define K7 53
#define K8 59
#define K9 61
#define K10 67

// 10000019	10000079	10000103	10000121	10000139	10000141	10000169	10000189	10000223	10000229
// 11002921	11002939	11002967	11003009	11003011	11003021	11003059	11003077	11003081	11003099
// 12000017	12000029	12000073	12000091	12000097	12000127	12000133	12000239	12000253	12000257
#define Q1 10000019
#define Q2 10000079
#define Q3 10000103
#define Q4 10000121
#define Q5 10000139
#define Q6 10000141
#define Q7 10000169
#define Q8 10000189
#define Q9 10000223
#define Q10 10000229

int hashes_1[Q1] = {0};
int hashes_2[Q2] = {0};
int hashes_3[Q3] = {0};
int hashes_4[Q4] = {0};
int hashes_5[Q5] = {0};
int hashes_6[Q6] = {0};
int hashes_7[Q7] = {0};
int hashes_8[Q8] = {0};
int hashes_9[Q9] = {0};
int hashes_10[Q10] = {0};

void print_string(char* s);
int mod(long long num, int q);

int main(void) {
  // read input
  int n, m;
  scanf("%d %d", &n, &m);

  char* s = (char*)malloc(sizeof(char) * (n + 1));
  char* p = (char*)malloc(sizeof(char) * (m + 1));
  scanf("%s %s", s, p);

  // compute the hashes of p and its rotations
  // p
  long long hash_1 = 0, hash_2 = 0, hash_3 = 0, hash_4 = 0, hash_5 = 0, hash_6 = 0, hash_7 = 0, hash_8 = 0, hash_9 = 0, hash_10 = 0;
  int i, j;
  for (i = 0; i < m; i++) {
    hash_1 = mod(hash_1 * K1 + p[i] - 'A', Q1);
    hash_2 = mod(hash_2 * K2 + p[i] - 'A', Q2);
    hash_3 = mod(hash_3 * K3 + p[i] - 'A', Q3);
    hash_4 = mod(hash_4 * K4 + p[i] - 'A', Q4);
    hash_5 = mod(hash_5 * K5 + p[i] - 'A', Q5);
    hash_6 = mod(hash_6 * K6 + p[i] - 'A', Q6);
    hash_7 = mod(hash_7 * K7 + p[i] - 'A', Q7);
    hash_8 = mod(hash_8 * K8 + p[i] - 'A', Q8);
    hash_9 = mod(hash_9 * K9 + p[i] - 'A', Q9);
    hash_10 = mod(hash_10 * K10 + p[i] - 'A', Q10);
  }
  hashes_1[hash_1] = 1;
  hashes_2[hash_2] = 1;
  hashes_3[hash_3] = 1;
  hashes_4[hash_4] = 1;
  hashes_5[hash_5] = 1;
  hashes_6[hash_6] = 1;
  hashes_7[hash_7] = 1;
  hashes_8[hash_8] = 1;
  hashes_9[hash_9] = 1;
  hashes_10[hash_10] = 1;
  // rotations
  long long km_1_1 = 1, km_1_2 = 1, km_1_3 = 1, km_1_4 = 1, km_1_5 = 1, km_1_6 = 1, km_1_7 = 1, km_1_8 = 1, km_1_9 = 1, km_1_10 = 1;
  for (i = 0; i < m - 1; i++) {
    km_1_1 = mod(km_1_1 * K1, Q1);
    km_1_2 = mod(km_1_2 * K2, Q2);
    km_1_3 = mod(km_1_3 * K3, Q3);
    km_1_4 = mod(km_1_4 * K4, Q4);
    km_1_5 = mod(km_1_5 * K5, Q5);
    km_1_6 = mod(km_1_6 * K6, Q6);
    km_1_7 = mod(km_1_7 * K7, Q7);
    km_1_8 = mod(km_1_8 * K8, Q8);
    km_1_9 = mod(km_1_9 * K9, Q9);
    km_1_10 = mod(km_1_10 * K10, Q10);
  }
  for (i = 0; i < m - 1; i++) {
    hash_1 = mod(K1 * (hash_1 - (p[i] - 'A') * km_1_1) + (p[i] - 'A'), Q1); hashes_1[hash_1] = i + 2;
    hash_2 = mod(K2 * (hash_2 - (p[i] - 'A') * km_1_2) + (p[i] - 'A'), Q2); hashes_2[hash_2] = i + 2;
    hash_3 = mod(K3 * (hash_3 - (p[i] - 'A') * km_1_3) + (p[i] - 'A'), Q3); hashes_3[hash_3] = i + 2;
    hash_4 = mod(K4 * (hash_4 - (p[i] - 'A') * km_1_4) + (p[i] - 'A'), Q4); hashes_4[hash_4] = i + 2;
    hash_5 = mod(K5 * (hash_5 - (p[i] - 'A') * km_1_5) + (p[i] - 'A'), Q5); hashes_5[hash_5] = i + 2;
    hash_6 = mod(K6 * (hash_6 - (p[i] - 'A') * km_1_6) + (p[i] - 'A'), Q6); hashes_6[hash_6] = i + 2;
    hash_7 = mod(K7 * (hash_7 - (p[i] - 'A') * km_1_7) + (p[i] - 'A'), Q7); hashes_7[hash_7] = i + 2;
    hash_8 = mod(K8 * (hash_8 - (p[i] - 'A') * km_1_8) + (p[i] - 'A'), Q8); hashes_8[hash_8] = i + 2;
    hash_9 = mod(K9 * (hash_9 - (p[i] - 'A') * km_1_9) + (p[i] - 'A'), Q9); hashes_9[hash_9] = i + 2;
    hash_10 = mod(K10 * (hash_10 - (p[i] - 'A') * km_1_10) + (p[i] - 'A'), Q10); hashes_10[hash_10] = i + 2;
  }

  // start matching s and p
  // printf("Start matching...\n");
  int count = 0;
  // compute hash of the first m characters of s
  hash_1 = 0, hash_2 = 0, hash_3 = 0, hash_4 = 0, hash_5 = 0, hash_6 = 0, hash_7 = 0, hash_8 = 0, hash_9 = 0, hash_10 = 0;
  for (i = 0; i < m; i++) {
    hash_1 = mod(hash_1 * K1 + s[i] - 'A', Q1);
    hash_2 = mod(hash_2 * K2 + s[i] - 'A', Q2);
    hash_3 = mod(hash_3 * K3 + s[i] - 'A', Q3);
    hash_4 = mod(hash_4 * K4 + s[i] - 'A', Q4);
    hash_5 = mod(hash_5 * K5 + s[i] - 'A', Q5);
    hash_6 = mod(hash_6 * K6 + s[i] - 'A', Q6);
    hash_7 = mod(hash_7 * K7 + s[i] - 'A', Q7);
    hash_8 = mod(hash_8 * K8 + s[i] - 'A', Q8);
    hash_9 = mod(hash_9 * K9 + s[i] - 'A', Q9);
    hash_10 = mod(hash_10 * K10 + s[i] - 'A', Q10);
  }
  // print the hashes for debugging
  for (i = 0; i < n - m + 1; i++) {
    if (hashes_1[hash_1]!=0 && hashes_2[hash_2]!=0 && hashes_3[hash_3]!=0 && hashes_4[hash_4]!=0 && hashes_5[hash_5]!=0 && hashes_6[hash_6]!=0 && hashes_7[hash_7]!=0 && hashes_8[hash_8]!=0 && hashes_9[hash_9]!=0 && hashes_10[hash_10]!=0) {
      count++;
    }
    // compute hash of the next m characters of s
    hash_1 = mod(K1 * (hash_1 - (s[i] - 'A') * km_1_1) + (s[i + m] - 'A'), Q1);
    hash_2 = mod(K2 * (hash_2 - (s[i] - 'A') * km_1_2) + (s[i + m] - 'A'), Q2);
    hash_3 = mod(K3 * (hash_3 - (s[i] - 'A') * km_1_3) + (s[i + m] - 'A'), Q3);
    hash_4 = mod(K4 * (hash_4 - (s[i] - 'A') * km_1_4) + (s[i + m] - 'A'), Q4);
    hash_5 = mod(K5 * (hash_5 - (s[i] - 'A') * km_1_5) + (s[i + m] - 'A'), Q5);
    hash_6 = mod(K6 * (hash_6 - (s[i] - 'A') * km_1_6) + (s[i + m] - 'A'), Q6);
    hash_7 = mod(K7 * (hash_7 - (s[i] - 'A') * km_1_7) + (s[i + m] - 'A'), Q7);
    hash_8 = mod(K8 * (hash_8 - (s[i] - 'A') * km_1_8) + (s[i + m] - 'A'), Q8);
    hash_9 = mod(K9 * (hash_9 - (s[i] - 'A') * km_1_9) + (s[i + m] - 'A'), Q9);
    hash_10 = mod(K10 * (hash_10 - (s[i] - 'A') * km_1_10) + (s[i + m] - 'A'), Q10);
  }

  // print result
  printf("%d\n", count);

  // free memory
  free(s);
  free(p);
  return 0;
}

void print_string(char* s) {
  int i = 0;
  while (s[i] != '\0') {
    printf("%c -> %d\n", s[i], s[i] - 'A');
    i++;
  }
  printf("\n");
}

int mod(long long num, int q) {
  return (num % q + q) % q;
}
