#include <stdlib.h>

#include "chunk.h"
#include "memory.h"
#include "value.h"


void init_chunk(chunk_t* chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  init_value_array(&chunk->constants);
}


void write_chunk(chunk_t* chunk, uint8_t byte, int line) {
  /*
   * In order to write a chunk, the existing `chunk_t` must be checked so that
   * its size can be analysed.
   *
   * If the capacity has almost reached maximum, i.e, if the next chunk of
   * bytecode getting added in a subsequent function call would fill up the
   * array, a detour is done to grow the array of chunks.
   *
   * Otherwise, we happily add the chunk to the next available space.
   *
   * Analogously, the line number from the source code is added to the struct
   * to aid the end user in an eventual crash incident.
   */
  if (chunk->capacity < chunk->count + 1) {
    int old_capacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(old_capacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code, old_capacity, chunk->capacity);
    chunk->lines = GROW_ARRAY(int, chunk->lines, old_capacity, chunk->capacity);
  }
  chunk->code[chunk->count] = byte;
  chunk->lines[chunk->count] = line;
  chunk->count++;
}


void free_chunk(chunk_t* chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  free_value_array(&chunk->constants);
  // Call init_chunk to leave the chunk in a nice well-defined empty state.
  init_chunk(chunk);
}


int add_constant(chunk_t* chunk, value_t value) {
  write_value_array(&chunk->constants, value);
  // return the index of the constant just added so that future reference can
  // use it.
  return chunk->constants.count - 1;

}
