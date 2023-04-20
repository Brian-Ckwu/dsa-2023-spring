#include <stdio.h>
#include <stdlib.h>

#define C_MAX 1000000000

typedef struct {
  int price;
  int day;
} PriceDay;

typedef struct PriceDayHeap { // binary heap of PriceDays
  int size;
  PriceDay* price_days;
} PriceDayHeap;

typedef struct Node {
  int num_nodes;
  int num_children;
  int child_tail;
  int price_sum;
  struct Node** children;
  PriceDayHeap price_days_heap; // a binary heap of price days
} Node;

void push_heap(PriceDayHeap* heap, PriceDay price_day) {
  int i = heap->size;
  heap->size++;
  while (i > 0) {
    int parent = (i - 1) / 2;
    if (heap->price_days[parent].price > price_day.price) {
      heap->price_days[i] = heap->price_days[parent];
      i = parent;
    } else {
      break;
    }
  }
  heap->price_days[i] = price_day;
}

void pop_heap(PriceDayHeap* heap) {
  int i = 0;
  PriceDay last = heap->price_days[--heap->size];
  while (i * 2 + 1 < heap->size) {
    int left = i * 2 + 1;
    int right = i * 2 + 2;
    if (right < heap->size && heap->price_days[right].price < heap->price_days[left].price) {
      left = right;
    }
    if (heap->price_days[left].price < last.price) {
      heap->price_days[i] = heap->price_days[left];
      i = left;
    } else {
      break;
    }
  }
  heap->price_days[i] = last;
}

void print_node_stats(Node* node) {
  printf("num_nodes: %d, num_children: %d, child_tail: %d, price_sum: %d\n", node->num_nodes, node->num_children, node->child_tail, node->price_sum);
  // print price_days
  int i;
  for (i = 0; i < node->price_days_heap.size; i++) {
    printf("price: %d, day: %d\n", node->price_days_heap.price_days[i].price, node->price_days_heap.price_days[i].day);
  }
}

// post-order traversal functions
void calc_num_nodes(Node* node) {
  int i;
  for (i = 0; i < node->num_children; i++) {
    calc_num_nodes(node->children[i]);
    node->num_nodes += node->children[i]->num_nodes;
  }
  node->num_nodes++;
}

void update_price_sum(Node* node) {
  int i;
  node->price_sum = 0;
  for (i = 0; i < node->num_children; i++) {
    update_price_sum(node->children[i]);
    node->price_sum += node->children[i]->price_sum;
    if (node->price_sum > C_MAX) {
      node->price_sum = C_MAX + 1;
    }
  }
  node->price_sum += node->price_days_heap.price_days[0].price;
  if (node->price_sum > C_MAX) {
    node->price_sum = C_MAX + 1;
  }
}

int main(void) {
  int N, M, C;
  scanf("%d %d %d", &N, &M, &C);
  Node* nodes = calloc(N, sizeof(Node)); // O(N) space
  int* parents = calloc(N - 1, sizeof(int)); // the parent of each node
  // read parents calculate the number of children for each node
  int i;
  for (i = 0; i < N - 1; i++) {
    int parent;
    scanf("%d", &parent);
    parents[i] = parent - 1;
    nodes[parent - 1].num_children++;
  }
  // allocate space for children
  for (i = 0; i < N; i++) {
    if (nodes[i].num_children) {
      nodes[i].children = calloc(nodes[i].num_children, sizeof(Node*));
    }
  }
  // assign children
  for (i = 0; i < N - 1; i++) {
    int parent = parents[i];
    int child = i + 1;
    nodes[parent].children[nodes[parent].child_tail++] = &nodes[child];
  }
  // calculate num_nodes
  calc_num_nodes(&nodes[0]);
  // allocate space for price_days
  for (i = 0; i < N; i++) {
    if (nodes[i].num_nodes) {
      nodes[i].price_days_heap.price_days = calloc(M, sizeof(PriceDay));
    }
  }

  // start signing sweetmelon contracts
  int day;
  for (day = 0; day < M; day++) {
    int node;
    int max_melons = 0;
    for (node = 0; node < N; node++) {
      // remove outdated price_day
      while (nodes[node].price_days_heap.size && nodes[node].price_days_heap.price_days[0].day < day) {
        pop_heap(&nodes[node].price_days_heap);
      }

      int price, days;
      scanf("%d %d", &price, &days);
      PriceDay price_day = {price, day + days};
      push_heap(&nodes[node].price_days_heap, price_day);
    }
    update_price_sum(&nodes[0]);
    // find the valid node with the most melons
    for (node = 0; node < N; node++) {
      if (nodes[node].price_sum <= C && nodes[node].num_nodes > max_melons) {
        max_melons = nodes[node].num_nodes;
      }
    }
    printf("%d\n", max_melons);
  }

  // free memory
  for (i = 0; i < N; i++) {
    if (nodes[i].num_children) {
      free(nodes[i].children);
    }
    free(nodes[i].price_days_heap.price_days);
  }
  free(nodes);
  free(parents);

  return 0;
}
