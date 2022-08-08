#include <stdlib.h>

#include "chunk.h"
#include "memory.h"
#include "value.h"
#include "lines.h"


void init_chunk(chunk_t* chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  init_value_array(&chunk->constants);
  init_lines(&chunk->lines);
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
  }
  chunk->code[chunk->count] = byte;
  chunk->count++;
  add_line(&chunk->lines, line);
}


void free_chunk(chunk_t* chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  free_value_array(&chunk->constants);
  free_lines(&chunk->lines);
  // Call init_chunk to leave the chunk in a nice well-defined empty state.
  init_chunk(chunk);
}


int add_constant(chunk_t* chunk, value_t value) {
  write_value_array(&chunk->constants, value);
  // return the index of the constant just added so that future reference can
  // use it.
  return chunk->constants.count - 1;
}


void write_constant(chunk_t* chunk, value_t value, int line) {
  int constant_index = add_constant(chunk, value);
  if (constant_index < MAX_OP_CONSTANT_NUM_INDEXES) {
    write_chunk(chunk, OP_CONSTANT, line);
    write_chunk(chunk, constant_index, line);
  } else {
    write_chunk(chunk, OP_CONSTANT_LONG, line);
    // the uint32_t needs to be recast to a byte, and each byte then needs to
    // be pushed to the chunk.
    uint8_t* index_as_byte_array = (uint8_t*) &constant_index;
    for (int i=0; i < 4; i++) {
      write_chunk(chunk, index_as_byte_array[i], line);
    }
  }
}
