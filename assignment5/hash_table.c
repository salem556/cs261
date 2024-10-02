/*
 * In this file, you will write the structures and functions needed to
 * implement a hash table.  Feel free to implement any helper functions
 * you need in this file to implement a hash table.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Salem Demssie
 * Email: demssies@oregonstate.edu
 */

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "dynarray.h"
#include "hash_table.h"


#define INITIAL_CAPACITY 8
#define LOAD_FACTOR_THRESHOLD 0.75


typedef struct {
    void* key;
    void* value;
    int is_active;

} ht_entry;

/*
 * This is the structure that represents a hash table.  You must define
 * this struct to contain the data needed to implement a hash table.
 */


struct ht {
    struct dynarray* entries;
    int size;
    int (*hash)(void* key);  // function pointer for hash function
    int (*compare)(void* a, void* b);  // function pointer for key comparison
};


// function prototypes
void ht_resize(struct ht* ht, int (*convert)(void*));


// helper function to resize the hash table when load factor threshold is reached
void ht_resize(struct ht* ht, int (*convert)(void*)) {
    int old_capacity = dynarray_size(ht->entries);
    int new_capacity = old_capacity * 2;
    struct dynarray* new_entries = dynarray_create();

    for (int i = 0; i < new_capacity; i++) {
        ht_entry* new_entry = calloc(1, sizeof(ht_entry));
        if (!new_entry) {
            for (int j = 0; j < i; j++) {
                free(dynarray_get(new_entries, j));
            }
            dynarray_free(new_entries);
            return;  // Failed to allocate memory for resize
        }
        dynarray_insert(new_entries, new_entry);
    }

    for (int i = 0; i < old_capacity; i++) {
        ht_entry* old_entry = (ht_entry*)dynarray_get(ht->entries, i);
        if (old_entry->is_active) {
            int index = convert(old_entry->key) % new_capacity;
            ht_entry* new_entry = (ht_entry*)dynarray_get(new_entries, index);
            while (new_entry->is_active) {
                index = (index + 1) % new_capacity;
                new_entry = (ht_entry*)dynarray_get(new_entries, index);
            }
            *new_entry = *old_entry;  // shallow copy
            new_entry->is_active = true;
        }
    }

    for (int i = 0; i < old_capacity; i++) {
        free(dynarray_get(ht->entries, i));
    }
    dynarray_free(ht->entries);
    ht->entries = new_entries;
}


/*
 * This function should allocate and initialize an empty hash table and
 * return a pointer to it.
 */
struct ht* ht_create(){
    struct ht* ht = malloc(sizeof(struct ht));
    if (ht == NULL) return NULL;

    ht->entries = dynarray_create();
    if (ht->entries == NULL) {
        free(ht);
        return NULL;
    }

    ht->size = 0;
    for (int i = 0; i < INITIAL_CAPACITY; i++) {
        ht_entry* entry = calloc(1, sizeof(ht_entry));
        if (entry == NULL) {
            // Clean up previously allocated entries
            for (int j = 0; j < i; j++) {
                free(dynarray_get(ht->entries, j));
            }
            dynarray_free(ht->entries);
            free(ht);
            return NULL;
        }
        dynarray_insert(ht->entries, entry);
    }

    return ht;
}

/*
 * This function should free the memory allocated to a given hash table.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the hash table.  That is the responsibility of the caller.
 *
 * Params:
 *   ht - the hash table to be destroyed.  May not be NULL.
 */
void ht_free(struct ht* ht){
    
    for (int i = 0; i < dynarray_size(ht->entries); i++) {
        free(dynarray_get(ht->entries, i));
    }
    dynarray_free(ht->entries);
    free(ht);
}

/*
 * This function should return 1 if the specified hash table is empty and
 * 0 otherwise.
 *
 * Params:
 *   ht - the hash table whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if ht is empty and 0 otherwise.
 */
int ht_isempty(struct ht* ht){
    return ht->size == 0;
}


/*
 * This function returns the size of a given hash table (i.e. the number of
 * elements stored in it, not the capacity).
 *
 * Params:
 *   ht - the hash table whose size is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return the size of the given hash table.
 */
int ht_size(struct ht* ht){
    return ht->size;
}


