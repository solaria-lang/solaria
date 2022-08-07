#include <stdio.h>


#include "chunk.h"
#include "debug.h"


void disassemble_chunk(chunk_t* chunk, const char* name) {
  printf("== %s ==\n", name);
  for (int offset = 0; offset < chunk->count;) {
    offset = disassemble_instruction(chunk, offset);
  }
}


int disassemble_instruction(chunk_t* chunk, int offset) {
  printf("%04d ", offset);

  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
    case OP_CONSTANT:
      return constant_instruction("OP_CONSTANT", chunk, offset);
    case OP_RETURN:
      return simple_instruction("OP_RETURN", offset);
    default:
      printf("Unknown opcode %d\n", instruction);
      return offset + 1;
  }
}


static int simple_instruction(const char* name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}


static int constant_instruction(const char* name, chunk_t* chunk, int offset) {
  // The constant itself for the OP_CONSTANT op_code is the next value in the
  // array of codes.
  uint8_t constant_index = chunk->code[offset + 1];
  printf("%-16s %4d '", name, constant_index);
  print_value(chunk->constants.values[constant_index]);
  printf("'\n");
  return offset + 2;
}
