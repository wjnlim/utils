#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "utils_ds/rb_tree.h"

typedef struct S {
    int num;
} S;

// for increasing order
int cmp_int(const void* lkey, const void* rkey) {
    intptr_t l = (intptr_t)lkey;
    intptr_t r = (intptr_t)rkey;

    if (l == r)
        return 0;
    else if (l < r)
        return -1;
    else
        return 1;
}

// for decreasing order
int cmp_struct(const void* lkey, const void* rkey) {
    S* l = (S*)lkey;
    S* r = (S*)rkey;
    // int l = *((int*)lkey);
    // int r = *((int*)rkey);

    if (l->num == r->num)
        return 0;
    else if (l->num < r->num)
        return 1;
    else
        return -1;
}

// static void print_int_tree(RB_tree* t) {
//     printf("Tree: ");
//     void* x = rbt_first(t);
//     while (x) {
//         printf("(%ld, %ld) ", (intptr_t)x->key, (intptr_t)x->data);
//         x = rbt_find_next(t, x);
//     }
//     printf("\n");
// }

static void print_int_tree(RB_tree* t) {
    printf("Tree(int): ");
    // void* x = rbt_first(t);
    // void* x = rbt_first(t);
    RBT_iterator* x = rbt_first(t);
    void* key;
    while (x) {
        key = x->key;
        printf("(%ld) ", (intptr_t)key);
        x = rbt_iterator_next(t, x);
    }
    printf("\n");
}

static void print_int_tree_reverse(RB_tree* t) {
    printf("Tree(int) in reverse: ");
    RBT_iterator* x = rbt_last(t);
    void* key;
    while (x) {
        key = x->key;
        printf("(%ld) ", (intptr_t)key);
        x = rbt_iterator_prev(t, x);
    }
    printf("\n");
}

static void print_struct_tree(RB_tree* t) {
    printf("Tree(struct): ");
    // void* x = rbt_first(t);
    RBT_iterator* x = rbt_first(t);
    void* key;
    while (x) {
        key = x->key;
        printf("(%d, %p) ", ((S*)key)->num, key);
        x = rbt_iterator_next(t, x);
    }
    printf("\n");
}

static void print_struct_tree_reverse(RB_tree* t) {
    printf("Tree(struct): ");
    // void* x = rbt_first(t);
    RBT_iterator* x = rbt_last(t);
    void* key;
    while (x) {
        key = x->key;
        printf("(%d, %p) ", ((S*)key)->num, key);
        x = rbt_iterator_prev(t, x);
    }
    printf("\n");
}

void test_init(void) {
    RB_tree* t1 = rbt_create(cmp_int);
    RB_tree* t2 = rbt_create(cmp_struct);
    assert(t1 != NULL);
    assert(t2 != NULL);
    rbt_destroy(t1);
    rbt_destroy(t2);
}

static void test_single_insert(RB_tree* ti, RB_tree* ts, intptr_t key, S* key_s) {
    // insert
    printf("Inserting key: %ld\n\n", key);
    RBT_iterator* inserted_key_it = rbt_insert(ti, (void*)key);
    // S key_struct = {.num = key};
    RBT_iterator* inserted_key_struct_it = rbt_insert(ts, (void*)key_s);
    assert(inserted_key_it != NULL && inserted_key_struct_it != NULL);
    assert((intptr_t)inserted_key_it->key == key && 
                (intptr_t)(((S*)inserted_key_struct_it->key)->num) == key);
    print_int_tree(ti);
    print_struct_tree(ts);
}

static void test_single_search(RB_tree* ti, RB_tree* ts, intptr_t key) {
    //search
    intptr_t false_key = key + 1;
    S key_struct = {.num = key};
    S false_s = {.num = false_key};

    printf("Searching false key\n");
    RBT_iterator* found_key_it = rbt_find(ti, (void*)false_key);
    RBT_iterator* found_key_s_it = rbt_find(ts, (void*)&false_s);
    assert(found_key_it == NULL && found_key_s_it == NULL);

    printf("Searching true key %ld\n", key);
    found_key_it = rbt_find(ti, (void*)key);
    found_key_s_it = rbt_find(ts, (void*)&key_struct);
    assert(found_key_it != NULL);
    assert(found_key_s_it != NULL);
    assert((intptr_t)found_key_it->key == key && 
        (intptr_t)(((S*)found_key_s_it->key)->num) == key);
}

