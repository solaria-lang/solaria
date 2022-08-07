// Like in Lua, Solaria processes "chunks" of code.

#ifndef SOLARIA_CHUNK_H
#define SOLARIA_CHUNK_H

#include "common.h"
#include "value.h"

// In Solaria's bytecode format, each instruction has a one-byte *operation
// code* (sometimes shortened to just *opcode*). That one-byte code controls
// the type of instructions we'll deal with (add, subtract, lookup, etc.)

typedef enum op_code_t {
  // Instruction to produce a particular constant. This instruction accepts a 1
  // byte operand, which specifies which constant to load from a chunk.
  OP_CONSTANT,
  // return from the current function.
  OP_RETURN,
} op_code_t;


typedef struct chunk_t {
  // `code` is just an array of bytes. As we don't know how big the array needs
  // to be before a chunk is compiled, it must be dynamic.
  uint8_t* code;
  // `constants` will store "literal" values that we need for computation.
  // E.g., values like "3" and "4" so that operations such "3 + 4" can take
  // place. This is often called a "constant pool", because you draw constants
  // from it.
  value_array_t constants;
  // `capacity` holds the number of elements we have allocated to `code`.
  // A trick scenario happens when we don't have more `capacity` to add new
  // entries. In this case the following steps are performed:
  // 1. Allocate a new array with more capacity.
  // 2. Copy the existing elements from the old array to the new one.
  // 3. Store the new capacity.
  // 4. Delete the old array.
  // 5. Update `code` to point to the new array.
  int capacity;
  // `count` holds the number of allocated elements that are actually in use.
  // This is useful when `count` is less than `capacity`, as a new element
  // can be stored in an available space in the array. The `count` value is
  // incremented then.
  int count;
} chunk_t;


void init_chunk(chunk_t* chunk);
void write_chunk(chunk_t* chunk, uint8_t byte);
void free_chunk(chunk_t* chunk);
int add_constant(chunk_t* chunk, value_t value);

#endif
