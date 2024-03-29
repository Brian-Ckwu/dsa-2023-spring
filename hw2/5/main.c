/*
Pseudocode:

read N (number of cats), M (number of questioning / swapping / magic step)

// construct a global BST
allocate a size-N node array a
gbst = new BST()
for i = 0 to N-1:
  read appetite to a[i]
  node = new Node(a[i])
  gbst.insert(node)

// construct BSTs of different colors
cbst = new BST()
for i = 0 to N-1:
  c = read color
  cnode = new Node(c)
  cbst.insert(cnode) // do nothing if the color node is already present

  node = new Node(a[i])
  cnode = cbst.find(c) // how about making cbst.insert return the inserted/found node to reduce time?
  cnode.bst.insert(node)

// start questioning / swapping / magic step
for i = 1 to M: // read each line
  op = read opeartion type
  if (op == QUESTION):
    c, l, r = read color, lower, upper
    cnode = cbst.find(c)
    num = 0
    if (cnode.key == c) { // else -> no such color -> output 0 directly
      lnode = cnode.bst.find(l)
      if (lnode.key < l) {
        num = num - 1
      }
      rnode = cnode.bst.find(r)
      if (rnode.key > r) {
        num = num - 1
      }
      num = num + (rnode.index - lnode.index + 1)
      num = MAX(0, num)
    }
    output num
  else if (op == SWAPPING):
    k = read greedy cat index
    ...
  else if (op == MAGIC):
    ...
  else
    raise ERROR
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define QUESTION 1
#define SWAPPING 2
#define MAGIC 3

#define SELECT_HIGH 1
#define SELECT_LOW 0
#define SUCCESS 1
#define FAILURE 0

typedef struct Node Node;

// An AVL tree
typedef struct AVLTree {
  Node* root;
} AVLTree;

// An AVL tree node
typedef struct Node {
  int i; // node index in the storing array
  int key;
  struct Node* parent; // TODO
  struct Node* left;
  struct Node* right;
  int height;
  int index;
  int num_nodes; // number of nodes of the subtree rooted at this Node
  AVLTree bst;
} Node;

void print_bst(Node* node, int level);
void set_new_node(Node* node, int i, int key);

int max(int a, int b) {
  return (a > b) ? a : b;
}

int abs(int a) {
  return (a > 0) ? a : -a;
}

int height(Node* node) {
  if (node == NULL) {
      return 0;
  }
  return node->height;
}

int num_nodes(Node* node) {
  if (node == NULL) {
      return 0;
  }
  return node->num_nodes;
}

void free_tree(Node* node) {
  if (node == NULL) {
    return;
  }
  free_tree(node->left);
  free_tree(node->right);
  free(node);
}

Node* find_min_descendent(Node* node) {
  Node* cur = node;
  while (cur->left != NULL) {
    cur = cur->left;
  }
  return cur;
}

Node* find_max_descendent(Node* node) {
  Node* cur = node;
  while (cur->right != NULL) {
    cur = cur->right;
  }
  return cur;
}

// AVL tree rotations
Node* rotate_right(AVLTree* tree, Node* node) {
  Node* left = node->left;
  Node* left_right = left->right;
  left->parent = node->parent;
  if (node->parent) {
    if (node->parent->left && node->parent->left->key == node->key) {
      node->parent->left = left;
    } else {
      node->parent->right = left;
    }
  }
  left->right = node; node->parent = left;
  node->left = left_right; if (left_right) left_right->parent = node;
  node->height = max(height(node->left), height(node->right)) + 1;
  left->height = max(height(left->left), height(left->right)) + 1;
  node->num_nodes = num_nodes(node->left) + num_nodes(node->right) + 1;
  left->num_nodes = num_nodes(left->left) + num_nodes(left->right) + 1;
  // update root if necessary
  if (tree->root->key == node->key) {
    tree->root = left;
  }
  return left;
}

Node* rotate_left(AVLTree* tree, Node* node) {
  Node* right = node->right;
  Node* right_left = right->left;
  right->parent = node->parent;
  if (node->parent) {
    if (node->parent->left && node->parent->left->key == node->key) {
      node->parent->left = right;
    } else {
      node->parent->right = right;
    }
  }
  right->left = node; node->parent = right;
  node->right = right_left; if (right_left) right_left->parent = node;
  node->height = max(height(node->left), height(node->right)) + 1;
  right->height = max(height(right->left), height(right->right)) + 1;
  node->num_nodes = num_nodes(node->left) + num_nodes(node->right) + 1;
  right->num_nodes = num_nodes(right->left) + num_nodes(right->right) + 1;
  // update root if necessary
  if (tree->root->key == node->key) {
    tree->root = right;
  }
  return right;
}

Node* walk_to_root(Node* node) {
  while (node->parent) {
    node = node->parent;
  }
  return node;
}

void update_height(AVLTree* tree, Node* node) {
  if (node == NULL) return;
  while (node->parent) {
    node = node->parent;
    node->height = max(height(node->left), height(node->right)) + 1;
    if (abs(height(node->left) - height(node->right)) > 1) {
      // rotate to balance
      if (height(node->left) > height(node->right)) {
        if (height(node->left->left) >= height(node->left->right)) {
          node = rotate_right(tree, node);
        } else {
          rotate_left(tree, node->left);
          node = rotate_right(tree, node);
        }
      } else {
        if (height(node->right->right) >= height(node->right->left)) {
          node = rotate_left(tree, node);
        } else {
          rotate_right(tree, node->right);
          node = rotate_left(tree, node);
        }
      }
    }
  }
}

void insert(AVLTree* tree, Node* node) {
  if (tree->root == NULL) {
    tree->root = node;
  } else {
    Node* cur = tree->root;
    while (1) {
      if (node->key < cur->key) {
        cur->num_nodes += 1;
        if (cur->left == NULL) {
          cur->left = node;
          node->parent = cur;
          update_height(tree, node);
          break;
        } else {
          cur = cur->left;
        }
      } else if (node->key > cur->key) {
        cur->num_nodes += 1;
        if (cur->right == NULL) {
          cur->right = node;
          node->parent = cur;
          update_height(tree, node);
          break;
        } else {
          cur = cur->right;
        }
      } else { // equal -> do nothing
        // printf("Duplicated node: key = %d\n", node->key); // XXX: remove this debugging line
        break;
      }
    }
  }
}

void remove_node(AVLTree* tree, Node* node) { // printf("Removing node %d...\n", node->key);
  Node* cur = tree->root;
  while (node->key != cur->key) {
    if (node->key < cur->key) {
      cur->num_nodes -= 1;
      cur = cur->left;
    } else if (node->key > cur->key) {
      cur->num_nodes -= 1;
      cur = cur->right;
    }
  }
  Node dummy = {INT_MIN, INT_MIN, NULL, tree->root, NULL, 0, 0, 0, {NULL}};
  Node* parent = &dummy;
  if (cur->parent) parent = cur->parent;
    
  // remove the node
  if (cur->left && cur->right) { // printf("Case 1: node %d has two children\n", cur->key);
    Node* succ = find_min_descendent(cur->right);
    Node* succ_parent = succ->parent;
    // connect succ's parent with succ's right child if necessary
    if (succ->parent->key != cur->key) {
      succ->parent->left = succ->right;
      if (succ->right) succ->right->parent = succ->parent;
      // update_height(tree, succ);
    }
    // reset the number of nodes from succ up to cur
    Node* tmp = succ;
    do {
      tmp->parent->num_nodes -= 1;
      tmp = tmp->parent;
    } while (tmp->key != cur->key);
    // replace cur with succ
    succ->parent = (parent->key != INT_MIN) ? parent : NULL;
    succ->left = cur->left; cur->left->parent = succ;
    succ->right = (cur->right->key != succ->key) ? cur->right : succ->right; if (cur->right->key != succ->key) cur->right->parent = succ;
    succ->height = max(height(succ->left), height(succ->right)) + 1;
    succ->num_nodes = cur->num_nodes;
    if (parent->left && parent->left->key == cur->key) {
      parent->left = succ;
    } else if (parent->right && parent->right->key == cur->key) {
      parent->right = succ;
    } else { // XXX: remove this debugging line
      printf("ERROR: remove() 1: parent->left->key = %d, parent->right->key = %d, cur->key = %d\n", parent->left->key, parent->right->key, cur->key);
    }
    succ_parent->height = max(height(succ_parent->left), height(succ_parent->right)) + 1;
    update_height(tree, succ_parent);
  } else if (cur->left) { // printf("Case 2: node %d has one left child\n", cur->key);
    cur->left->parent = (parent->key != INT_MIN) ? parent : NULL;
    if (parent->left && parent->left->key == cur->key) {
      parent->left = cur->left;
    } else if (parent->right && parent->right->key == cur->key) {
      parent->right = cur->left;
    } else { // XXX: remove this debugging line
      printf("ERROR: remove() 2: parent->left->key = %d, parent->right->key = %d, cur->key = %d\n", parent->left->key, parent->right->key, cur->key);
    }
    update_height(tree, cur);
  } else if (cur->right) { // printf("Case 3: node %d has one right child\n", cur->key);
    cur->right->parent = (parent->key != INT_MIN) ? parent : NULL;
    if (parent->left && parent->left->key == cur->key) {
      parent->left = cur->right;
    } else if (parent->right && parent->right->key == cur->key) {
      parent->right = cur->right;
    } else { // XXX: remove this debugging line
      printf("ERROR: remove() 3: parent->left->key = %d, parent->right->key = %d, cur->key = %d\n", parent->left->key, parent->right->key, cur->key);
    }
    update_height(tree, cur);
  } else { // printf("Case 4: node %d has no child\n", cur->key);
    if (parent->left && parent->left->key == cur->key) {
      parent->left = NULL;
    } else if (parent->right && parent->right->key == cur->key) {
      parent->right = NULL;
    } else { // XXX: remove this debugging line
      printf("ERROR: remove() 4: parent->left->key = %d, parent->right->key = %d, cur->key = %d\n", parent->left->key, parent->right->key, cur->key);
    }
    update_height(tree, cur);
  } // printf("Done removing node %d\n", node->key);

  // reset root if necessary
  if (parent->key == INT_MIN) { tree->root = parent->left ? walk_to_root(parent->left) : NULL; }

  // reset cur
  // cur->parent = NULL;
  // cur->left = NULL;
  // cur->right = NULL;
}

Node* find(AVLTree* tree, int key) {
  Node* cur = tree->root;
  Node* prev = NULL;

  cur->index = 0;
  if (cur->left) cur->index += cur->left->num_nodes;

  while (cur != NULL) {
    prev = cur;
    if (key < cur->key) {
      cur = cur->left;
      if (cur) cur->index = prev->index - 1;
      if (cur && cur->right) cur->index -= cur->right->num_nodes;
    } else if (key > cur->key) {
      cur = cur->right;
      if (cur) cur->index = prev->index + 1;
      if (cur && cur->left) cur->index += cur->left->num_nodes;
    } else {
      return cur;
    }
  }
  return prev; // return parent of cur if cur is NULL
}

Node* find_inorder_successor(AVLTree* tree, Node* node) {
  if (node->right) return find_min_descendent(node->right);
  Node* succ = NULL;
  Node* cur = tree->root;
  while (cur->key != node->key) {
    if (node->key < cur->key) {
      succ = cur;
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }
  return succ;
}

void set_new_node(Node* node, int i, int key) {
  node->i = i;
  node->key = key;
  node->parent = NULL;
  node->left = NULL;
  node->right = NULL;
  node->height = 1;
  node->num_nodes = 1;
}

void swap_keys(Node* a, Node* b) {
  int tmp = a->key;
  a->key = b->key;
  b->key = tmp;
}

// print the tree representation in pre-order with connected lines
void print_bst(Node* node, int level) {
  if (node == NULL) {
    return;
  }
  int i;
  for (i = 0; i < level; i++) {
    printf("  ");
  }
  printf("%d (num_nodes: %d, height: %d)", node->key, node->num_nodes, node->height);
  if (node->bst.root) {
    printf(" BST of color %d\n", node->key);
    print_bst(node->bst.root, level + 1);
  }
  printf("\n");
  print_bst(node->left, level + 1);
  print_bst(node->right, level + 1);
}

void print_keys(Node* nodes, int N) {
  printf("Keys: ");
  int i;
  for (i = 0; i < N; i++) {
    printf("%d ", nodes[i].key);
  }
  printf("\n");
}

int main(void) {
  int N, M;
  scanf("%d %d", &N, &M);

  // Construct a global tree
  Node* nodes = (Node*) calloc(N, sizeof(Node));
  AVLTree gbst = {NULL}; // Global BST
  int i;
  for (i = 0; i < N; i++) {
    int a;
    scanf("%d", &a);
    set_new_node(&nodes[i], i, a);
    insert(&gbst, &nodes[i]);
  }

  // Construct trees by their colors
  Node* cnodes = (Node*) calloc(N, sizeof(Node));
  Node* nodes_2 = (Node*) calloc(N, sizeof(Node));
  AVLTree cbst = {NULL};
  for (i = 0; i < N; i++) {
    int c;
    scanf("%d", &c);
    set_new_node(&cnodes[i], i, c);
    insert(&cbst, &cnodes[i]);

    set_new_node(&nodes_2[i], i, nodes[i].key); // nodes[i].key == a
    // Find the color node
    Node* cnode = find(&cbst, c);
    insert(&cnode->bst, &nodes_2[i]);
  }

  // // XXX: for debugging
  // printf("After initial insertion:\n");
  // print_keys(nodes, N);
  // print_keys(nodes_2, N);
  // print_bst(gbst.root, 0);
  // print_bst(cbst.root, 0);

  for (i = 1; i <= M; i++) { // printf("Case #%d:\n", i);
    int op;
    scanf("%d", &op);
    if (op == QUESTION) {
      int c, l, r;
      scanf("%d %d %d", &c, &l, &r);
      Node* cnode = find(&cbst, c);
      int num = 0;
      if (cnode->key == c) {
        Node* lnode = find(&cnode->bst, l);
        Node* rnode = find(&cnode->bst, r);
        if (lnode->key < l) num--;
        if (rnode->key > r) num--;
        num += (rnode->index - lnode->index + 1);
        num = max(num, 0);
      }
      printf("%d\n", num);
    } else if (op == SWAPPING) {
      int k;
      scanf("%d", &k);
      Node* grdy = &nodes[k]; // print_bst(gbst.root, 0); printf("Trying to swap %d\n", grdy->key);
      Node* succ = find_inorder_successor(&gbst, grdy); //printf("Done finding inorder successor\n");
      Node* grdy_cnode = find(&cbst, cnodes[grdy->i].key); //printf("Done finding color node of grdy\n");
      Node* succ_cnode = find(&cbst, cnodes[succ->i].key); //printf("Done finding color node of succ\n");
      if (succ) { // printf("Swapping %d and %d\n", grdy->key, succ->key);
        // remove grdy and succ from their BSTs
        remove_node(&gbst, grdy); remove_node(&gbst, succ);
        remove_node(&grdy_cnode->bst, &nodes_2[grdy->i]); remove_node(&succ_cnode->bst, &nodes_2[succ->i]);
        // swap keys in nodes and nodes_2
        swap_keys(&nodes[grdy->i], &nodes[succ->i]);
        swap_keys(&nodes_2[grdy->i], &nodes_2[succ->i]);
        // insert grdy and succ in their BSTs
        set_new_node(grdy, grdy->i, nodes[grdy->i].key); insert(&gbst, grdy);
        set_new_node(succ, succ->i, nodes[succ->i].key); insert(&gbst, succ);
        set_new_node(&nodes_2[grdy->i], grdy->i, nodes[grdy->i].key); insert(&grdy_cnode->bst, &nodes_2[grdy->i]);
        set_new_node(&nodes_2[succ->i], succ->i, nodes[succ->i].key); insert(&succ_cnode->bst, &nodes_2[succ->i]);
      }
    } else if (op == MAGIC) {
      int c, s, t;
      scanf("%d %d %d", &c, &s, &t);
      Node* cnode = find(&cbst, c);
      Node* magic_cat = NULL;
      int appetite;
      if (s == SELECT_HIGH) {
        magic_cat = find_max_descendent(cnode->bst.root);
      } else if (s == SELECT_LOW) {
        magic_cat = find_min_descendent(cnode->bst.root);
      }
      if (t == SUCCESS) {
        appetite = find_max_descendent(gbst.root)->key + 1;
      } else if (t == FAILURE) {
        appetite = find_min_descendent(gbst.root)->key - 1;
      }
      // modify the magic_cat in both BSTs
      remove_node(&gbst, &nodes[magic_cat->i]); remove_node(&cnode->bst, magic_cat);
      set_new_node(&nodes[magic_cat->i], magic_cat->i, appetite); insert(&gbst, &nodes[magic_cat->i]);
      set_new_node(magic_cat, magic_cat->i, appetite); insert(&cnode->bst, magic_cat);
    } else {
      printf("ERROR: unknown operation %d\n", op);
    }

    // printf("After step %d:\n", i);
    // print_keys(nodes, N);
    // print_keys(nodes_2, N);
    // print_bst(gbst.root, 0);
    // print_bst(cbst.root, 0);
  }

  // // XXX: for debugging
  // print_keys(nodes, N);
  // print_keys(nodes_2, N);
  // print_bst(gbst.root, 0);
  // print_bst(cbst.root, 0);

  // free memory
  free(nodes);
  free(cnodes);
  free(nodes_2);

  return 0;
}
// 8th attempt
