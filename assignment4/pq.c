/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Salem Demssie
 * Email: demssies@oregonstate.edu
 */

#include <stdlib.h>
#include <assert.h>

#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq_node {
    void* value;
    int priority;
};

struct pq {
    struct dynarray* data;
};

/*
 * Helper function to swap two pq_node pointers.
 */
static void swap(struct pq_node** a, struct pq_node** b) {
    struct pq_node* temp = *a;
    *a = *b;
    *b = temp;
}

/*
 * Helper function to maintain the heap property from a given node up to the root.
 */
static void heapify_up(struct pq* pq, int idx) {
    while (idx > 0) {
        int parent_idx = (idx - 1) / 2;
        struct pq_node* parent = dynarray_get(pq->data, parent_idx);
        struct pq_node* current = dynarray_get(pq->data, idx);
        if (current->priority < parent->priority) {
            dynarray_set(pq->data, idx, parent);
            dynarray_set(pq->data, parent_idx, current);
            idx = parent_idx;
        } else {
            break;
        }
    }
}


/*
 * Helper function to maintain the heap property from a given node down to the leaves.
 */
static void heapify_down(struct pq* pq, int idx) {
    int size = dynarray_size(pq->data);
    while (1) {
        int left_idx = 2 * idx + 1;
        int right_idx = 2 * idx + 2;
        int min_idx = idx;

        if (left_idx < size) {
            struct pq_node* left = dynarray_get(pq->data, left_idx);
            struct pq_node* min = dynarray_get(pq->data, min_idx);
            if (left->priority < min->priority) {
                min_idx = left_idx;
            }
        }

        if (right_idx < size) {
            struct pq_node* right = dynarray_get(pq->data, right_idx);
            struct pq_node* min = dynarray_get(pq->data, min_idx);
            if (right->priority < min->priority) {
                min_idx = right_idx;
            }
        }

        if (min_idx != idx) {
            struct pq_node* temp = dynarray_get(pq->data, idx);
            dynarray_set(pq->data, idx, dynarray_get(pq->data, min_idx));
            dynarray_set(pq->data, min_idx, temp);
            idx = min_idx;
        } else {
            break;
        }
    }
}


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
	struct pq* pq = malloc(sizeof(struct pq));
    assert(pq);
    pq->data = dynarray_create();
	assert(pq->data);
    return pq;

}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue. That is the responsibility of the caller. However, you
 * do need to free the memory that holds each element stored in the priority 
 * queue. 
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
	int size = dynarray_size(pq->data);
	for (int i = 0; i < size; i++){
		free(dynarray_get(pq->data, i));
	}
	dynarray_free(pq->data);
	free(pq);
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
	return dynarray_size(pq->data) == 0 ? 1: 0;
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
    struct pq_node* node = malloc(sizeof(struct pq_node));
    assert(node);
    node->value = value;
    node->priority = priority;

    dynarray_insert(pq->data, node);

    int size = dynarray_size(pq->data);
    int current = size - 1;
    while (current > 0) {
        int parent = (current - 1) / 2;
        struct pq_node* parentNode = dynarray_get(pq->data, parent);
        struct pq_node* currentNode = dynarray_get(pq->data, current);
        // if the current node has a lower priority (higher priority in queue terms), swap it with its parent
        if (parentNode->priority <= currentNode->priority) {
            break;
        }
        dynarray_set(pq->data, current, parentNode);
        dynarray_set(pq->data, parent, currentNode);
        current = parent;
    }
}
/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
    assert(!pq_isempty(pq));
    struct pq_node* node = dynarray_get(pq->data, 0);
    return node->value;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
    assert(!pq_isempty(pq));
    struct pq_node* node = dynarray_get(pq->data, 0);
    return node->priority;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
  	assert(!pq_isempty(pq)); // make sure the priority queue is not empty
    int size = dynarray_size(pq->data);
    struct pq_node* root = dynarray_get(pq->data, 0);//get first element
    void* value = root->value;// store value to return


    struct pq_node* lastNode = dynarray_get(pq->data, size - 1);
    dynarray_set(pq->data, 0, lastNode);
    dynarray_remove(pq->data, size - 1); //remore last element
    free(root);

    heapify_down(pq, 0);

    return value;
}
