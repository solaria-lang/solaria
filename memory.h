#ifndef SOLARIA_MEMORY_H
#define SOLARIA_MEMORY_H


#include "common.h"


#define GROW_CAPACITY(capacity) \
  ((capacity) < 8 ? 8 : (capacity) * 2)


#define GROW_ARRAY(type, pointer, old_count, new_count) \
  (type*)reallocate(pointer, sizeof(type) * (old_count), \
      sizeof(type) * (new_count))


#define FREE_ARRAY(type, pointer, old_count) \
  reallocate(pointer, sizeof(type) * (old_count), 0)


#define UINT8_POINTER_TO_UINT32(uint8_t_p) \
  *((uint32_t*) uint8_t_p)


void* reallocate(void* pointer, size_t old_size, size_t new_size);

#endif
