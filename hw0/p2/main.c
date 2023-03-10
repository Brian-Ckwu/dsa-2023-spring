#include <stdio.h>
#include <stdlib.h>

#define FILLED 'o'
#define EMPTY '_'

typedef struct {
  int n;
  int m;
  int** rows;
  int** cols;
} Config;

typedef struct {
  int m;
  int k;
  int* ls;
  int* ps;
  char* body;
} Row;

void read_constraints(int** constraints, int n);
void print_constraints(int** constraints, int n);
char** find_matrix(Config* config);
void print_matrix(char** matrix, Config* config);

int main(void) {
  // Read and parse input
  Config config;
  scanf("%d%d", &config.n, &config.m); // read n & m
  config.rows = (int**)calloc(config.n, sizeof(int*));
  config.cols = (int**)calloc(config.m, sizeof(int*));
  // read row constraints
  read_constraints(config.rows, config.n);
  read_constraints(config.cols, config.m);

  // Find the correct matrix
  char** matrix = find_matrix(&config);
  print_matrix(matrix, &config);

  // Free memory
  int i;
  for (i = 0; i < config.n; i++) {
    free(config.rows[i]);
    free(matrix[i]);
  }
  free(config.rows);
  free(matrix);
  for (i = 0; i < config.m; i++) {
    free(config.cols[i]);
  }
  free(config.cols);
  // TODO: free matrix
  return 0;
}

void print_matrix(char** matrix, Config* config) {
  int r, c;
  for (r = 0; r < config->n; r++) {
    for (c = 0; c < config->m; c++) {
      if (matrix[r][c]) {
        printf("%c", FILLED);
      } else {
        printf("%c", EMPTY);
      }
    }
    printf("\n");
  }
}

Row* _initialize_rows(Config* config) {
  Row* rows = (Row*)calloc(config->n, sizeof(Row));
  int r;
  for (r = 0; r < config->n; r++) {
    int k = config->rows[r][0];
    int* ls = (int*)calloc(k, sizeof(int));
    int j;
    for (j = 1; j <= k; j++) {
      ls[j - 1] = config->rows[r][j];
    }
    // ps & body need refreshing afterwards
    int* ps = (int*)calloc(k, sizeof(int));
    char* body = (char*)calloc(config->m, sizeof(char));
    Row row = {
      .m=config->m,
      .k=k,
      .ls=ls,
      .ps=ps,
      .body=body
    };
    rows[r] = row;
  }
  return rows;
}

void _update_row_body(Row* row) {
  int i;
  for (i = 0; i < row->m; i++) {
    row->body[i] = 0;
  }
  int p;
  for (p = 0; p < row->k; p++) {
    // Fill the block
    int f;
    for (f = 0; f < row->ls[p]; f++) {
      row->body[row->ps[p] + f] = 1;
    }
  }  
}

void _refresh_row(Row* row) {
  // Initialize filled blocks' pointers
  int p = 0;
  row->ps[p] = 0;
  for (p = 1; p < row->k; p++) {
    row->ps[p] = row->ps[p - 1] + row->ls[p - 1] + 1;
  }
  // Update row body
  _update_row_body(row);
}

int _next_row_state(Row* row){
  int p;
  int last_pos = row->m - 1;
  for (p = row->k - 1; p >= 0; p--) {
    if (row->ps[p] + row->ls[p] <= last_pos) {
      // move ps[p] to the right
      (row->ps[p])++;
      // move all ps[pp] to concatenate with ps[pp - 1] for all pp > p
      int pp;
      for (pp = p + 1; pp < row->k; pp++) {
        row->ps[pp] = row->ps[pp - 1] + row->ls[pp - 1] + 1;
      }
      // update row body
      _update_row_body(row);
      return 1;
    }
    last_pos = row->ps[p] - 2; // at least one space between filled blocks
  }
  return 0;
}

int _check_cols(char** matrix, Config* config) {
  // for each column
  int c;
  int* ls = (int*)calloc(config->n / 2 + 1, sizeof(int));
  int correct = 1;
  for (c = 0; c < config->m; c++) {
    // read column condition
    int k = 0;
    int l = 0;
    int r;
    for (r = 0; r < config->n; r++) {
      if (matrix[r][c]) {
        l++;
      } else {
        if (l) ls[k++] = l;
        l = 0;
      }
    }
    if (l) ls[k++] = l;
    // check column condition
    if (k != config->cols[c][0]) {
      correct = 0;
      break;
    }
    int i;
    for (i = 1; i <= config->cols[c][0]; i++) {
      if (ls[i - 1] != config->cols[c][i]) {
        correct = 0;
        break;
      }
    }
  }
  free(ls);
  return correct;
}

int _update_rows_and_check_cols(char** matrix, Row* rows, Config* config, int depth) {
  if (depth == config->n) {
    // check matrix and possibly return
    if (_check_cols(matrix, config)) return 1;
  } else {
    do {
      if (_update_rows_and_check_cols(matrix, rows, config, depth + 1)) return 1;
      // Refresh all rows after [depth] if depth != config->n
      int r;
      for (r = depth + 1; r < config->n; r++) {
        _refresh_row(&rows[r]);
      }
    } while (_next_row_state(&rows[depth]));
  }
  return 0;
}

// Go through all possible combinations of rows -> Check if correct in terms of columns
char** find_matrix(Config* config) {
  // Initialize the matrix
  char** matrix = (char**)calloc(config->n, sizeof(char*));
  // Initialize the state of every row
  Row* rows = _initialize_rows(config);
  int r;
  for (r = 0; r < config->n; r++) {
    _refresh_row(&rows[r]);
    matrix[r] = rows[r].body;
  }
  // Recursively update the rows and print the matrix
  _update_rows_and_check_cols(matrix, rows, config, 0);
  // Free rows
  for (r = 0; r < config->n; r++) {
    free(rows[r].ls);
    free(rows[r].ps);
  }
  free(rows);

  return matrix;
}

void read_constraints(int** constraints, int n) {
  int i;
  for (i = 0; i < n; i++) {
    int k;
    scanf("%d", &k);
    constraints[i] = (int*)calloc(k + 1, sizeof(int)); // k k1 k2 ... kn
    constraints[i][0] = k;
    int j;
    for (j = 1; j <= k; j++) {
      scanf("%d", &constraints[i][j]);
    }
  }
}

void print_constraints(int** constraints, int n) {
  int i;
  for (i = 0; i < n; i++) {
    int k = constraints[i][0];
    int j;
    for (j = 0; j <= k; j++) {
      printf("%d ", constraints[i][j]);
    }
    printf("\n");
  }
}
