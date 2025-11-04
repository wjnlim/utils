#define _GNU_SOURCE

#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "utils_ds/hash_table.h"
// #include "common/my_err.h"

#define DEFAULT_CAPACITY 32 // must be a power of 2
#define FNV_OFFSET 2166136261u
#define FNV_PRIME 16777619
#define TABLE_MAX_LOAD 0.75

static char TOMBSTONE;
static void* VAL_TOMBSTONE = &TOMBSTONE;

typedef struct {
    const char* key;
    void* value;
} Entry;

struct Hash_table {
    Entry* entries;
    int count;
    int capacity;

    pthread_mutex_t lock;
};

Hash_table* hash_table_create() {
    Hash_table* ht = malloc(sizeof(*ht));
    if (ht == NULL) {
        fprintf(stderr, "[hash_table.c: hash_table_create()] malloc error(%s)\n", strerror(errno));
        return NULL;
    }
    ht->count = 0;
    ht->capacity = DEFAULT_CAPACITY;
    pthread_mutex_init(&ht->lock, NULL);

    ht->entries = calloc(DEFAULT_CAPACITY, sizeof(Entry));
    if (ht->entries == NULL) {
        fprintf(stderr, "[hash_table.c: hash_table_create()] calloc error(%s)\n", strerror(errno));
        free(ht);
        return NULL;
    }
    return ht;
}

void hash_table_destroy(Hash_table* ht) {
    assert(ht != NULL);
    free(ht->entries);
    free(ht);
}

/*
    This assumes that key is NULL-terminated
*/
static uint32_t hash_key(const char* key) {
    uint32_t hash = FNV_OFFSET;
    for (const char* c = key; *c != '\0'; c++) {
        hash ^= (uint32_t)(*c);
        hash *= FNV_PRIME;
    }
    // debug purpose
    // printf("[hash_table.c: hash_key()] hash val for key:%s is %u\n", key, hash);
    return hash;
}

static Entry* find_entry(Entry* entries, int capacity, const char* key) {
    uint32_t hash = hash_key(key);
    uint32_t idx = hash & (capacity - 1);

    Entry* tombstone = NULL;

    for (;;) {
        Entry* entry = &entries[idx];
        if (entry->key == NULL) {
            // check the entry is a tombstone or empty entry
            if (entry->value == VAL_TOMBSTONE) {
                // tombstone
                if (tombstone == NULL)
                    tombstone = entry;
            } else {
                // empty entry
                return tombstone != NULL ? tombstone : entry;
            }
        } else if (strcmp(entry->key, key) == 0) {
            // found the key
            return entry;
        }

        idx = (idx + 1) & (capacity - 1); // we know the capacity is a power of 2
    }
}

void* hash_table_get(Hash_table* ht, const char* key) {
    if (ht->count == 0)
        return NULL;
    
    Entry* entry = find_entry(ht->entries, ht->capacity, key);
    if (entry->key == NULL)
        return NULL;

    return entry->value;
}

void* hash_table_get_locked(Hash_table* ht, const char* key) {
    pthread_mutex_lock(&ht->lock);
    void* val = hash_table_get(ht, key);
    pthread_mutex_unlock(&ht->lock);
    return val;
}

static bool grow_capacity(Hash_table* ht) {
    int new_capacity = ht->capacity * 2;
    if (new_capacity < ht->capacity) {
        // capacity overflow
        return false; 
    }
    
    Entry* new_entries = calloc(new_capacity, sizeof(Entry));
    if (new_entries == NULL) {
        fprintf(stderr, "[hash_table.c: grow_capacity()] calloc error(%s)\n", strerror(errno));
        return false;
    }

    ht->count = 0;
    for (int i = 0; i < ht->capacity; i++) {
        Entry* entry = &ht->entries[i];
        if (entry->key == NULL)
            continue;
        
        Entry* new_pos = find_entry(new_entries, new_capacity, entry->key);
        new_pos->key = entry->key;
        new_pos->value = entry->value;
        ht->count++;
    }
    free(ht->entries);
    ht->entries = new_entries;
    ht->capacity = new_capacity;

    return true;
}

bool hash_table_put(Hash_table* ht, const char* key, void* value) {
    if (ht->count + 1 > ht->capacity * TABLE_MAX_LOAD) {
        if (!grow_capacity(ht)) {
            fprintf(stderr, "[hash_table.c: hash_table_put()] grow_capacity() error\n");
            return false;
        }
    }

    Entry* entry = find_entry(ht->entries, ht->capacity, key);

    //if it is new key and found entry is not tombstone
    if (entry->key == NULL && entry->value != VAL_TOMBSTONE)
        ht->count++;
    
    entry->key = strdup(key);
    if (entry->key == NULL) {
        fprintf(stderr, "[hash_table.c: hash_table_put()] strdup() error\n");
        return false;
    }
    entry->value = value;
    return true;
}

bool hash_table_put_locked(Hash_table* ht, const char* key, void* value) {
    pthread_mutex_lock(&ht->lock);
    bool res = hash_table_put(ht, key, value);
    pthread_mutex_unlock(&ht->lock);
    return res;
}

bool hash_table_delete(Hash_table* ht, const char* key) {
    if (ht->count == 0)
        return false;
    
    Entry* entry = find_entry(ht->entries, ht->capacity, key);
    if (entry->key == NULL)
        return false;
    
    // Make the place tomnstone
    entry->key = NULL;
    entry->value = VAL_TOMBSTONE;

    return true;
}

bool hash_table_delete_locked(Hash_table* ht, const char* key) {
    pthread_mutex_lock(&ht->lock);
    bool res = hash_table_delete(ht, key);
    pthread_mutex_unlock(&ht->lock);
    return res;
}

void hash_table_print(Hash_table* ht) {
    printf("\nHash table entries: ");
    for (int i = 0; i < ht->capacity; i++) {
        Entry* entry = &ht->entries[i];
        if (entry->key != NULL && entry->value != NULL) {
            printf("{%s, %p} ", entry->key, entry->value);
        }
    }
    printf("\n");
}