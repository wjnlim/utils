#ifndef RB_TREE_H
#define RB_TREE_H


typedef struct RB_tree RB_tree;
typedef struct RBT_iterator {
    void* key;
} RBT_iterator;

/*
    0 if lkey == rkey,
    -1 if lkey < rkey,
    1 if lkey > rkey
*/
typedef int (*cmp_key)(const void* lkey, const void* rkey);

RB_tree* rbt_create(cmp_key cmp);
void rbt_destroy(RB_tree* rbt);

/*
    Insert the key
    return an iterator to the key, or NULL on error
*/
RBT_iterator* rbt_insert(RB_tree* rbt, void* key);
/*
    Delete all keys int rbt equivalent to the key.
    return the number of the deleted keys, 
*/
int rbt_delete(RB_tree* rbt, void* key);
/*
    Delete the iterator from the rbt.
    the key in the iter must be freed before this function is called
    if it was malloc()ed.
    return the next iterator of the iter,
    NULL if the next iterator doesn't exist
*/
RBT_iterator* rbt_delete_iterator(RB_tree* rbt, RBT_iterator* iter);
/*
    Search the key in the tree
    return an iterator which have a key equivalent to the key,
    NULL on error or if the key doesn't exist in the tree
*/
RBT_iterator* rbt_find(RB_tree* rbt, void* key);
RBT_iterator* rbt_first(RB_tree* rbt);
RBT_iterator* rbt_last(RB_tree* rbt);

/*
    Search an iterator to the last key in the tree 
    which is considered to go before (not same key) the given key.
    (the given key need not exist in the tree)
    return the found key,
    NULL on error or if the target key doesn't exist in the tree
*/
RBT_iterator* rbt_find_before(RB_tree* rbt, void* key);
/*
    Search an iterator to the first key in the tree 
    which is considered to go after (not same key) the given key.
    (the given key need not exist in the tree)
    return the found key,
    NULL on error or if the target key doesn't exist in the tree
*/
RBT_iterator* rbt_find_after(RB_tree* rbt, void* key);

/*
    Get An iterator which contains the next key of the given iterator.
    return the iterator,
    NULL on error or if the next key doesn't exist in the tree.
*/
RBT_iterator* rbt_iterator_next(RB_tree* rbt, RBT_iterator* iter);
/*
    Get An iterator which contains the prev key of the given iterator.
    return the iterator,
    NULL on error or if the prev key doesn't exist in the tree.
*/
RBT_iterator* rbt_iterator_prev(RB_tree* rbt, RBT_iterator* iter);


#endif