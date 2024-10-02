/*
 * This file is where you should implement your binary search tree.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name:Salem Demssie
 * Email:demssies@oregonstate.edu
 */

#include <stdlib.h>

#include "bst.h"
#include "stack.h"

/*
 * This structure represents a single node in a BST.  In addition to containing
 * pointers to its two child nodes (i.e. `left` and `right`), it contains two
 * fields representing the data stored at this node.  The `key` field is an
 * integer value that should be used as an identifier for the data in this
 * node.  Nodes in the BST should be ordered based on this `key` field.  The
 * `value` field stores data associated with the key.
 *
 * You should not modify this structure.
 */
struct bst_node {
  int key;
  void* value;
  struct bst_node* left;
  struct bst_node* right;
};


/*
 * This structure represents an entire BST.  It specifically contains a
 * reference to the root node of the tree.
 *
 * You should not modify this structure.
 */
struct bst {
  struct bst_node* root;
};

/*
 * This function should allocate and initialize a new, empty, BST and return
 * a pointer to it.
 */
struct bst* bst_create() {
    struct bst* new_bst = malloc(sizeof(struct bst));
    if (new_bst != NULL) {
 	new_bst->root = NULL;
    }
    return new_bst;
}

/*
 * This function should free the memory associated with a BST.  While this
 * function should up all memory used in the BST itself, it should not free
 * any memory allocated to the pointer values stored in the BST.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   bst - the BST to be destroyed.  May not be NULL.
 */
void bst_free(struct bst* bst) {
    if (bst == NULL) return;
    // Traverse the tree in post-order and free each node
    struct stack* nodeStack = stack_create();
    struct bst_node* current = bst->root;
    while (!stack_isempty(nodeStack) || current != NULL) {
        if (current != NULL) {
            stack_push(nodeStack, current);
            current = current->left;
        } else {
            current = stack_pop(nodeStack);
            struct bst_node* temp = current;
            current = current->right;
            free(temp);
        }
    }
    stack_free(nodeStack);
    free(bst);
}

/*
 * This function should return the total number of elements stored in a given
 * BST.
 *
 * Params:
 *   bst - the BST whose elements are to be counted.  May not be NULL.
 */
int bst_size(struct bst* bst) {
  if (bst == NULL || bst->root == NULL) return 0;
    int count = 0;
    struct stack* nodeStack = stack_create();
    struct bst_node* node = bst->root;

    while (!stack_isempty(nodeStack) || node != NULL) {
        if (node != NULL) {
            stack_push(nodeStack, node);
            node = node->left;
        } else {
            node = stack_pop(nodeStack);
            count++;
            node = node->right;
        }
    }

    stack_free(nodeStack);
    return count;
  
}

/*
 * This function should insert a new key/value pair into the BST.  The key
 * swhould be used to order the key/value pair with respect to the other data
 * stored in the BST.  The value should be stored along with the key, once the
 * right location in the tree is found.
 *
 * Params:
 *   bst - the BST into which a new key/value pair is to be inserted.  May not
 *     be NULL.
 *   key - an integer value that should be used to order the key/value pair
 *     being inserted with respect to the other data in the BST.
 *   value - the value being inserted into the BST.  This should be stored in
 *     the BST alongside the key.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void bst_insert(struct bst* bst, int key, void* value) {
   struct bst_node **node = &bst->root;

    while (*node != NULL) {
        if (key < (*node)->key) {
            node = &(*node)->left;
        } else {
            node = &(*node)->right;
        }
    }

    *node = malloc(sizeof(struct bst_node));
    if (*node != NULL) {
        (*node)->key = key;
        (*node)->value = value;
        (*node)->left = NULL;
        (*node)->right = NULL;
    }
}

/*
 * This function should remove a key/value pair with a specified key from a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should remove the first one it encounters (i.e. the one closest to
 * the root of the tree).
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair to be removed from the BST.
 */
