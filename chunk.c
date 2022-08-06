#include <stdlib.h>

#include "chunk.h"
#include "memory.h"


void init_chunk(chunk_t* chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
}


void write_chunk(chunk_t* chunk, uint8_t byte) {
  /*
   * In order to write a chunk, the existing `chunk_t` must verified so that
   * its size can be analysed.
   *
   * If the capacity has almost reached maximum, i.e, if the next chunk of
   * bytecode getting added in a subsequent function call would fill up the
   * array, a detour is done to grow the array of chunks.
   *
   * Otherwise, we happily add the chunk to the next available space.
   */
  if (chunk->capacity < chunk->count + 1) {
    int old_capacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(old_capacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code, old_capacity, chunk->capacity);
  }
  chunk->code[chunk->count] = byte;
  chunk->count++;
}

void free_chunk(chunk_t* chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  // Call init_chunk to leave the chunk in a nice well-defined empty state.
  init_chunk(chunk);
}
