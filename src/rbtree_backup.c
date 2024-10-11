#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p->root = p->nil;
  p->nil = p->root;
  p->nil->color = RBTREE_BLACK;
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory

  const node_t* nil = t->nil;

  node_t* parent_node;
  node_t* current_node = t->root;

  // set current_node as leaf node
  while(current_node->left != nil)
    current_node = current_node->left;
  
  // free node before current_node is root
  while(current_node != t->root) {

    if(current_node->left != nil)
      current_node = current_node->left;

    else if(current_node->right != nil)
      current_node = current_node->right;
    
    else {  // if current_node has no child
      node_t* child_node = current_node;
      current_node = current_node->parent;
      if(current_node->left == child_node)
        current_node->left = nil;
      else
        current_node->right = nil;
      free(child_node);
    }
  }
  
  // free root node
  free(current_node);

  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  
  node_t* nil = t->nil;

  // find insert place
  node_t* current_node = t->root;
  node_t* parent_node = &(t->root);

  while(current_node != nil)
    if(key < current_node->key) {
      parent_node = current_node;
      current_node = current_node->left;
    }
    else {
      parent_node = current_node;
      current_node = current_node->right;
    }
  
  // make insert node and insert in rbtree
  node_t* insert_node = (node_t*)malloc(sizeof(node_t));
  insert_node->color = RBTREE_RED;
  insert_node->key = key;
  insert_node->parent = parent_node;
  insert_node->left = insert_node->right = nil;

  if(key < parent_node->key)
    parent_node->left = insert_node;
  else
    parent_node->right = insert_node;
  
  // fix up rbtree
  current_node = insert_node;
  while(current_node->color == RBTREE_RED && t->root != current_node->parent) {

    node_t* grand_parent_node = current_node->parent->parent;
    node_t* parent_node = current_node->parent;
    node_t* uncle_node;
    if(parent_node == grand_parent_node->left)
      uncle_node = current_node->parent->parent->right;
    else
      uncle_node = current_node->parent->parent->left;
    
    // case that uncle is red
    if(uncle_node->color == RBTREE_RED) {
      parent_node->color = uncle_node->color = RBTREE_BLACK;
      grand_parent_node->color = RBTREE_RED;
      current_node = grand_parent_node;
    }

    // case that uncle is black
    else {

      // lr, rl case
      if(grand_parent_node->left == parent_node && parent_node->right == current_node) {
        grand_parent_node->left = current_node;
        current_node->parent = grand_parent_node;

        parent_node->right = current_node->left;
        current_node->left->parent = parent_node;

        current_node->left = parent_node;
        parent_node->parent = current_node;

        current_node = parent_node;
      }
      else if(grand_parent_node->right == parent_node && parent_node->left == current_node) {
        grand_parent_node->right = current_node;
        current_node->parent = grand_parent_node;

        parent_node->left = current_node->right;
        current_node->right->parent = parent_node;

        current_node->right = parent_node;
        parent_node->parent = current_node;

        current_node = parent_node;
      }

      // ll, lr case
      else if(grand_parent_node->left == parent_node && parent_node->left == current_node) {

        if(grand_parent_node == t->root) {
          parent_node->parent = nil;
          t->root = parent_node;
        }
        else {
          node_t* grand_grand_parent_node = grand_parent_node->parent;
          if(grand_grand_parent_node->left == grand_parent_node)
            grand_grand_parent_node->left = parent_node;
          else
            grand_grand_parent_node->right = parent_node;
          parent_node->parent = grand_grand_parent_node;
        }

        grand_parent_node->left = parent_node->right;
        parent_node->right->parent = grand_parent_node;

        parent_node->right = grand_parent_node;
        grand_parent_node->parent = parent_node;

        parent_node->color = RBTREE_BLACK;
        grand_parent_node->color = RBTREE_RED;
      }
      else if(grand_parent_node->right == parent_node && parent_node->right == current_node) {

        if(grand_parent_node == t->root) {
          parent_node->parent = nil;
          t->root = parent_node;
        }
        else {
          node_t* grand_grand_parent_node = grand_parent_node->parent;
          if(grand_grand_parent_node->left == grand_parent_node)
            grand_grand_parent_node->left = parent_node;
          else
            grand_grand_parent_node->right = parent_node;
          parent_node->parent = grand_grand_parent_node;
        }

        grand_parent_node->right = parent_node->left;
        parent_node->left->parent = grand_parent_node;

        parent_node->left = grand_parent_node;
        grand_parent_node->parent = parent_node;

        parent_node->color = RBTREE_BLACK;
        grand_parent_node->color = RBTREE_RED;
      }
    }
  }

  t->root->color = RBTREE_BLACK;

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  
  node_t* nil = t->nil;

  node_t* current_node = t->root;
  while(current_node != nil) {

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
  if(current_node == nil)
    return NULL;
  
  while(current_node->left != nil)
    current_node = current_node->left;

  return current_node;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t* nil = t->nil;
  node_t* current_node = t->root;
  if(current_node == nil)
    return NULL;
  
  while(current_node->right != nil)
    current_node = current_node->right;

  return current_node;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase

  node_t* nil = t->nil;
  key_t key = p->key;

  // find delete node
  node_t* current_node = t->root;
  while(t != nil && current_node->key != key) {
    if(key < current_node->key)
      current_node = current_node->left;
    else
      current_node = current_node->right;
  }

  // if p is not in rbtree
  if(current_node == nil)
    return -1;
  
  node_t* delete_node;
  node_t* temp_p = current_node;
  if(current_node->left == nil && current_node->right == nil)
    delete_node = current_node;
  
  else if(current_node->left != nil) {
    node_t* parent_node = current_node;
    current_node = current_node->left;
    while(current_node != nil) {
      parent_node = current_node;
      current_node = current_node->right;
    }
    delete_node = current_node;
    key_t temp = delete_node->key;
    delete_node->key = temp_p->key;
    temp_p->key = temp;
  }

  else if(current_node->right != nil) {
    node_t* parent_node = current_node;
    current_node = current_node->right;
    while(current_node != nil) {
      parent_node = current_node;
      current_node = current_node->left;
    }
    delete_node = current_node;
    key_t temp = delete_node->key;
    delete_node->key = temp_p->key;
    temp_p->key = temp;
  }

  // double black
  if(delete_node->color == RBTREE_BLACK) {
    
  }

  free(delete_node);

  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array

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