static void test_single_flnp(RB_tree* ti, RB_tree* ts, intptr_t key) {
    // first, last, next, prev
    printf("Searching first, last, next, prev\n");
    RBT_iterator* first_key_it = rbt_first(ti);
    RBT_iterator* last_key_it = rbt_last(ti);
    assert(first_key_it != NULL && last_key_it != NULL);

    printf("\nTree(int): first: %ld, last: %ld", (intptr_t)first_key_it->key, (intptr_t)last_key_it->key);
    RBT_iterator* found_key_it = rbt_find(ti, first_key_it->key);
    RBT_iterator* next_it = rbt_iterator_next(ti, found_key_it);

    if (next_it) {
        printf(", next: %ld", (intptr_t)next_it->key);
    }

    RBT_iterator* after_key_it = rbt_find_after(ti, found_key_it->key);
    if (after_key_it) {
        printf(", next: %ld", (intptr_t)after_key_it->key);
    }

    found_key_it = rbt_find(ti, last_key_it->key);
    RBT_iterator* prev_it = rbt_iterator_prev(ti, found_key_it);

    if (prev_it) {
        printf(", prev: %ld", (intptr_t)prev_it->key);
    }
    RBT_iterator* before_key_it = rbt_find_before(ti, last_key_it->key);
    if (before_key_it) {
        printf(", prev: %ld", (intptr_t)before_key_it->key);
    }
    printf("\n");

    RBT_iterator* first_key_s_it = rbt_first(ts);
    RBT_iterator* last_key_s_it = rbt_last(ts);
    printf("Tree(struct): first: %p, last: %p", first_key_s_it->key, last_key_s_it->key);

    RBT_iterator* found_key_s_it = rbt_find(ts, first_key_s_it->key);

    next_it = rbt_iterator_next(ts, found_key_s_it);
    if (next_it) {
        printf(", next: %p", next_it->key);
    }
    RBT_iterator* after_key_s_it = rbt_find_after(ts, first_key_s_it->key);
    if (after_key_s_it) {
        printf(", after: %p", after_key_s_it->key);
    }

    found_key_s_it = rbt_find(ts, last_key_s_it->key);
    prev_it = rbt_iterator_prev(ts, found_key_s_it);
    if (prev_it) {
        printf(", prev: %p", prev_it->key);
    }
    RBT_iterator* before_key_s_it = rbt_find_before(ts, last_key_s_it->key);
    if (before_key_s_it) {
        printf(", before: %p", before_key_s_it->key);
    }
    printf("\n");
}

static void test_single_delete(RB_tree* ti, RB_tree* ts, intptr_t key) {
    S key_struct = {.num = key};
    // delete
    printf("\nDeleting node with key: %ld...\n", key);
    rbt_delete(ti, (void*)key);
    rbt_delete(ts, (void*)&key_struct);
    print_int_tree(ti);
    print_struct_tree(ts);
}

