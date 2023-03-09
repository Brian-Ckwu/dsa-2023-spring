#include <stdio.h>
#include <stdlib.h>

void print_cartesian(int** arrs, const int arr_len, int* arr_lens, int* index, int depth);

int main(void) {
  int a[2] = {1, 2};
  int b[4] = {1, 2, 3, 4};
  int c[2] = {1, 2};
  int d[3] = {1, 3};

  int* arrs[] = {a, b, c, d};
  const int arr_len = sizeof(arrs) / sizeof(int*);
  int arr_lens[] = {2,4,2,2};
  int* index = (int*)calloc(arr_len, sizeof(int));
  print_cartesian(arrs, arr_len, arr_lens, index, 0);
  free(index);

  return 0;
}

void print_cartesian(int** arrs, const int arr_len, int* arr_lens, int* index, int depth) {
  if (depth == arr_len) {
    int i;
    for (i = 0; i < arr_len; i++) {
      printf("%d ", arrs[i][index[i]]);
    }
    printf("\n");
  } else { // visit every element of the array
    int i;
    for (i = 0; i < arr_lens[depth]; i++) {
      index[depth] = i;
      print_cartesian(arrs, arr_len, arr_lens, index, depth + 1);
    }
  }
}
