#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_MAX 1000000

#define K 29
#define P 10000019

typedef struct String {
  int set_id;
  int start; // TODO: rotational string (cyclic array)
  char* data;
  struct String* next;
} String;

typedef struct {
  int size;
  String* str_list;
} Bucket;

long long set_sizes[2 * N_MAX + 1];
char str_buf[N_MAX + 1];
Bucket str_buckets[P];

long long mod(long long hash);
void print_string(String* string);
String* find_string(long long hash, char* str);
void insert_string(long long hash, String* string);

int M; // for rotational string related functions

int main(void) {
  int N, Q = 0;
  scanf("%d %d", &N, &Q);

  int set_count = 0;
  long long ans = 0;
  long long hash;
  long long km_1 = 1;
  int i, j; // for loop variables
  String* string = NULL; // for holding reference of found strings

  // Start reading N strings
  // 1. Read the first string to determine M
  scanf("%s", str_buf);
  M = strlen(str_buf);
  // precompute qm_1
  for (j = 0; j < M - 1; j++) {
    km_1 = (km_1 * K) % P;
  }
  // allocate space for string data
  int string_id = 0; // [0, (N + Q) * M)
  String* strings = (String*)calloc((N + Q) * M, sizeof(String));
  int string_data_id = 0; // [0, (N + Q))
  char** string_data = (char**)malloc((N + Q) * sizeof(char*));
  for (j = 0; j < (N + Q); j++) {
    string_data[j] = (char*)malloc((M + 1) * sizeof(char));
  }
  // copy the first string from str_buf to str_data
  for (j = 0; j <= M; j++) {
    string_data[string_data_id][j] = str_buf[j];
  }
  // XXX: debugging
  // printf("%s\n", string_data[string_data_id]);
  
  // Hash the string: step 1 & 2
  // step 1: hash the original string
  hash = 0;
  for (j = 0; j < M; j++) {
    hash = (hash * K + (string_data[string_data_id][j] - 'a')) % P;
  }
  // step 2-1: compute rolling hashes and add to buckets
  set_count++;
  for (j = 0; j < M; j++) {
    // insert the string from the last iteration
    strings[string_id].set_id = set_count;
    strings[string_id].start = j;
    strings[string_id].data = string_data[string_data_id];
    insert_string(hash, &strings[string_id]);

    // XXX: debugging
    // print_string(&strings[string_id]);

    string_id++;
    // hash the rotated string
    hash = mod((hash - (string_data[string_data_id][j] - 'a') * km_1) * K + (string_data[string_data_id][j] - 'a'));
  }
  string_data_id++;

  // 2. Read the remaining strings
  for (i = 1; i < N; i++) {
    scanf("%s", string_data[string_data_id]);
    // Hash the string: step 1 & 2
    hash = 0;
    // step 1: hash the original string
    for (j = 0; j < M; j++) {
      hash = (hash * K + (string_data[string_data_id][j] - 'a')) % P;
    }

    string = find_string(hash, string_data[string_data_id]);
    if (string == NULL) { // step 2-1: compute rolling hashes and add to buckets
      set_count++;
      for (j = 0; j < M; j++) {
        // insert the string from the last iteration
        strings[string_id].set_id = set_count;
        strings[string_id].start = j;
        strings[string_id].data = string_data[string_data_id];
        insert_string(hash, &strings[string_id]);

        // XXX: debugging
        // print_string(&strings[string_id]);

        string_id++;
        // hash the rotated string
        hash = mod((hash - (string_data[string_data_id][j] - 'a') * km_1) * K + (string_data[string_data_id][j] - 'a'));
      }
    } else { // step 2-2: increase ans
      ans += ++set_sizes[string->set_id];
    }
    string_data_id++;
  }
  printf("%lld\n", ans);

  int op;
  for (i = 1; i <= Q; i++) {
    scanf("%d %s", &op, string_data[string_data_id]);
    hash = 0;
    for (j = 0; j < M; j++) {
      hash = (hash * K + (string_data[string_data_id][j] - 'a')) % P;
    }
    string = find_string(hash, string_data[string_data_id]);
    if (op == 1) {
      if (string != NULL) {
        ans += ++set_sizes[string->set_id];
      } else { // add rolling hashes
        set_count++;
        for (j = 0; j < M; j++) {
          strings[string_id].set_id = set_count;
          strings[string_id].start = j;
          strings[string_id].data = string_data[string_data_id];
          insert_string(hash, &strings[string_id]);

          string_id++;          

          hash = mod((hash - (string_data[string_data_id][j] - 'a') * km_1) * K + (string_data[string_data_id][j] - 'a'));
        }
      }
    } else {
      ans -= set_sizes[string->set_id]--;
    }
    printf("%lld\n", ans);
    string_data_id++;
  }

  // free memory
  for (i = 0; i < (N + Q); i++) {
    free(string_data[i]);
  }
  free(strings);
  free(string_data);
}

long long mod(long long hash) {
  return ((hash % P) + P) % P;
}

void print_string(String* string) {
  int i, j;
  for (i = 0; i < M; i++) {
    j = (string->start + i) % M;
    printf("%c", string->data[j]);
  }
  printf("\n");
}

String* find_string(long long hash, char* str) {
  Bucket* bucket = &str_buckets[hash];
  if (bucket->size > 0) {
    String* string = bucket->str_list;
    int i, j;
    while (string) {
      int match = 1;
      for (i = 0; i < M; i++) {
        j = (string->start + i) % M;
        if (str[i] != string->data[j]) {
          match = 0;
          break;
        }
      }
      if (match) return string;
      string = string->next;
    }
  }
  return NULL;
}

void insert_string(long long hash, String* string) {
  if (str_buckets[hash].size == 0) { // no strings in this bucket yet
    string->next = NULL;
    str_buckets[hash].str_list = string;
  } else {
    string->next = str_buckets[hash].str_list;
    str_buckets[hash].str_list = string;
  }
  str_buckets[hash].size++;
}