void test_single(intptr_t key) {
    printf("testing single element...\n");
    RB_tree* ti = rbt_create(cmp_int);
    RB_tree* ts = rbt_create(cmp_struct);
    assert(ti != NULL && ts != NULL);

    print_int_tree(ti);
    print_struct_tree(ts);

    S key_s = {.num = key};
    test_single_insert(ti, ts, key, &key_s);
    test_single_search(ti, ts, key);
    test_single_flnp(ti, ts, key);
    test_single_delete(ti, ts, key);
    // // insert
    // printf("Inserting key: %ld\n\n", key);
    // void* inserted_key = rbt_insert(ti, (void*)key);
    // S key_struct = {.num = key};
    // void* inserted_key_struct = rbt_insert(ts, (void*)&key_struct);
    // assert(inserted_key != NULL && inserted_key_struct != NULL);
    // assert((intptr_t)inserted_key == key && 
    //             (intptr_t)(((S*)inserted_key_struct)->num) == key);
    // print_int_tree(ti);
    // print_struct_tree(ts);
    // //search
    // intptr_t false_key = key + 1;
    // S false_s = {.num = false_key};

    // printf("Searching false key\n");
    // RBT_iterator* found_key_it = rbt_find(ti, (void*)false_key);
    // RBT_iterator* found_key_s_it = rbt_find(ts, (void*)&false_s);
    // assert(found_key_it == NULL && found_key_s_it == NULL);

    // printf("Searching true key\n");
    // found_key_it = rbt_find(ti, (void*)key);
    // found_key_s_it = rbt_find(ts, (void*)&key_struct);
    // assert(found_key_it != NULL && found_key_s_it != NULL);
    // assert((intptr_t)found_key_it->key == key && 
    //     (intptr_t)(((S*)found_key_s_it->key)->num) == key);
    // // first, last, next, prev
    // printf("Searching first, last, next, prev\n");
    // void* first_key = rbt_first(ti);
    // void* last_key = rbt_last(ti);
    // assert(first_key != NULL && last_key != NULL);
    // // printf("Searched first, last\n");
    // printf("\nTree(int): first: %ld, last: %ld", (intptr_t)first_key, (intptr_t)last_key);
    // // printf("\nTree(int): first: %p, last: %p", fk, lk);
    // // void* nk = rbt_find_next(ti, fi);
    // // RBT_iterator* nit = rbt_get_iterator(ti, fi);
    // found_key_it = rbt_find(ti, first_key);
    // RBT_iterator* next_it = rbt_iterator_next(ti, found_key_it);
    // // void* nk = rbt_iterator_get_key(rbt_iterator_next(ti, nit));
    // if (next_it) {
    //     printf(", next: %ld", (intptr_t)next_it->key);
    // }

    // // printf("Searching after\n");
    // void* after_key = rbt_find_after(ti, found_key_it->key);
    // if (after_key) {
    //     printf(", next: %ld", (intptr_t)after_key);
    // }
    // // printf("Searching prev\n");
    // // void* pk = rbt_find_prev(ti, fi);
    // // RBT_iterator* pit = rbt_get_iterator(ti, fi);
    // found_key_it = rbt_find(ti, last_key);
    // RBT_iterator* prev_it = rbt_iterator_prev(ti, found_key_it);
    // // void* pk = rbt_iterator_get_key(rbt_iterator_prev(ti, pit));
    // if (prev_it) {
    //     printf(", prev: %ld", (intptr_t)prev_it->key);
    // }
    // void* before_key = rbt_find_before(ti, last_key);
    // if (before_key) {
    //     printf(", prev: %ld", (intptr_t)before_key);
    // }
    // printf("\n");

    // void* first_key_s = rbt_first(ts);
    // void* last_key_s = rbt_last(ts);
    // printf("tree(struct): first: %p, last: %p", first_key_s, last_key_s);
    // // nk = rbt_find_next(ts, fs);
    // // nit = rbt_get_iterator(ts, fs);
    // found_key_s_it = rbt_find(ts, first_key_s);
    // // nk = rbt_iterator_get_key(rbt_iterator_next(ts, nit));
    // next_it = rbt_iterator_next(ts, found_key_s_it);
    // if (next_it) {
    //     printf(", next: %p", next_it->key);
    // }
    // void* after_key_s = rbt_find_after(ts, first_key_s);
    // if (after_key_s) {
    //     printf(", next: %p", after_key_s);
    // }
    // // pk = rbt_find_prev(ts, fs);
    // // pit = rbt_get_iterator(ts, fs);
    // // pk = rbt_iterator_get_key(rbt_iterator_prev(ts, pit));
    // found_key_s_it = rbt_find(ts, last_key_s);
    // prev_it = rbt_iterator_prev(ts, found_key_s_it);
    // if (prev_it) {
    //     printf(", prev: %p", prev_it->key);
    // }
    // void* before_key_s = rbt_find_before(ts, last_key_s);
    // if (before_key_s) {
    //     printf(", next: %p", before_key_s);
    // }
    // printf("\n");
    // // delete
    // printf("\nDeleting node with key: %ld...\n", key);
    // rbt_delete(ti, first_key);
    // rbt_delete(ts, first_key_s);
    // print_int_tree(ti);
    // print_struct_tree(ts);

    rbt_destroy(ti);
    rbt_destroy(ts);

    printf("\n");
}

