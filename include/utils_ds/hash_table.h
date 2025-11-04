#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdbool.h>

typedef struct Hash_table Hash_table;

Hash_table* hash_table_create();
void hash_table_destroy(Hash_table* ht);
/*
    \param key: NULL terminated string
*/
void* hash_table_get(Hash_table* ht, const char* key);
/*
    \param key: NULL terminated string
*/
void* hash_table_get_locked(Hash_table* ht, const char* key);
/*
    \param key: NULL terminated string
*/
bool hash_table_put(Hash_table* ht, const char* key, void* value);
/*
    \param key: NULL terminated string
*/
bool hash_table_put_locked(Hash_table* ht, const char* key, void* value);
/*
    \param key: NULL terminated string
*/
bool hash_table_delete(Hash_table* ht, const char* key);
/*
    \param key: NULL terminated string
*/
bool hash_table_delete_locked(Hash_table* ht, const char* key);
/*
    print all entries
    (For debug purpose)
*/
void hash_table_print(Hash_table* ht);


#endif
