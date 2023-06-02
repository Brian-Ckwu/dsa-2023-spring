#include <stdio.h>
#include <stdlib.h>

#define MAX_N 100000

typedef long long ll;
const ll Q = 1000000007LL;

enum Color { RED, BLACK };

typedef struct Node {
  ll min_price;
  ll max_price;
  ll key;
  ll even_sum;
  ll odd_sum;
  int num_nodes; // number of nodes in the subtree rooted at this node
  enum Color color; // RED or BLACK
  struct Node* p; // parent
  struct Node* l; // left child
  struct Node* r; // right child
} Node;

// currently oridinary BST; TODO: change to Red-Black Tree later
typedef struct RBTree { // the BST uses Node.diff as the key
  Node* root;
  Node* nil_node;
} RBTree;

Node* find_minimum(Node* node);
void compute_sum(Node* node);
void compute_sum_fixup(Node* node); // compute sum towards root
void L_rotate(Node* x);
void R_rotate(Node* y);
void RB_insert_fixup(Node* z);
void insert(Node* z);
void RB_transplant(Node* u, Node* v);
void RB_delete_fixup(Node* x);
void delete_update_num_nodes(Node* z);
void delete(Node* z);
void update(Node* node, ll c, ll d, ll e, ll f);

Node dumplings[MAX_N];
RBTree bst;
ll P; // Alexandar's total cost for each day
int inorder_counter; // whether inorder_counter == 0 without explicit initialization?

int main(void) {
  int N, M; // N types of dumplings; M days (i.e., M-1 price changes)
  scanf("%d %d", &N, &M);

  // initialize nil_node
  Node nil_node = { .min_price = 0, .max_price = 0, .key = -1, .even_sum = 0, .odd_sum = 0, .num_nodes = 0, .color = BLACK, .p = NULL, .l = NULL, .r = NULL };
  bst.nil_node = &nil_node; bst.nil_node->p = &nil_node; bst.nil_node->l = &nil_node; bst.nil_node->r = &nil_node;
  bst.root = &nil_node;

  ll a, b;
  int i; // read initial prices of the dumplings
  for (i = 0; i < N; i++) {
    scanf("%lld %lld", &a, &b);
    dumplings[i].min_price = (a <= b) ? a : b;
    dumplings[i].max_price = (a <= b) ? b : a;
    dumplings[i].key = dumplings[i].min_price - dumplings[i].max_price; // the larger the difference, the lefter the node
    insert(&dumplings[i]);
  }
  
  // Day 1
  P = bst.root->even_sum; // inorder_sum();
  printf("%lld\n", P); P %= Q;

  // Day 2 to Day M
  int t;
  ll c, d, e, f;
  for (i = 1; i < M; i++) {
    scanf("%d %lld %lld %lld %lld", &t, &c, &d, &e, &f);
    update(&dumplings[t - 1], c, d, e, f);
    P = bst.root->even_sum; // inorder_sum();
    printf("%lld\n", P); P %= Q;
  }

  return 0;
}

Node* find_minimum(Node* node) {
  while (node->l != bst.nil_node) {
    node = node->l;
  }
  return node;
}

void compute_sum(Node* node) {
  if (node == bst.nil_node) return;
  node->even_sum = node->l->even_sum;
  node->odd_sum = node->l->odd_sum;
  if (node->l->num_nodes % 2 == 0) { // left subtree has even number of nodes
    node->even_sum += node->min_price + node->r->odd_sum;
    node->odd_sum += node->max_price + node->r->even_sum;
  } else { // left subtree has odd number of nodes
    node->even_sum += node->max_price + node->r->even_sum;
    node->odd_sum += node->min_price + node->r->odd_sum;
  }
}

void compute_sum_fixup(Node* node) {
  while (node != bst.nil_node) {
    compute_sum(node);
    node = node->p;
  }
}

void L_rotate(Node* x) {
  Node* y = x->r;
  x->r = y->l;
  x->num_nodes = x->l->num_nodes + x->r->num_nodes + 1; // update num_nodes
  if (y->l != bst.nil_node) y->l->p = x;
  y->p = x->p;
  if (x->p == bst.nil_node) {
    bst.root = y;
  } else if (x == x->p->l) {
    x->p->l = y;
  } else {
    x->p->r = y;
  }
  y->l = x;
  y->num_nodes = y->l->num_nodes + y->r->num_nodes + 1; // update num_nodes
  x->p = y;
  compute_sum_fixup(x); // update even_sum and odd_sum
}

void R_rotate(Node* y) {
  Node* x = y->l;
  y->l = x->r;
  y->num_nodes = y->l->num_nodes + y->r->num_nodes + 1; // update num_nodes
  if (x->r != bst.nil_node) x->r->p = y;
  x->p = y->p;
  if (y->p == bst.nil_node) {
    bst.root = x;
  } else if (y == y->p->l) {
    y->p->l = x;
  } else {
    y->p->r = x;
  }
  x->r = y;
  x->num_nodes = x->l->num_nodes + x->r->num_nodes + 1; // update num_nodes
  y->p = x;
  compute_sum_fixup(y); // update even_sum and odd_sum
}