static void test_multi_insert(RB_tree* ti, RB_tree* ts,
                                        intptr_t arr[], S sarr[]) {
    printf("Inserting 11 elements...\n\n");
    RBT_iterator* inserted_k_it, *inserted_ks_it;
    for (int i = 0; i < 11; i++) {
        inserted_k_it = rbt_insert(ti, (void*)arr[i]);
        printf("inserted(int): (%ld)\n", (intptr_t)inserted_k_it->key);

        inserted_ks_it = rbt_insert(ts, (void*)&sarr[i]);
        printf("inserted(struct): (%ld, %p)\n",
                        (intptr_t)(((S*)inserted_ks_it->key)->num), inserted_ks_it->key);
    }

    print_int_tree(ti);
    print_struct_tree(ts);
}

static void test_multi_search(RB_tree* ti, RB_tree* ts,
                                        intptr_t arr[], S sarr[]) {
    RBT_iterator* found_k_it, *found_ks_it;

    printf("Searching elements not in tree...\n\n");
    for (int i = 11; i < 14; i++) {
        found_k_it = rbt_find(ti, (void*)arr[i]);
        found_ks_it = rbt_find(ts, (void*)&sarr[i]);

        assert(found_k_it == NULL && found_ks_it == NULL);
    }

    intptr_t k26 = 26;
    S ks26 = {.num = 26};
    printf("Searching an element less than 26..\n\n");
    found_k_it = rbt_find_before(ti, (void*)k26);
    found_ks_it = rbt_find_after(ts, (void*)&ks26);
    assert(found_k_it != NULL && found_ks_it != NULL);

    printf("element(int) < 26: (%ld)\n", (intptr_t)found_k_it->key);
    printf("element(struct) < 26: (%ld, %p)\n", 
    (intptr_t)(((S*)found_ks_it->key)->num), found_ks_it->key);
}