/*
 * This function takes a key, maps it to an integer index value in the hash table,
 * and returns it. The hash function is passed in as a function pointer, stored in 'convert'
 * Assuming the passed in hash function is well designed.
 *
 * Params:
 *   ht - the hash table into which to store the element.  May not be NULL.
 *   key - the key of the element used to calculate the index
 *   convert - a pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 *
 * Return:
 *   Should return the index value of 'key' in the hash table .
 */
int ht_hash_func(struct ht* ht, void* key, int (*convert)(void*)) {
    return convert(key) % dynarray_size(ht->entries);
}







/*
 * This function should insert a given element into a hash table with a
 * specified key. Note that you cannot have two same keys in one hash table.
 * If the key already exists, update the value associated with the key.  
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * Resolution of collisions is requried, use either chaining or open addressing.
 * If using chaining, double the number of buckets when the load factor is >= 4
 * If using open addressing, double the array capacity when the load factor is >= 0.75
 * load factor = (number of elements) / (hash table capacity)
 *
 * Params:
 *   ht - the hash table into which to insert an element.  May not be NULL.
 *   key - the key of the element
 *   value - the value to be inserted into ht.
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */
void ht_insert(struct ht* ht, void* key, void* value, int (*convert)(void*)) {
    if ((float)(ht->size + 1) / dynarray_size(ht->entries) >= LOAD_FACTOR_THRESHOLD) {
        ht_resize(ht, convert);
    }

    int index = ht_hash_func(ht, key, convert);
    ht_entry* entry = (ht_entry*)dynarray_get(ht->entries, index);

    // loop to handle collisions and find the appropriate spot
    while (entry->is_active) {
        if (convert(entry->key) == convert(key)) {
            entry->value = value;  // Update value if key already exists
            return;
        }
        index = (index + 1) % dynarray_size(ht->entries);
        entry = (ht_entry*)dynarray_get(ht->entries, index);
        
        // prevent infinite loop by checking if it loops back to the start index
        if (index == ht_hash_func(ht, key, convert)) {
            break;  // indicates full table, though resize should prevent this
        }
    }

    // if found a spot or reach an inactive spot
    if (!entry->is_active) {
        entry->key = key;
        entry->value = value;
        entry->is_active = true;
        ht->size++;
    }
}




/*
 * This function should search for a given element in a hash table with a
 * specified key provided.   
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * If the key is found, return the corresponding value (void*) of the element,
 * otherwise, return NULL
 *
 * Params:
 *   ht - the hash table into which to loop up for an element.  May not be NULL.
 *   key - the key of the element to search for
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 *
 * Return:
 *   Should return the value of the corresponding 'key' in the hash table .
 */
void* ht_lookup(struct ht* ht, void* key, int (*convert)(void*)){
    int index = ht_hash_func(ht, key, convert);
    ht_entry* entry = (ht_entry*)dynarray_get(ht->entries, index);

    while (entry->is_active || entry->key != NULL) {
        if (entry->is_active && convert(entry->key) == convert(key)) {
            return entry->key; // Key found, return the key itself
        }
        index = (index + 1) % dynarray_size(ht->entries);
        entry = (ht_entry*)dynarray_get(ht->entries, index);

        // check for loop around to prevent infinite loop
        if (index == ht_hash_func(ht, key, convert)) {
            break; // looped back to the start
        }
    }

    return NULL; // key not found
}


/*
 * This function should remove a given element in a hash table with a
 * specified key provided.   
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * If the key is found, remove the element and return, otherwise, do nothing and return 
 *
 * Params:
 *   ht - the hash table into which to remove an element.  May not be NULL.
 *   key - the key of the element to remove
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */
void ht_remove(struct ht* ht, void* key, int (*convert)(void*)){
    int index = ht_hash_func(ht, key, convert);
    for (int i = 0; i < dynarray_size(ht->entries); i++) {
        ht_entry* entry = (ht_entry*)dynarray_get(ht->entries, index);

        if (entry->is_active && convert(entry->key) == convert(key)) {
            entry->is_active = false;  // deactivate the entry
            ht->size--;  // decrement size
            return;
        }

        index = (index + 1) % dynarray_size(ht->entries);  

        // prevent infinite loop by breaking if it comes back to the start
        if (index == ht_hash_func(ht, key, convert)) {
            break;
        }
    }
}
