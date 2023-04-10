#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
  int row;
  int col;
} Position;

void print_matrix(int** matrix, int n, int m);
int** read_matrix_array(unsigned int n, unsigned int m);
void convert_rank_to_position(int** ranks, Position* rank2pos, int n, int m);
void print_rank_positions(Position* rank2pos, int n, int m);
void training(int** ranks, int** toasts, int n, int m, int days, Position* rank2pos);
void racing(int** ranks, int** toasts, int n, int m, int days, Position* rank2pos);
void swap(int* a, int* b);
int pos_is_valid(int row, int col, int n, int m);
int all_border_invalid(int row, int col, int offset, int n, int m);
void free_matrix(int** matrix, int rows);

// Currently: array implementation
int main(void) {
  // Read inputs
  unsigned int n, m;
  scanf("%d %d", &n, &m);

  int** ranks = read_matrix_array(n, m); // Time: O(n * m), Space: O(n * m)
  int** toasts = read_matrix_array(n, m); // Time: O(n * m), Space: O(n * m)
  Position* rank2pos = (Position*) calloc(n * m, sizeof(Position)); // Time: O(n * m), Space: O(n * m)
  convert_rank_to_position(ranks, rank2pos, n, m); // Time: O(n * m)

  unsigned int T1, R, T2;
  scanf("%d %d %d", &T1, &R, &T2);

  training(ranks, toasts, n, m, T1, rank2pos); // Time: O(T1 * length * max(n, m))
  racing(ranks, toasts, n, m, R, rank2pos); // Time: O(R * length^2)
  training(ranks, toasts, n, m, T2, rank2pos); // Time: O(T2 * length * max(n, m))
  free(rank2pos);

  // Print final number of toasts
  print_matrix(toasts, n, m);

  // Free memory
  free_matrix(ranks, n);
  free_matrix(toasts, n);
}

int pos_is_valid(int row, int col, int n, int m) {
  return (0<=row && row<n && 0<=col && col<m);
}

int all_border_invalid(int row, int col, int offset, int n, int m) {
  return (
    !pos_is_valid(row, col + offset, n, m) &&
    !pos_is_valid(row + offset, col, n, m) &&
    !pos_is_valid(row, col - offset, n, m) &&
    !pos_is_valid(row - offset, col, n, m)
  );
}

void training(int** ranks, int** toasts, int n, int m, int days, Position* rank2pos) {
  int** visited = (int**) calloc(n, sizeof(int*));
  int row;
  for (row = 0; row < n; row++) {
    visited[row] = (int*) calloc(m, sizeof(int));
  }

  int day;
  for (day = 0; day < days; day++) {
    int rank_init, length, size;
    scanf("%d %d %d", &rank_init, &length, &size);

    // Initialize visited matrix
    int i, j;
    for (i = 0; i < n; i++) {
      for (j = 0; j < m; j++) {
        visited[i][j] = 0;
      }
    }

    Position center = rank2pos[rank_init - 1];
    visited[center.row][center.col] = 1;

    int l; // visit [length] bakeries
    for (l = 0; l < length; l++) {
      int offset = 1;
      int found_bakery = 0;
      while (!all_border_invalid(center.row, center.col, offset, n, m)) {
        int dirs[5] = {0, offset, 0, -offset, 0};
        int dir;
        int min_rank = INT_MAX;
        int min_r = INT_MAX;
        int min_c = INT_MAX;
        for (dir = 0; dir < 4; dir++) {
          int r = center.row + dirs[dir];
          int c = center.col + dirs[dir + 1];
          if (pos_is_valid(r, c, n, m) && !visited[r][c] && toasts[r][c] > 0 && ranks[r][c] < min_rank) {
            min_rank = ranks[r][c];
            min_r = r;
            min_c = c;
            found_bakery = 1;
          }
        }

        if (found_bakery) {
          toasts[min_r][min_c] -= size;
          if (toasts[min_r][min_c] < 0) {
            toasts[min_r][min_c] = 0;
          }
          center.row = min_r;
          center.col = min_c;
          visited[min_r][min_c] = 1;

          // for debugging
          print_matrix(toasts, n, m);
          printf("\n");

          break;
        }
        offset++;
      }
      if (!found_bakery) break;
    }
  }
  free_matrix(visited, n);
}

void racing(int** ranks, int** toasts, int n, int m, int days, Position* rank2pos) {
  int day;
  for (day = 0; day < days; day++) { // Time: O(days * length^2)
    int rank_init, length;
    scanf("%d %d", &rank_init, &length);
    Position center = rank2pos[rank_init - 1];
    int max_offset = length / 2;
    int num_swaps = (length * length - 1) / 2;
    int i;
    for (i = 0; i < num_swaps; i++) { // Time: O(length^2)
      int row_offset = -max_offset + (i % length);
      int col_offset = -max_offset + (i / length);
      swap(&ranks[center.row + row_offset][center.col + col_offset], &ranks[center.row - row_offset][center.col - col_offset]);
      swap(&toasts[center.row + row_offset][center.col + col_offset], &toasts[center.row - row_offset][center.col - col_offset]);
    }
    convert_rank_to_position(ranks, rank2pos, n, m);
  }
}

void swap(int* a, int* b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void print_matrix(int** matrix, int n, int m) {
  int i, j;
  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
}

int** read_matrix_array(unsigned int n, unsigned int m) {
  int** matrix = (int**) calloc(n, sizeof(int*));
  int i, j;
  for (i = 0; i < n; i++) {
    matrix[i] = (int*) calloc(m, sizeof(int));
    for (j = 0; j < m; j++) {
      scanf("%d", &matrix[i][j]);
    }
  }
  return matrix;
}

void convert_rank_to_position(int** ranks, Position* rank2pos, int n, int m) {
  int i, j;
  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      rank2pos[ranks[i][j] - 1] = (Position) {i, j};
    }
  }
}

void print_rank_positions(Position* rank2pos, int n, int m) {
  int i;
  for (i = 0; i < n * m; i++) {
    printf("Row: %d, Col: %d\n", rank2pos[i].row, rank2pos[i].col);
  }
}

void free_matrix(int** matrix, int rows) {
  int row;
  for (row = 0; row < rows; row++) {
    free(matrix[row]);
  }
  free(matrix);
}