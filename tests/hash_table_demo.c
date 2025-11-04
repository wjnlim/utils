#include <stdio.h>
#include <stdlib.h>

#include "utils_ds/hash_table.h"

static char *in_table_data1[] = { "alpha", "bravo", "charlie", "delta",
    "echo", "foxtrot", "golf", "hotel", "india", "juliet",
    "kilo", "lima", "mike", "november", "oscar", "papa",
    "quebec", "romeo", "sierra", "tango", "uniform",
    "victor", "whisky", "x-ray", "yankee", "zulu"
};

static char *in_table_data2[] = { "alpha2", "bravo2", "charlie2", "delta2",
    "echo2", "foxtrot2", "golf2", "hotel2", "india2", "juliet2",
    "kilo2", "lima2", "mike2", "november2", "oscar2", "papa2",
    "quebec2", "romeo2", "sierra2", "tango2", "uniform2",
    "victor2", "whisky2", "x-ray2", "yankee2", "zulu2"
};

static char *not_in_table_data[] = {
    "not", "in", "table", "data", "not_in", "table_data"
};

int main(void) {
    Hash_table* ht = hash_table_create();
    for (int i = 0; i < 26; i++) {
        if (hash_table_put(ht, in_table_data1[i], (void*)i) < 0) {
            printf("hash_table_put error\n");
            exit(EXIT_FAILURE);
        }
    }
    // print some key-value pairs in table
    for (int i = 11; i < 24; i++) {
        const char* key = in_table_data1[i];
        void* val = hash_table_get(ht, key);
        printf("%9.9s -> %d\n", key, val ? (int) val : 0);
    }
    printf("\n");
    // print key-value pairs not in table
    for (int i = 0; i < 6; i++) {
        const char* key = not_in_table_data[i];
        void* val = hash_table_get(ht, key);
        printf("%9.9s -> %d\n", key, val ? (int) val : 0);
    }
    // delete some key-value pairs
    for (int i = 20; i < 25; i++) {
        const char* key = in_table_data1[i];
        if (hash_table_delete(ht, key)) {
            printf("Key:%s is deleted\n", key);
        } else {
            printf("Cannot delete key:%s the key in not in the hash table\n", key);
        }
    }
    for (int i = 0; i < 4; i++) {
        const char* key = not_in_table_data[i];
        if (hash_table_delete(ht, key)) {
            printf("Key:%s is deleted\n", key);
        } else {
            printf("Cannot delete key:%s the key in not in the hash table\n", key);
        }
    }
    printf("\n");
    for (int i = 0; i < 26; i++) {
        if (hash_table_put(ht, in_table_data2[i], (void*)i) < 0) {
            printf("hash_table_put error\n");
            exit(EXIT_FAILURE);
        }
    }
    // print all key-value pairs
    for (int i = 0; i < 26; i++) {
        const char* key = in_table_data1[i];
        void* val = hash_table_get(ht, key);
        printf("%9.9s -> %d\n", key, val ? (int) val : 0);
    }
    for (int i = 0; i < 26; i++) {
        const char* key = in_table_data2[i];
        void* val = hash_table_get(ht, key);
        printf("%9.9s -> %d\n", key, val ? (int) val : 0);
    }

    hash_table_destroy(ht);
    exit(EXIT_SUCCESS);
}