static void test_multi_delete(RB_tree* ti, RB_tree* ts,
                                        intptr_t arr[], S sarr[]) {
    printf("Deleting first 2 elemets in arr...\n\n");
    for (int i = 0; i < 2; i++) {
        rbt_delete(ti, (void*)arr[i]);

        rbt_delete(ts, (void*)&sarr[i]);
    }
    print_int_tree(ti);
    print_struct_tree(ts);

    printf("Deleting first 2 elements in tree...\n\n");
    // int tree
    RBT_iterator* first_k_it = rbt_first(ti);
    RBT_iterator* next_k_it = rbt_iterator_prev(ti, first_k_it);
    assert(next_k_it == NULL);
    next_k_it = rbt_find_before(ti, first_k_it->key);
    assert(next_k_it == NULL);

    next_k_it = rbt_find_after(ti, first_k_it->key);
    RBT_iterator* next_deleted_it = rbt_delete_iterator(ti, first_k_it);
    assert(next_deleted_it == next_k_it);
    rbt_delete_iterator(ti, next_k_it);
    // struct tree
    RBT_iterator* first_ks_it = rbt_first(ts);
    RBT_iterator* next_ks_it = rbt_iterator_prev(ts, first_ks_it);
    assert(next_ks_it == NULL);
    next_ks_it = rbt_find_before(ts, first_ks_it->key);
    assert(next_ks_it == NULL);

    next_ks_it = rbt_find_after(ts, first_ks_it->key);
    next_deleted_it = rbt_delete_iterator(ts, first_ks_it);
    assert(next_deleted_it == next_ks_it);
    rbt_delete_iterator(ts, next_ks_it);

    print_int_tree(ti);
    print_struct_tree(ts);

    printf("Deleting last 2 elements in tree...\n\n");
    // int tree
    RBT_iterator* last_k_it = rbt_last(ti);
    RBT_iterator* prev_k_it = rbt_iterator_next(ti, last_k_it);
    assert(prev_k_it == NULL);
    prev_k_it = rbt_find_after(ti, last_k_it->key);
    assert(prev_k_it == NULL);

    prev_k_it = rbt_iterator_prev(ti, last_k_it);
    RBT_iterator* before_k_it = rbt_find_before(ti, last_k_it->key);
    assert(prev_k_it == before_k_it);
    rbt_delete_iterator(ti, last_k_it);
    rbt_delete_iterator(ti, prev_k_it);

    // struct tree
    RBT_iterator* last_ks_it = rbt_last(ts);
    RBT_iterator* prev_ks_it = rbt_iterator_next(ts, last_ks_it);
    assert(prev_ks_it == NULL);
    prev_ks_it = rbt_find_after(ts, last_ks_it->key);
    assert(prev_ks_it == NULL);

    prev_ks_it = rbt_iterator_prev(ts, last_ks_it);
    RBT_iterator* before_ks_it = rbt_find_before(ts, last_ks_it->key);
    assert(prev_ks_it == before_ks_it);
    rbt_delete_iterator(ts, last_ks_it);
    rbt_delete_iterator(ts, prev_ks_it);

    print_int_tree(ti);
    print_struct_tree(ts);
}

static void test_multi_insert_all(RB_tree* ti, RB_tree* ts,
                                        intptr_t arr[], S sarr[]) {
    printf("Inserting all elements in arr...\n\n");
    RBT_iterator* inserted_k_it, *inserted_ks_it;
    for (int i = 0; i < 14; i++) {
        inserted_k_it = rbt_insert(ti, (void*)arr[i]);
        printf("inserted(int): (%ld)\n", (intptr_t)inserted_k_it->key);

        inserted_ks_it = rbt_insert(ts, (void*)&sarr[i]);
        printf("inserted(struct): (%ld, %p)\n", 
                    (intptr_t)(((S*)inserted_ks_it->key)->num), inserted_ks_it->key);
    }
    print_int_tree(ti);
    print_struct_tree(ts);
}

static void test_multi_search_24s(RB_tree* ti, RB_tree* ts,
                                        intptr_t arr[], S sarr[]) {
    printf("Searching all key 24s...\n\n");
    intptr_t k24 = 24;
    S ks24 = {.num = 24};
    RBT_iterator* found_k_it, *found_ks_it;
    found_k_it = rbt_find(ti, (void*)k24);
    while (found_k_it) {
        if (cmp_int(found_k_it->key, (void*)k24) == 0) {
            printf("element(int) 24: (%ld)\n", (intptr_t)found_k_it->key);
            found_k_it = rbt_iterator_next(ti, found_k_it);
        } else {
            break;
        }
    }

    found_ks_it = rbt_find(ts, (void*)&ks24);
    while (found_ks_it) {
        if (cmp_struct(found_ks_it->key, (void*)&ks24) == 0) {
            printf("element(struct) 24: (%ld, %p)\n", 
            (intptr_t)(((S*)found_ks_it->key)->num), found_ks_it->key);
            found_ks_it = rbt_iterator_next(ts, found_ks_it);
        } else {
            break;
        }
    }
}

static void test_multi_delete_24s(RB_tree* ti, RB_tree* ts,
                                        intptr_t arr[], S sarr[]) {
    printf("Deleting all key 24s...\n\n");
    intptr_t k24 = 24;
    S ks24 = {.num = 24};
    // RBT_iterator* found_k24_it = rbt_find(ti, (void*)k24);
    int num24 = rbt_delete(ti, (void*)k24);

    RBT_iterator* found_ks24_it = rbt_find(ts, (void*)&ks24);
    while (found_ks24_it) {
        if (cmp_struct(found_ks24_it->key, (void*)&ks24) == 0) {
            found_ks24_it = rbt_delete_iterator(ts, found_ks24_it);
        } else {
            break;
        }
    }

    printf("Deleted %d 24s\n", num24);
    
    print_int_tree(ti);
    print_struct_tree(ts);
}