void bst_remove(struct bst* bst, int key) {
    struct bst_node** node = &bst->root;
    while (*node != NULL) {
        if (key == (*node)->key) {
            if (!(*node)->left) {
                struct bst_node* temp = *node;
                *node = (*node)->right;
                free(temp);
            } else if (!(*node)->right) {
                struct bst_node* temp = *node;
                *node = (*node)->left;
                free(temp);
            } else {
                struct bst_node** succ = &(*node)->right;
                while ((*succ)->left) {
                    succ = &(*succ)->left;
                }
                (*node)->key = (*succ)->key;
                (*node)->value = (*succ)->value;
                struct bst_node* temp = *succ;
                *succ = (*succ)->right;
                free(temp);
            }
            return;
        } else if (key < (*node)->key) {
            node = &(*node)->left;
        } else {
            node = &(*node)->right;
        }
    }
}


/*
 * This function should return the value associated with a specified key in a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should return the first one it encounters (i.e. the one closest to
 * the root of the tree).  If the BST does not contain the specified key, this
 * function should return NULL.
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair whose value is to be returned.
 *
 * Return:
 *   Should return the value associated with the key `key` in `bst` or NULL,
 *   if the key `key` was not found in `bst`.
 */
void* bst_get(struct bst* bst, int key) {
  struct bst_node* current = bst->root;
    while (current) {
        if (key == current->key) {
            return current->value;
        } else if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NULL;
}

/*****************************************************************************
 **
 ** BST puzzle functions
 **
 *****************************************************************************/

/*
 * This function should return the height of a given BST, which is the maximum
 * depth of any node in the tree (i.e. the number of edges in the path from
 * the root to that node).  Note that the height of an empty tree is -1 by
 * convention.
 *
 * Params:
 *   bst - the BST whose height is to be computed
 *
 * Return:
 *   Should return the height of bst.
 */
int bst_height(struct bst* bst) {
    if (bst == NULL || bst->root == NULL) return -1;
    int max_height = -1, current_depth = 0;
    struct stack* stack = stack_create();
    struct bst_node* node = bst->root;
    while (!stack_isempty(stack) || node != NULL) {
        if (node != NULL) {
            stack_push(stack, node);
            node = node->left;
            current_depth++;
            if (current_depth > max_height) {
                max_height = current_depth;
            }
        } else {
            node = stack_pop(stack);
            node = node->right;
            current_depth--;
        }
    }
    stack_free(stack);
    return max_height;
}

/*
 * This function should determine whether a specified value is a valid path
 * sum within a given BST.  In other words, this function should check whether
 * the given BST contains any path from the root to a leaf in which the keys
 * sum to the specified value.
 *
 * Params:
 *   bst - the BST whose paths sums to search
 *   sum - the value to search for among the path sums of `bst`
 *
 * Return:
 *   Should return 1 if `bst` contains any path from the root to a leaf in
 *   which the keys add up to `sum`.  Should return 0 otherwise.
 */
int bst_path_sum(struct bst* bst, int sum) {
    if (bst == NULL || bst->root == NULL) return 0;

    struct stack* stack = stack_create();
    struct bst_node* node = bst->root;
    int current_sum = 0;

    // to handle the path sums correctly, we need to also keep track of the sums at each node.
    struct stack* sums_stack = stack_create();

    while (!stack_isempty(stack) || node != NULL) {
        while (node != NULL) {
            stack_push(stack, node);
            current_sum += node->key;
            stack_push(sums_stack, (void*)(long)current_sum);  // Cast current_sum to void* to store in stack

            // Move to the left child
            node = node->left;
        }

        if (!stack_isempty(stack)) {
            node = stack_pop(stack);
            current_sum = (int)(long)stack_pop(sums_stack);  // Retrieve the current sum when this node was pushed

            // Check if it's a leaf node and if the current path sum equals the target sum
            if (node->left == NULL && node->right == NULL && current_sum == sum) {
                stack_free(stack);
                stack_free(sums_stack);
                return 1;
            }

            // Move to the right child
            node = node->right;
        }
    }

    stack_free(stack);
    stack_free(sums_stack);
    return 0;
}

/*
 * This function should compute a range sum in a given BST.  Specifically, it
 * should compute the sum of all keys in the BST between a given lower bound
 * and a given upper bound.  For full credit, you should not process any subtree
 * whose keys cannot be included in the range sum.
 *
 * Params:
 *   bst - the BST within which to compute a range sum
 *   lower - the lower bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *   upper - the upper bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *
 * Return:
 *   Should return the sum of all keys in `bst` between `lower` and `upper`.
 */
int bst_range_sum(struct bst* bst, int lower, int upper) {
    int range_sum = 0;
    struct bst_node* node = bst->root;
    struct stack* stack = stack_create();
    while (!stack_isempty(stack) || node != NULL) {
        if (node != NULL) {
            stack_push(stack, node);
            node = node->left;
        } else {
            node = stack_pop(stack);
            if (node->key >= lower && node->key <= upper) {
                range_sum += node->key;
            }
            node = node->right;
        }
    }
    stack_free(stack);
    return range_sum;
  return 0;
}
/*
 * This function should compute a range sum in a given BST.  Specifically, it
 * should compute the sum of all keys in the BST between a given lower bound
 * and a given upper bound.  For full credit, you should not process any subtree
 * whose keys cannot be included in the range sum.
 *
 * Params:
 *   bst - the BST within which to compute a range sum
 *   lower - the lower bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *   upper - the upper bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *
 * Return:
 *   Should return the sum of a}
/*****************************************************************************
 **
 ** BST iterator definition (extra credit only)
 **
 *****************************************************************************/

/*
 * Structure used to represent a binary search tree iterator.  It contains
 * only a reference to a stack to be used to implement the iterator.
 *
 * You should not modify this structure.
 */
struct bst_iterator {
  struct stack* stack;
};

/*
 * This function should allocate and initialize an iterator over a specified
 * BST and return a pointer to that iterator.
 *
 * Params:
 *   bst - the BST for over which to create an iterator.  May not be NULL.
 */
struct bst_iterator* bst_iterator_create(struct bst* bst) {
  struct bst_iterator* iter = malloc(sizeof(struct bst_iterator));
  if (iter) {
        iter->stack = stack_create();
        struct bst_node* node = bst->root;
        while (node != NULL) {
            stack_push(iter->stack, node);
            node = node->left;
        }
    }
    return iter;
}

/*
 * This function should free all memory allocated to a given BST iterator.
 * It should NOT free any memory associated with the BST itself.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   iter - the BST iterator to be destroyed.  May not be NULL.
 */
void bst_iterator_free(struct bst_iterator* iter) {
  if (iter != NULL) {
       // Assuming that stack_free also frees all elements inside the stack.
       // If stack elements need explicit freeing, that would have to be handled here as well.
       stack_free(iter->stack);
       free(iter);
     }  
}
/*
 * This function should indicate whether a given BST iterator has more nodes
 * to visit.  It should specifically return 1 (true) if the iterator has at
 * least one more node to visit or 0 (false) if it does not have any more
 * nodes to visit.
 *
 * Param:
 *   iter - the BST iterator to be checked for remaining nodes to visit.  May
 *     not be NULL.
 */
int bst_iterator_has_next(struct bst_iterator* iter) {
  return !stack_isempty(iter->stack);  // Returns 1 if there is at least one node left 
}

/*
 * This function should return both the value and key associated with the
 * current node pointed to by the specified BST iterator and advnce the
 * iterator to point to the next node in the BST (in in-order order).
 *
 * Because a function can't return two things, the key associated with the
 * current node should be returned the normal way, while its value should be
 * returned via the argument `value`.  Specifically, the argument `value`
 * is a pointer to a void pointer.  The current BST node's value (a void
 * pointer) should be stored at the address represented by `value` (i.e. by
 * dereferencing `value`).  This will look something like this:
 *
 *   *value = current_node->value;
 *
 * Parameters:
 *   iter - BST iterator.  The key and value associated with this iterator's
 *     current node should be returned, and the iterator should be updated to
 *     point to the next node in the BST (in in-order order).  May not be NULL.
 *   value - pointer at which the current BST node's value should be stored
 *     before this function returns.
 *
 * Return:
 *   This function should return the key associated with the current BST node
 *   pointed to by `iter`.
 */
int bst_iterator_next(struct bst_iterator* iter, void** value) {
     if (iter == NULL || stack_isempty(iter->stack)) {
          *value = NULL;
          return 0; 
     }

     struct bst_node* node = stack_pop(iter->stack);
     *value = node->value;
     int key = node->key;
 
     // Push the right subtree of the node onto the stack
     node = node->right;
     while (node != NULL) {
         stack_push(iter->stack, node);
         node = node->left;
     }

     return key;  // Returning the key of the current node
} 
