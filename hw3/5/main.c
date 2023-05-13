#include <stdio.h>
#include <stdlib.h>

#define MAX_KNIGHTS 200000

struct Knight;
struct Heap; // use a vanilla dynamic array first, implement heap later

typedef struct Knight {
  int id;
  int heap_id; // disjoint set's ID
  int health;
  int attack;
  int num_attacks;
  long long rem_health; // remaining health
} Knight;

typedef struct Heap {
  int id;
  int max_size;
  int size;
  long long tot_attack; // total attack power
  long long rcv_attack; // total received attack points
  int num_attacks;
  int* arr; // dynamic array of knight IDs
} Heap;

Knight knights[MAX_KNIGHTS + 1];
Heap heaps[MAX_KNIGHTS + 1];

void print_knights(Knight* knights, int n) {
  int i;
  for (i = 1; i <= n; i++) {
    printf("ID: %d; HeapID: %d; Health: %d; Attack: %d; Rem_Health: %d; Num_Attacks: %d; Heap_Num_Attacks: %d, Heap_Tot_Attack: %d, Heap_Rcv_Attack: %d\n", knights[i].id, knights[i].heap_id, knights[i].health, knights[i].attack, knights[i].rem_health, knights[i].num_attacks, heaps[knights[i].heap_id].num_attacks, heaps[knights[i].heap_id].tot_attack, heaps[knights[i].heap_id].rcv_attack);
  }
}

int peep_heap(Heap* heap) { // get the root of the heap
  return heap->arr[0];
}

int total_num_attacks(int knight_id) {
  if (knights[knight_id].heap_id <= 0) return knights[knight_id].num_attacks;
  return knights[knight_id].num_attacks + heaps[knights[knight_id].heap_id].num_attacks;
}

void heapify_up(Heap* heap, int i) {
  while (i > 0) {
    int parent = (i - 1) / 2;
    if (knights[heap->arr[i]].rem_health < knights[heap->arr[parent]].rem_health) {
      int temp = heap->arr[i];
      heap->arr[i] = heap->arr[parent];
      heap->arr[parent] = temp;
      i = parent;
    } else {
      break;
    }
  }
}

void heapify_down(Heap* heap, int i) {
  while (2*i+1 < heap->size) {
    int j = 2*i+1;
    if (j+1 < heap->size && knights[heap->arr[j+1]].rem_health < knights[heap->arr[j]].rem_health) j++;
    if (knights[heap->arr[j]].rem_health < knights[heap->arr[i]].rem_health) {
      int temp = heap->arr[i];
      heap->arr[i] = heap->arr[j];
      heap->arr[j] = temp;
      i = j;
    } else {
      break;
    }
  }
}

// push_heap according to rem_health
void push_heap(Heap* heap, int id) {
  if (heap->size >= heap->max_size) {
    heap->max_size *= 2;
    heap->arr = (int*)realloc(heap->arr, sizeof(int) * heap->max_size);
  }
  // add one knight to heap
  // XXX: heap->tot_attack += knights[id].attack;
  heap->arr[heap->size] = id;
  heapify_up(heap, heap->size);
  heap->size++;
}

int pop_heap(Heap* heap) {
  Knight* knight = &knights[peep_heap(heap)];
  knight->heap_id = -1; // change to NULL
  knight->num_attacks += heap->num_attacks; // add num_attacks back to knight
  heap->tot_attack -= knight->attack;
  heap->arr[0] = heap->arr[--heap->size];
  heapify_down(heap, 0);
  return knight->id;
}

// merge b (smaller disjoint set) into a (larger disjoint set)
void merge_heaps(Heap* a, Heap* b) {
  a->tot_attack += b->tot_attack;
  int i;
  Knight* knight = NULL;
  for (i = 0; i < b->size; i++) {
    knight = &knights[b->arr[i]];
    knight->rem_health = knight->rem_health - b->rcv_attack + a->rcv_attack;
    knight->num_attacks = knight->num_attacks + b->num_attacks - a->num_attacks;
    knight->heap_id = a->id;
    push_heap(a, knight->id);
  }
}

void attack(Heap* a, Heap* b) {
  a->num_attacks++;
  b->rcv_attack += a->tot_attack;
  while (b->size > 0 && knights[peep_heap(b)].rem_health <= b->rcv_attack) {
    pop_heap(b);
  }
}

// focus on passing group 2 first
int main(void) {
  int n, m;
  scanf("%d %d", &n, &m);

  int i;
  // read healths
  for (i = 1; i <= n; i++) {
    scanf("%d", &knights[i].health);
  }
  // read attacks
  heaps[0].arr = NULL;
  for (i = 1; i <= n; i++) {
    scanf("%lld", &knights[i].attack);
    knights[i].id = i;
    knights[i].heap_id = i;
    knights[i].rem_health = knights[i].health;
    knights[i].num_attacks = 0;
    // initialize heap of this knight
    heaps[i].id = i;
    heaps[i].max_size = 1;
    heaps[i].size = 1;
    heaps[i].tot_attack = knights[i].attack;
    heaps[i].rcv_attack = 0;
    heaps[i].num_attacks = 0;
    heaps[i].arr = (int*)malloc(sizeof(int));
    heaps[i].arr[0] = i; // knight ID
  }

  // start attacking
  int Kai, Ksi;
  Heap *Ka_set, *Ks_set;
  for (i = 1; i <= m; i++) {
    scanf("%d %d", &Kai, &Ksi);
    if (knights[Kai].heap_id > 0 && knights[Ksi].heap_id > 0) { // both alive
      Ka_set = &heaps[knights[Kai].heap_id];
      Ks_set = &heaps[knights[Ksi].heap_id];
      if (Ka_set->id != Ks_set->id) { // ensure different heaps
        attack(Ka_set, Ks_set);
        if (Ka_set->size > Ks_set->size) {
          merge_heaps(Ka_set, Ks_set);
        } else {
          merge_heaps(Ks_set, Ka_set);
        }
      }
    }
  }

  // print num_attacks
  printf("%d", total_num_attacks(1));
  for (i = 2; i <= n; i++) {
    printf(" %d", total_num_attacks(i)); // total attacks = self attacks + leader attacks
  }

  // free
  for (i = 1; i <= n; i++) {
    free(heaps[i].arr);
  }
  return 0;
}