void test_multi() {
    printf("testing multi element...\n");
    RB_tree* ti = rbt_create(cmp_int);
    assert(ti != NULL);
    RB_tree* ts = rbt_create(cmp_struct);
    assert(ts != NULL);

    // incr order: 2, 5, 8, 10, 12, 23, 24, 24, 25, 34, 36, 67, 156, 990
    // decr order: 990, 156, 67, 36, 34, 25, 24, 24, 23, 12, 10, 8, 5, 2
    intptr_t arr[] = {10, 5, 8, 2, 34, 23, 156, 24, 67, 12, 24, 36, 990, 25};
    S sarr[] = {{10}, {5}, {8}, {2}, {34}, {23}, {156}, {24}, {67}, 
                    {12}, {24}, {36}, {990}, {25}};

    printf("arr[]: ");
    for (int i = 0; i < 14; i++) {
        printf("%ld ", arr[i]);
    }
    printf("\n");

    print_int_tree(ti);
    print_struct_tree(ts);

    test_multi_insert(ti, ts, arr, sarr);
    test_multi_search(ti, ts, arr, sarr);
    test_multi_delete(ti, ts, arr, sarr);
    test_multi_insert_all(ti, ts, arr, sarr);
    test_multi_search_24s(ti, ts, arr, sarr);
    test_multi_delete_24s(ti, ts, arr, sarr);

    // printf("Inserting 11 elements...\n\n");
    // void* inserted_k, *inserted_ks;
    // for (int i = 0; i < 11; i++) {
    //     inserted_k = rbt_insert(ti, (void*)arr[i]);
    //     printf("inserted(int): (%ld)\n", (intptr_t)inserted_k);

    //     inserted_ks = rbt_insert(ts, (void*)&sarr[i]);
    //     printf("inserted(struct): (%ld, %p)\n", (intptr_t)(((S*)inserted_ks)->num), 
    //                                                                     inserted_ks);
    // }

    // print_int_tree(ti);
    // print_struct_tree(ts);

    // RBT_iterator* found_k_it, *found_ks_it;

    // printf("Searching elements not in tree...\n\n");
    // for (int i = 11; i < 14; i++) {
    //     found_k_it = rbt_find(ti, (void*)arr[i]);
    //     found_ks_it = rbt_find(ts, (void*)&sarr[i]);

    //     assert(found_k_it == NULL && found_ks_it == NULL);
    // }

    // printf("Deleting first 2 elemets in arr...\n\n");
    // for (int i = 0; i < 2; i++) {
    //     // ni = rbt_find(ti, (void*)arr[i]);
    //     // printf("\tdeleting\n");
    //     // printf("\tdeleting (%ld, %ld)...\n", (intptr_t)ni->key, (intptr_t)ni->data);
    //     rbt_delete(ti, (void*)arr[i]);

    //     // ns = rbt_find(ts, (void*)(&sarr[i].num));
    //     // printf("\tdeleting (%ld, %p)...\n", (intptr_t)(*(int*)ns->key), ns->data);
    //     rbt_delete(ts, (void*)&sarr[i]);
    // }
    // print_int_tree(ti);
    // print_struct_tree(ts);

    // printf("Deleting first 2 elements in tree...\n\n");
    // void* first_k = rbt_first(ti);
    // RBT_iterator* first_k_it = rbt_first_iterator(ti);
    // // void* nin = rbt_find_prev(ti, ni);
    // RBT_iterator* next_k_it = rbt_iterator_prev(ti, first_k_it);
    // // void* nin = rbt_iterator_get_key(rbt_iterator_prev(ti, nit));
    // assert(next_k_it == NULL);
    // void* next_k = rbt_find_before(ti, first_k);
    // assert(next_k == NULL);
    // next_k = rbt_find_after(ti, first_k);
    // rbt_delete(ti, first_k);
    // rbt_delete(ti, next_k);

    // void* first_ks = rbt_first(ts);
    // RBT_iterator* first_ks_it = rbt_first_iterator(ts);
    // // void* nsn = rbt_find_prev(ts, ns);
    // RBT_iterator* next_ks_it = rbt_iterator_prev(ts, first_ks_it);
    // // void* nsn = rbt_iterator_get_key(rbt_iterator_prev(ti, nst));
    // assert(next_ks_it == NULL);
    // void* next_ks = rbt_find_before(ts, first_ks);
    // assert(next_ks == NULL);
    // next_ks = rbt_find_after(ts, first_ks);
    // rbt_delete(ts, first_ks);
    // rbt_delete(ts, next_ks);
    // print_int_tree(ti);
    // print_struct_tree(ts);

    // printf("Deleting last 2 elements in tree...\n\n");
    // void* last_k = rbt_last(ti);
    // RBT_iterator* last_k_it = rbt_last_iterator(ti);
    // RBT_iterator* prev_k_it = rbt_iterator_next(ti, last_k_it);
    // // void* nin = rbt_iterator_get_key(rbt_iterator_prev(ti, nit));
    // // void* nip = rbt_find_next(ti, ni);
    // assert(prev_k_it == NULL);
    // void* prev_k = rbt_find_after(ti, last_k);
    // assert(prev_k == NULL);
    // prev_k = rbt_find_before(ti, last_k);
    // rbt_delete(ti, last_k);
    // rbt_delete(ti, prev_k);

    // void* last_ks = rbt_last(ts);
    // RBT_iterator* last_ks_it = rbt_last_iterator(ts);
    // RBT_iterator* prev_ks_it = rbt_iterator_next(ts, last_ks_it);
    // // void* nsp = rbt_find_next(ts, ns);
    // assert(prev_ks_it == NULL);
    // void* prev_ks = rbt_find_after(ts, last_ks);
    // assert(prev_ks == NULL);
    // prev_ks = rbt_find_before(ts, last_ks);
    // rbt_delete(ts, last_ks);
    // rbt_delete(ts, prev_ks); // <- rbt_delete(ts, nsn) will cause error!! why?
    // print_int_tree(ti);
    // print_struct_tree(ts);

    // printf("Inserting all elements in arr...\n\n");
    // for (int i = 0; i < 14; i++) {
    //     inserted_k = rbt_insert(ti, (void*)arr[i]);
    //     printf("inserted(int): (%ld)\n", (intptr_t)inserted_k);

    //     inserted_ks = rbt_insert(ts, (void*)&sarr[i]);
    //     printf("inserted(struct): (%ld, %p)\n", (intptr_t)(((S*)inserted_ks)->num), 
    //                                                                     inserted_ks);
    // }
    // print_int_tree(ti);
    // print_struct_tree(ts);

    // printf("Deleting all key 24s...\n\n");
    // intptr_t k24 = 24;
    // S ks24 = {.num = 24};
    // RBT_iterator* found_k24_it = rbt_find(ti, (void*)k24);
    // while (found_k24_it) {
    //     rbt_delete(ti, found_k24_it->key);
    //     found_k24_it = rbt_find(ti, (void*)k24);
    // }
    // RBT_iterator* found_ks24_it = rbt_find(ts, (void*)&ks24);
    // while (found_ks24_it) {
    //     rbt_delete(ts, found_ks24_it->key);
    //     found_ks24_it = rbt_find(ts, (void*)&ks24);
    // }
    
    // print_int_tree(ti);
    // print_struct_tree(ts);

    printf("\n");
    print_int_tree_reverse(ti);
    print_struct_tree_reverse(ts);

    rbt_destroy(ti);
    rbt_destroy(ts);
}

int main(void) {
    test_init();
    test_single(1024);
    test_multi();
    printf("Passed all tests!\n");
}