#include "rbtree.h"

#include <stdlib.h>

////////////////////////////////////////////////////////////////
// utility and debuging functions

void print_RBTREE(rbtree* rbt) {

    node_t* nil = rbt->nil;

    node_t* node_stack[10000];
    int top = -1;

    node_stack[++top] = rbt->root;
    while(-1 < top) {

        node_t* current_node = node_stack[top--];
        if(current_node == nil)
            continue;

        node_stack[++top] = current_node->right;
        node_stack[++top] = current_node->left;

        // print key
        printf("key: %2d ", current_node->key);
        if(current_node->color == RBTREE_BLACK)
            printf("(BLACK),\t");
        else
            printf("(RED),  \t");

        // print parent
        if(current_node->parent == nil)
            printf("parent: nil\t");
        else 
            printf("parent: %d\t", current_node->parent->key);

        // print left
        if(current_node->left == nil)
            printf("left: nil ");
        else 
            printf("left: %2d ", current_node->left->key);
        
        if(current_node->left->color == RBTREE_BLACK)
            printf("(BLACK),\t");
        else 
            printf("(RED),  \t");
        
        // print right
        if(current_node->right == nil)
            printf("right: nil ");
        else 
            printf("right: %2d ", current_node->right->key);
        
        if(current_node->right->color == RBTREE_BLACK)
            printf("(BLACK)\n");
        else 
            printf("(RED)\n");
    }
    printf("\n");
}

int RBTREE_len(rbtree* rbt) {

    node_t* nil = rbt->nil;

    node_t* node_stack[10000];
    int top = -1;

    int count = 0;

    node_stack[++top] = rbt->root;
    while(-1 < top) {

        node_t* current_node = node_stack[top--];
        if(current_node == nil)
            continue;

        node_stack[++top] = current_node->right;
        node_stack[++top] = current_node->left;

        count++;
    }

    return count;
}

int check_red_property(rbtree* rbt) {

  node_t* nil = rbt->nil;

  node_t* node_stack[10000];
  int top = -1;

  node_stack[++top] = rbt->root;
  while(-1 < top) {

    node_t* current_node = node_stack[top--];
    if(current_node == nil)
        continue;

    node_stack[++top] = current_node->right;
    node_stack[++top] = current_node->left;

    if(current_node->color == RBTREE_RED) {
      if(current_node->parent->color == RBTREE_RED ||
        current_node->left->color == RBTREE_RED ||
        current_node->right->color == RBTREE_RED) {

        printf("red property error %d\n", current_node->key);
        print_RBTREE(rbt);
        int buf;
        scanf("%d", &buf);
        return 0;
      }
    }
  }
  return 1;
}

node_t* root_property(rbtree* t) {
  t->root->color = RBTREE_BLACK;
  t->root->parent = t->nil;
  return t->root;
}

void link_node(rbtree* t, node_t* parent_node, node_t* child_node) {

  child_node->parent = parent_node;

  if(parent_node == t->nil) {
    t->nil->left = t->nil->right = child_node;
    t->root = child_node;
    root_property(t);
  }

  else if(child_node->key == parent_node->key) {
    if(parent_node->left == t->nil)
      parent_node->left = child_node;
    else if(parent_node->right == t->nil)
      parent_node->right = child_node;
  }

  else if(child_node->key < parent_node->key) 
    parent_node->left = child_node;
  else
    parent_node->right = child_node;
}

node_t* find_uncle(node_t* child_node) {
  node_t* parent_node = child_node->parent;
  node_t* grand_node = parent_node->parent;
  if(grand_node->left == parent_node)
    return grand_node->right;
  else
    return grand_node->left;
}

node_t* find_sibling(node_t* child_node) {
  node_t* parent_node = child_node->parent;
  if(parent_node->left == child_node)
    return parent_node->right;
  else
    return parent_node->left;
}

node_t* left_rotate(rbtree* t, node_t* last_parent) {

  node_t* NIL = t->nil;
  node_t* link = last_parent->parent;

  node_t* right_child = last_parent->right;
  node_t* temp_subtree = right_child->left;

  link_node(t, right_child, last_parent); 
  NIL->key = last_parent->key + 1;
  link_node(t, last_parent, temp_subtree); 
  link_node(t, link, right_child); 

  return right_child;
}

node_t* right_rotate(rbtree* t, node_t* last_parent) {

  node_t* NIL = t->nil;
  node_t* link = last_parent->parent;

  node_t* left_child = last_parent->left;
  node_t* temp_subtree = left_child->right;

  link_node(t, left_child, last_parent);
  NIL->key = last_parent->key - 1;
  link_node(t, last_parent, temp_subtree);
  link_node(t, link, left_child);

  return left_child;
}

