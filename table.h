#ifndef SOLARIA_TABLE_H
#define SOLARIA_TABLE_H

#include "common.h"
#include "value.h"

/*
 * As the hash table will need to return Solaria objects for evaluation, our
 * hash table implementation will store key-value pairs that are objects
 * themselves.
 */
typedef struct {
  obj_string_t* key;
  value_t value;
} entry_t;

/*
 * Solaria uses a hash table to store variables. The algorithm used for hashing
 * is called "FNV-1a". As far as table_t goes, it is only a struct to store
 * an array of entries.
 */
typedef struct {
  // Number of key-value pairs.
  int count;
  // Array's allocated size.
  int capacity;
  entry_t* entries;
} table_t;

void init_table(table_t* table);
void free_table(table_t* table);
bool table_get(table_t* table, obj_string_t* key, value_t* value);
bool table_set(table_t* table, obj_string_t* key, value_t value);
bool table_delete(table_t* table, obj_string_t* key);
void table_add_all(table_t* from, table_t* to);
obj_string_t* table_find_string(table_t* table, const char* chars, int length, uint32_t hash);

#endif
