#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

typedef unsigned char bool;
typedef struct Node {
  int rank;
  int toasts;
  struct Node* l; // left
  struct Node* r; // right
  struct Node* u; // up
  struct Node* d; // down
  struct Node* lop; // first left open bakery
  struct Node* rop; // first right open bakery
  struct Node* uop; // first up open bakery
  struct Node* dop; // first down open bakery
  struct Node* lvn; // first left valid neighbor
  struct Node* rvn; // first right valid neighbor
  struct Node* uvn; // first up valid neighbor
  struct Node* dvn; // first down valid neighbor
  unsigned short x;
  unsigned short y;
} Node;

typedef struct {
  int n;
  int m;
  Node* lefts;
  Node* rights;
  Node* tops;
  Node* bottoms;
  Node** nodes;
} Grid;

typedef struct {
  Node** nodes;
  int size;
} VisitedStack;

Node* initialize_terminal_nodes(int k) {
  Node* nodes = (Node*) malloc(k * sizeof(Node));
  for (int i = 0; i < k; i++) {
    nodes[i].rank = 0;
    nodes[i].toasts = 0;
    // point to itself
    nodes[i].l = &nodes[i];
    nodes[i].r = &nodes[i];
    nodes[i].u = &nodes[i];
    nodes[i].d = &nodes[i];
  }
  return nodes;
}

Node** initialize_body_nodes(int n, int m) {
  Node** nodes = (Node**) malloc(n * sizeof(Node*));
  for (int i = 0; i < n; i++) {
    nodes[i] = (Node*) malloc(m * sizeof(Node));
  }
  return nodes;
}

void connect_grid_nodes(Grid* grid) {
  // horizontal connections
  int row;
  for (row = 0; row < grid->n; row++) {
    grid->lefts[row].r = &grid->nodes[row][0];
    grid->nodes[row][0].l = &grid->lefts[row];
    grid->rights[row].l = &grid->nodes[row][grid->m - 1];
    grid->nodes[row][grid->m - 1].r = &grid->rights[row];
    int col;
    for (col = 0; col < grid->m - 1; col++) {
      grid->nodes[row][col].r = &grid->nodes[row][col + 1];
      grid->nodes[row][col + 1].l = &grid->nodes[row][col];
    }
  }
  // vertical connections
  int col;
  for (col = 0; col < grid->m; col++) {
    grid->tops[col].d = &grid->nodes[0][col];
    grid->nodes[0][col].u = &grid->tops[col];
    grid->bottoms[col].u = &grid->nodes[grid->n - 1][col];
    grid->nodes[grid->n - 1][col].d = &grid->bottoms[col];
    int row;
    for (row = 0; row < grid->n - 1; row++) {
      grid->nodes[row][col].d = &grid->nodes[row + 1][col];
      grid->nodes[row + 1][col].u = &grid->nodes[row][col];
    }
  }
  // initialize first open bakeries
  for (row = 0; row < grid->n; row++) {
    for (col = 0; col < grid->m; col++) {
      grid->nodes[row][col].lop = grid->nodes[row][col].l;
      grid->nodes[row][col].rop = grid->nodes[row][col].r;
      grid->nodes[row][col].uop = grid->nodes[row][col].u;
      grid->nodes[row][col].dop = grid->nodes[row][col].d;
      grid->nodes[row][col].lvn = grid->nodes[row][col].l;
      grid->nodes[row][col].rvn = grid->nodes[row][col].r;
      grid->nodes[row][col].uvn = grid->nodes[row][col].u;
      grid->nodes[row][col].dvn = grid->nodes[row][col].d;
      grid->nodes[row][col].x = row;
      grid->nodes[row][col].y = col;
    }
  }
}

Grid initialize_grid(int n, int m) {
  Grid grid = {
    .n = n,
    .m = m,
    .lefts = initialize_terminal_nodes(n),
    .rights = initialize_terminal_nodes(n),
    .tops = initialize_terminal_nodes(m),
    .bottoms = initialize_terminal_nodes(m),
    .nodes = initialize_body_nodes(n, m)
  };
  connect_grid_nodes(&grid);
  return grid;
}

void free_grid(Grid* grid) {
  free(grid->lefts);
  free(grid->rights);
  free(grid->tops);
  free(grid->bottoms);
  int row;
  for (row = 0; row < grid->n; row++) {
    free(grid->nodes[row]);
  }
  free(grid->nodes);
}