void red_property(rbtree* t, node_t* red_child) {

  // check if red_child is root node
  if(t->root == red_child) {
    root_property(t);
    return;
  }

  node_t* NIL = t->nil;
  node_t* parent_node = red_child->parent;
  node_t* grand_node = parent_node->parent;
  node_t* uncle_node = find_uncle(red_child);

  // check red property is broken
  if(red_child->parent->color == RBTREE_BLACK)
    return;

  // if red property is broken
  else {

    // if uncle node is red
    if(uncle_node->color == RBTREE_RED) {
      parent_node->color = uncle_node->color = RBTREE_BLACK;
      grand_node->color = RBTREE_RED;
      red_property(t, grand_node);
    }

    // if uncle node is black
    else if(uncle_node->color == RBTREE_BLACK) {
      grand_node->color = RBTREE_RED;

      if(grand_node->left == parent_node) {
        if(parent_node->right == red_child)
          parent_node = left_rotate(t, parent_node);

        parent_node->color = RBTREE_BLACK;
        right_rotate(t, grand_node);
      }
      else {
        if(parent_node->left == red_child)
          parent_node = right_rotate(t, parent_node);

        parent_node->color = RBTREE_BLACK;
        left_rotate(t, grand_node);
      }
    }
  }

  return;
}

void black_property(rbtree* t, node_t* extra_black) {
  // if(extra_black == t->nil)
  //   printf("black property nil\n");
  // else
  //   printf("black property %d\n", extra_black->key);
  node_t* NIL = t->nil;

  // if extra black is on red node
  if(extra_black->color == RBTREE_RED) {
    extra_black->color = RBTREE_BLACK;
    return;
  }

  // if extra black is on root node
  if(t->root == extra_black)
    return;

  // doubly black
  node_t* parent_node = extra_black->parent;
  node_t* sibling_node = find_sibling(extra_black);

  // if sibling is red
  if(sibling_node->color == RBTREE_RED) {
    parent_node->color = RBTREE_RED;
    sibling_node->color = RBTREE_BLACK;
    if(parent_node->left == extra_black)
      left_rotate(t, parent_node);
    else
      right_rotate(t, parent_node);
  }
  
  // if sibling is black
  else {
    // if sibling has only black child
    if(sibling_node->left->color == RBTREE_BLACK && sibling_node->right->color == RBTREE_BLACK) {
      sibling_node->color = RBTREE_RED;
      black_property(t, parent_node);
    }
    else {
      if(parent_node->left == sibling_node){
        if(sibling_node->left->color != RBTREE_RED) {
          sibling_node->color = RBTREE_RED;
          sibling_node->right->color = RBTREE_BLACK;
          sibling_node = left_rotate(t, sibling_node);
        }

        sibling_node->left->color = RBTREE_BLACK;
        if(parent_node->color == RBTREE_RED) {
          parent_node->color = RBTREE_BLACK;
          sibling_node->color = RBTREE_RED;
        }

        right_rotate(t, parent_node);
      }
      else if(parent_node->right == sibling_node) {
        if(sibling_node->right->color != RBTREE_RED) {
          sibling_node->color = RBTREE_RED;
          sibling_node->left->color = RBTREE_BLACK;
          sibling_node = right_rotate(t, sibling_node);
        }

        sibling_node->right->color = RBTREE_BLACK;
        if(parent_node->color == RBTREE_RED) {
          parent_node->color = RBTREE_BLACK;
          sibling_node->color = RBTREE_RED;
        }

        left_rotate(t, parent_node);
      }
    }
  }

  return;
}

////////////////////////////////////////////////////////////////

rbtree *new_rbtree(void) {

  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t* nil = (node_t*)malloc(sizeof(node_t));
  p->nil = nil;

  p->root = p->nil;
  p->nil = p->root;
  p->nil->color = RBTREE_BLACK;
  return p;
}

