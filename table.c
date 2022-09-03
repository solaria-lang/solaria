#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"

/*
 * This macro's value helps to keep the table's load factor. We will grow the
 * array of entries when the capacity is 75% full.
 */
#define TABLE_MAX_LOAD 0.75


void init_table(table_t* table) {
  table->count = 0;
  table->capacity = 0;
  table->entries = NULL;
}

void free_table(table_t* table) {
  FREE_ARRAY(entry_t, table->entries, table->capacity);
  init_table(table);
}


static entry_t* find_entry(entry_t* entries, int capacity, obj_string_t* key) {
  // use modulo to map the key’s hash code to an index within the array’s
  // bounds. That gives us a bucket index where, ideally, we’ll be able to find
  // or place the entry.
  uint32_t index = key->hash % capacity;
  entry_t* tombstone = NULL;
  for (;;) {
    entry_t* entry = &entries[index];
    if (entry->key == NULL) {
      if (IS_NULL(entry->value)) {
        // Empty entry. If there's a tombstone, return its bucket so that
        // it can be reused by the insert function.
        return tombstone != NULL ? tombstone : entry;
      } else {
        // We found a tombstone.
        if (tombstone == NULL) tombstone = entry;
      }
    } else if (entry->key == key) {
      // We found the key.
      return entry;
    }
    index = (index + 1) % capacity;
  }
}


bool table_get(table_t* table, obj_string_t* key, value_t* value) {
  if (table->count == 0) return false;

  entry_t* entry = find_entry(table->entries, table->capacity, key);
  if (entry->key == NULL) return false;

  *value = entry->value;
  return true;
}


static void adjust_capacity(table_t* table, int capacity) {
  entry_t* entries = ALLOCATE(entry_t, capacity);
  // Clears the count, as we'll get rid of the tombstone buckets.
  table->count = 0;
  for (int i = 0; i < capacity; i++) {
    // initiate the new array with empty buckets in all indexes.
    entries[i].key = NULL;
    entries[i].value = NULL_VAL;
  }
  for (int i = 0; i < table->capacity; i++) {
    // walk the old array and move the existing elements to their new buckets
    // in the new array.
    // All the entries need to be copied back over since the hash function
    // depends on the capacity. If we change the capacity without copying the
    // entries to their new positions, we risk the problem of not being able
    // to find entries in the new array.
    entry_t* entry = &table->entries[i];
    if (entry->key == NULL) continue;
    entry_t* dest = find_entry(entries, capacity, entry->key);
    dest->key = entry->key;
    dest->value = entry->value;
    table->count++;
  }
  // free the old array.
  FREE_ARRAY(entry_t, table->entries, table->capacity);
  table->entries = entries;
  table->capacity = capacity;
}


bool table_set(table_t* table, obj_string_t* key, value_t value) {
  if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
    int capacity = GROW_CAPACITY(table->capacity);
    adjust_capacity(table, capacity);
  }
  entry_t* entry = find_entry(table->entries, table->capacity, key);
  bool is_new_key = entry->key == NULL;
  if (is_new_key && IS_NULL(entry->value)) {
    // not a tombstone.
    table->count++;
  }
  entry->key = key;
  entry->value = value;
  return is_new_key;
}


bool table_delete(table_t* table, obj_string_t* key) {
  if (table->count == 0) return false;

  // find the entry.
  entry_t* entry = find_entry(table->entries, table->capacity, key);
  if (entry->key == NULL) return false;

  // place a tombstone in the entry.
  entry->key = NULL;
  entry->value = BOOL_VAL(true);
  return true;
}


void table_add_all(table_t* from, table_t* to) {
  // move all entries from a table to a different one.
  for (int i = 0; i < from->capacity; i++) {
    entry_t* entry = &from->entries[i];
    if (entry->key != NULL) {
      table_set(to, entry->key, entry->value);
    }
  }
}


obj_string_t* table_find_string(table_t* table, const char* chars, int length, uint32_t hash) {
  if (table->count == 0) return NULL;

  uint32_t index = hash % table->capacity;
  for (;;) {
    entry_t* entry = &table->entries[index];
    if (entry->key == NULL) {
      // Stop if we find an empty non-tombstone entry.
      if (IS_NULL(entry->value)) return NULL;
    } else if (entry->key->length == length &&
        entry->key->hash == hash &&
        memcmp(entry->key->chars, chars, length) == 0) {
      // We found it.
      return entry->key;
    }

    index = (index + 1) % table->capacity;
  }
}