void RB_insert_fixup(Node* z) {
  while (z->p->color == RED) { // violation of RED-RED rule
    Node* y;
    if (z->p == z->p->p->l) { // z->p is the left child of z->p->p (z->p->p must exist because z->p->color == RED)
      y = z->p->p->r; // y: the uncle of x
      if (y->color == RED) { // case 1: uncle is RED
        z->p->color = BLACK;
        y->color = BLACK;
        z->p->p->color = RED;
        z = z->p->p; // continue examining z->p->p
      } else { // case 2 or 3: uncle is BLACK
        if (z == z->p->r) { // case 2: z is the right child of z->p
          z = z->p;
          L_rotate(z);
        }
        z->p->color = BLACK;
        z->p->p->color = RED;
        R_rotate(z->p->p);
      }
    } else { // z->p is the right child of z->p->p
      y = z->p->p->l;
      if (y->color == RED) {
        z->p->color = BLACK;
        y->color = BLACK;
        z->p->p->color = RED;
        z = z->p->p;
      } else {
        if (z == z->p->l) {
          z = z->p;
          R_rotate(z);
        }
        z->p->color = BLACK;
        z->p->p->color = RED;
        L_rotate(z->p->p);
      }
    }
  }
  bst.root->color = BLACK;
}

void insert(Node* z) {
  Node* y = bst.nil_node; // y: the parent of x
  Node* x = bst.root; // x: for finding the position to insert z
  while (x != bst.nil_node) { // when x is nil -> it's time to insert z
    x->num_nodes++; // update the number of nodes in the subtree rooted at x
    y = x;
    if (z->key < x->key) {
      x = x->l;
    } else {
      x = x->r;
    }
  }
  z->p = y;
  if (y == bst.nil_node) {
    bst.root = z;
  } else if (z->key < y->key) {
    y->l = z;
  } else {
    y->r = z;
  }
  z->l = bst.nil_node;
  z->r = bst.nil_node;
  z->color = RED;
  z->num_nodes = 1; // z is a leaf
  compute_sum_fixup(z);
  RB_insert_fixup(z);
}

void RB_transplant(Node* u, Node* v) {
  if (u->p == bst.nil_node) { // if u is the root
    bst.root = v; // make v the new root
  } else if (u == u->p->l) {
    u->p->l = v;
  } else {
    u->p->r = v;
  }
  v->p = u->p; // if v is nil, it's ok (v->p will still be nil)
}

void RB_delete_fixup(Node* x) {
  while (x != bst.root && x->color == BLACK) {
    Node* w;
    if (x == x->p->l) { // x is the left child of x->p
      w = x->p->r; // w: the sibling of x
      if (w->color == RED) { // case 1: w is RED
        w->color = BLACK;
        x->p->color = RED;
        L_rotate(x->p);
        w = x->p->r;
      }
      if (w->l->color == BLACK && w->r->color == BLACK) { // case 2: w is BLACK and both children of w are BLACK
        w->color = RED;
        x = x->p;
      } else {
        if (w->r->color == BLACK) { // case 3: w is BLACK, w->l is RED, and w->r is BLACK
          w->l->color = BLACK;
          w->color = RED;
          R_rotate(w);
          w = x->p->r;
        }
        // case 4: w is BLACK, w->r is RED
        w->color = x->p->color;
        x->p->color = BLACK;
        w->r->color = BLACK;
        L_rotate(x->p);
        x = bst.root;
      }
    } else { // x is the right child of x->p
      w = x->p->l;
      if (w->color == RED) {
        w->color = BLACK;
        x->p->color = RED;
        R_rotate(x->p);
        w = x->p->l;
      }
      if (w->l->color == BLACK && w->r->color == BLACK) {
        w->color = RED;
        x = x->p;
      } else {
        if (w->l->color == BLACK) {
          w->r->color = BLACK;
          w->color = RED;
          L_rotate(w);
          w = x->p->l;
        }
        w->color = x->p->color;
        x->p->color = BLACK;
        w->l->color = BLACK;
        R_rotate(x->p);
        x = bst.root;
      }
    }
  }
  x->color = BLACK;
}

void delete_update_num_nodes(Node* z) {
  Node* x = z->p;
  while (x != bst.nil_node) {
    x->num_nodes--;
    x = x->p;
  }
}

void delete(Node* z) {
  Node* y = z;
  Node* x;
  enum Color y_original_color = y->color;
  if (z->l == bst.nil_node) {
    x = z->r;
    delete_update_num_nodes(z);
    RB_transplant(z, z->r);
  } else if (z->r == bst.nil_node) {
    x = z->l;
    delete_update_num_nodes(z);
    RB_transplant(z, z->l);
  } else {
    y = find_minimum(z->r);
    y_original_color = y->color;
    x = y->r;
    if (y->p == z) {
      delete_update_num_nodes(z);
      x->p = y;
    } else {
      delete_update_num_nodes(y);
      RB_transplant(y, y->r);
      y->r = z->r;
      y->r->p = y;
    }
    RB_transplant(z, y);
    y->l = z->l;
    y->l->p = y;
    y->color = z->color;
    y->num_nodes = y->l->num_nodes + y->r->num_nodes + 1;
  }
  compute_sum_fixup(x->p);
  if (y_original_color == BLACK) {
    RB_delete_fixup(x);
  }
}

void update(Node* node, ll c, ll d, ll e, ll f) {
  delete(node);
  // update prices
  ll a = (c * P + d) % Q;
  ll b = (e * P + f) % Q;
  node->min_price = (a <= b) ? a : b;
  node->max_price = (a <= b) ? b : a;
  node->key = node->min_price - node->max_price;
  insert(node);
}