void delete_rbtree(rbtree *t) {
  
  node_t* NIL = t->nil;
  node_t* current_node = t->root;

  while(current_node != NIL) {
    node_t* temp_node;
    if(current_node->left == NIL && current_node->right == NIL) {
      temp_node = current_node->parent;
      // printf("free %d\n", current_node->key);
      free(current_node);
    }
    else {
      if(current_node->left != NIL) {
        temp_node = current_node->left;
        current_node->left = NIL;
      }
      else {
        temp_node = current_node->right;
        current_node->right = NIL;
      }
    }

    current_node = temp_node;
  }

  free(NIL);
  free(t);

  return;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  
  node_t* NIL = t->nil;

  // insert node
  node_t* insert_node = (node_t*)malloc(sizeof(node_t));
  insert_node->color = RBTREE_RED;
  insert_node->key = key;
  insert_node->parent = insert_node->left = insert_node->right = NIL;

  // if rbtree is empty
  if(t->root == NIL)
    t->root = insert_node;

  else {

    // find parent of insert_node
    node_t* parent_node = NIL;
    node_t* check_node = t->root;
    while(check_node != NIL) {
      parent_node = check_node;
      if(key < check_node->key)
        check_node = check_node->left;
      else
        check_node = check_node->right;
    }

    // insert
    link_node(t, parent_node, insert_node);

    // check and fix red property
    red_property(t, insert_node);
  }

  check_red_property(t);

  root_property(t);
  
  return insert_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  
  node_t* NIL = t->nil;
  node_t* current_node = t->root;

  while(current_node != NIL) {

    if(current_node->key == key)
      return current_node;
    
    if(key < current_node->key)
      current_node = current_node->left;
    else
      current_node = current_node->right;
  }

  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find

  node_t* nil = t->nil;
  node_t* current_node = t->root;
  
  while(current_node->left != nil)
    current_node = current_node->left;

  return current_node;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t* nil = t->nil;
  node_t* current_node = t->root;
  
  while(current_node->right != nil)
    current_node = current_node->right;

  return current_node;
}

int rbtree_erase(rbtree *t, node_t *p) {

  node_t* NIL = t->nil;
  const key_t key = p->key;

  // find p in rbtree
  node_t* current_node = t->root;
  while(current_node != NIL)
    if(current_node == p)
      break;
    else if(key < current_node->key)
      current_node = current_node->left;
    else
      current_node = current_node->right;
    
    // if p is not in rbtree
    if(current_node == NIL)
      return 0;
    
    // find predecessor
    node_t* predecessor = current_node;
    node_t* temp_node = current_node->left;

    node_t* parent_node = predecessor->parent;
    node_t* replace_node = predecessor->right; 

    // if p is not predecessor
    if(temp_node == NIL) {

      // if p is root node (p is root and has left child)
      if(p == t->root) {
        link_node(t, parent_node, p->right);
        replace_node = p->right;
      }

      else if(replace_node != NIL)
        link_node(t, parent_node, replace_node);
      else {
        if(parent_node->left == predecessor)
          parent_node->left = NIL;
        else
          parent_node->right = NIL;
        NIL->parent = parent_node;
      }
    }

    else{

      while(temp_node != NIL) {
        predecessor = temp_node;
        temp_node = temp_node->right;
      }

      // swap key with predecessor
      key_t temp_key = p->key;
      p->key = predecessor->key;
      predecessor->key = temp_key;

      parent_node = predecessor->parent;
      replace_node = predecessor->left;

      if(replace_node != NIL)
        link_node(t, predecessor->parent, predecessor->left);
      else {
        if(parent_node->left == predecessor)
          parent_node->left = NIL;
        else
          parent_node->right = NIL;
        NIL->parent = parent_node;
      }
    }

    // check and fix black property
    if(predecessor->color == RBTREE_BLACK)
      black_property(t, replace_node);

    free(predecessor);

    return 1;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {

  node_t* nil = t->nil;

  // if rbtree is empty
  if(t->root == nil)
    return 0;

  // calculate maximum depth
  node_t* current_node = t->root;
  int black_num = 0;
  while(current_node != nil) {
    if(current_node->color == RBTREE_BLACK)
      black_num++;
    current_node = current_node->left;
  }

  int max_depth = black_num * 2;

  // DFS
  node_t* node_stack = (node_t*)malloc(sizeof(node_t) * (max_depth * 2));
  int top = -1;
  node_stack[++top] = *(t->root);
  
  int arr_idx = 0;
  while(0 <= top && arr_idx < n) {
    node_t current_node = node_stack[top--];

    if(current_node.left == nil && current_node.right == nil)
      arr[arr_idx++] = current_node.key;
    
    else {
      if(current_node.right != nil)
        node_stack[++top] = *(current_node.right);
      node_t* temp_left = current_node.left;
      current_node.left = current_node.right = nil;
      node_stack[++top] = current_node;
      if(temp_left != nil)
        node_stack[++top] = *temp_left;
    }
  }

  free(node_stack);

  return 0;
}