void print_grid(Grid* grid) {
  int row, col;
  for (row = 0; row < grid->n; row++) {
    Node* node = &grid->lefts[row]; // traverse from each row from the left
    for (col = 0; col < grid->m; col++) {
      node = node->r;
      printf("%d ", node->toasts);
    }
    printf("\n");
  }
}

void print_rank_to_toasts(Node** rank_to_node, int n, int m) {
  int i;
  for (i = 0; i < n * m; i++) {
    printf("%d: %d\n", i + 1, rank_to_node[i]->toasts);
  }
}

void reset_visit_status(VisitedStack* visited_today) {
  while (visited_today->size) {
    visited_today->size--;
    Node* node = visited_today->nodes[visited_today->size];
    node->lop->rvn = node->lop->rop; node->rop->lvn = node->rop->lop; node->uop->dvn = node->uop->dop; node->dop->uvn = node->dop->uop;
  }
}

void training(Grid* grid, Node** rank_to_node, int start_day, int days) {
  int day;
  VisitedStack visited_today;
  for (day = start_day; day < start_day + days; day++) {
    int rank_init, len, team_size;
    scanf("%d %d %d", &rank_init, &len, &team_size);

    visited_today.nodes = (Node**) malloc((len + 1) * sizeof(Node*));

    Node* center = rank_to_node[rank_init - 1];
    visited_today.nodes[0] = center; visited_today.size = 1;
    center->lvn->rvn = center->rvn; center->rvn->lvn = center->lvn; center->uvn->dvn = center->dvn; center->dvn->uvn = center->uvn;

    int i;
    for (i = 0; i < len; i++) {
      // try to find a bakery starting from the center
      Node *bakery = NULL;
      int min_dist = INT_MAX;
      int min_rank = INT_MAX;
      int dist;
      if (center->lvn->rank && ((dist=abs(center->x - center->lvn->x)+abs(center->y - center->lvn->y)) < min_dist || (dist==min_dist && center->lvn->rank < min_rank))) {bakery=center->lvn; min_dist=dist; min_rank=center->lvn->rank;}
      if (center->rvn->rank && ((dist=abs(center->x - center->rvn->x)+abs(center->y - center->rvn->y)) < min_dist || (dist==min_dist && center->rvn->rank < min_rank))) {bakery=center->rvn; min_dist=dist; min_rank=center->rvn->rank;}
      if (center->uvn->rank && ((dist=abs(center->x - center->uvn->x)+abs(center->y - center->uvn->y)) < min_dist || (dist==min_dist && center->uvn->rank < min_rank))) {bakery=center->uvn; min_dist=dist; min_rank=center->uvn->rank;}
      if (center->dvn->rank && ((dist=abs(center->x - center->dvn->x)+abs(center->y - center->dvn->y)) < min_dist || (dist==min_dist && center->dvn->rank < min_rank))) {bakery=center->dvn; min_dist=dist; min_rank=center->dvn->rank;}

      if (bakery != NULL) {
        bakery->toasts -= team_size;
        bakery->lvn->rvn = bakery->rvn; bakery->rvn->lvn = bakery->lvn; bakery->uvn->dvn = bakery->dvn; bakery->dvn->uvn = bakery->uvn;
        if (bakery->toasts <= 0) {
          bakery->toasts = 0;
          bakery->lop->rop = bakery->rop; bakery->rop->lop = bakery->lop; bakery->uop->dop = bakery->dop; bakery->dop->uop = bakery->uop;
        }
        center = bakery;
        visited_today.nodes[visited_today.size] = bakery; visited_today.size++;
      } else {
        break;
      }
    }
    reset_visit_status(&visited_today);
    free(visited_today.nodes); visited_today.nodes = NULL;
  }
}

void racing(Grid* A, Grid* B, Node** rank_to_node_A, Node** rank_to_node_B, int days) {
  // Update grid B's status to match grid A's
  int row, col;
  for (row = 0; row < A->n; row++) {
    for (col = 0; col < A->m; col++) {
      B->nodes[B->n - 1 - row][B->m - 1 - col].toasts = A->nodes[row][col].toasts;
    }
  }
  // Start rotating
  int day;
  for (day = 0; day < days; day++) {
    int rank_init, l;
    scanf("%d %d", &rank_init, &l);
    // Get center nodes
    Node* a = rank_to_node_A[rank_init - 1];
    Node* b = rank_to_node_B[rank_init - 1];
    // Walk to the left upper corner
    int i;
    for (i = 0; i < l / 2; i++) {
      a = a->l->u;
      b = b->l->u;
    }
    // Switch left-border pointers
    Node* al = a->l;
    a->l = b->l; b->l->r = a;
    b->l = al; al->r = b;
    for (i = 1; i < l; i++) {
      a = a->d;
      b = b->d;
      al = a->l;
      a->l = b->l; b->l->r = a;
      b->l = al; al->r = b;
    }
    // Switch bottom-border pointers (down)
    Node* ad = a->d;
    a->d = b->d; b->d->u = a;
    b->d = ad; ad->u = b;
    for (i = 1; i < l; i++) {
      a = a->r;
      b = b->r;
      ad = a->d;
      a->d = b->d; b->d->u = a;
      b->d = ad; ad->u = b;
    }
    // Switch right-border pointers
    Node* ar = a->r;
    a->r = b->r; b->r->l = a;
    b->r = ar; ar->l = b;
    for (i = 1; i < l; i++) {
      a = a->u;
      b = b->u;
      ar = a->r;
      a->r = b->r; b->r->l = a;
      b->r = ar; ar->l = b;
    }
    // Switch top-border pointers (up)
    Node* au = a->u;
    a->u = b->u; b->u->d = a;
    b->u = au; au->d = b;
    for (i = 1; i < l; i++) {
      a = a->l;
      b = b->l;
      au = a->u;
      a->u = b->u; b->u->d = a;
      b->u = au; au->d = b;
    }
  }
  // Update rank_to_node_A (no need to update rank_to_node_B)
  for (row = 0; row < A->n; row++) {
    Node* node = &A->lefts[row]; // traverse from each row from the left
    for (col = 0; col < A->m; col++) {
      node = node->r;
      rank_to_node_A[node->rank - 1] = node;
      node->lop = node->l; node->rop = node->r; node->uop = node->u; node->dop = node->d;
      node->lvn = node->l; node->rvn = node->r; node->uvn = node->u; node->dvn = node->d;
    }
  }
  // Update A's open bakeries
  for (row = 0; row < A->n; row++) {
    Node* node = &A->lefts[row]; // traverse from each row from the left
    for (col = 0; col < A->m; col++) {
      node = node->r;
      node->x = row;
      node->y = col;
      if (node->toasts == 0) {
        node->lop->rop = node->rop; node->rop->lop = node->lop; node->uop->dop = node->dop; node->dop->uop = node->uop;
        node->lvn->rvn = node->rop; node->rvn->lvn = node->lop; node->uvn->dvn = node->dop; node->dvn->uvn = node->uop;
      }
    }
  }
}

/*
Linked-list implementation
*/
int main(void) {
  int n, m;
  scanf("%d %d", &n, &m);

  // Initialize two street grids -> Time: O(n + m), Space: O(n + m)
  Grid A = initialize_grid(n, m); // 0 degrees
  Grid B = initialize_grid(n, m); // 180 degrees rotated grid of A

  // Maps for storing rank to node pointers
  Node** rank_to_node_A = malloc(n * m * sizeof(Node*));
  Node** rank_to_node_B = malloc(n * m * sizeof(Node*));

  // Read ranks and record mappings
  int row, col;
  for (row = 0; row < n; row++) {
    for (col = 0; col < m; col++) {
      int rank;
      scanf("%d", &rank);
      A.nodes[row][col].rank = rank;
      B.nodes[n - 1 - row][m - 1 - col].rank = rank;
      rank_to_node_A[rank - 1] = &A.nodes[row][col];
      rank_to_node_B[rank - 1] = &B.nodes[n - 1 - row][m - 1 - col];
    }
  }

  // Read toasts
  for (row = 0; row < n; row++) {
    for (col = 0; col < m; col++) {
      int toasts;
      scanf("%d", &toasts);
      A.nodes[row][col].toasts = toasts;
      B.nodes[n - 1 - row][m - 1 - col].toasts = toasts;
    }
  }

  int T1, R, T2;
  scanf("%d %d %d", &T1, &R, &T2);

  training(&A, rank_to_node_A, 1, T1);
  racing(&A, &B, rank_to_node_A, rank_to_node_B, R);
  training(&A, rank_to_node_A, T1 + 1, T2);

  print_grid(&A);

  // Free memory
  free(rank_to_node_A);
  free(rank_to_node_B);
  free_grid(&A);
  free_grid(&B);
}
