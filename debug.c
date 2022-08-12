#include <stdio.h>


#include "chunk.h"
#include "debug.h"
#include "memory.h"
#include "lines.h"


void disassemble_chunk(chunk_t* chunk, const char* name) {
  printf("== %s ==\n", name);
  for (int offset = 0; offset < chunk->count;) {
    offset = disassemble_instruction(chunk, offset);
  }
}


int disassemble_instruction(chunk_t* chunk, int offset) {
  printf("chunk_idx=%04d ", offset);
  printf("line=%d ", get_line_num(&chunk->lines, offset));
  printf("op_code=");

  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
    case OP_CONSTANT:
      return constant_instruction("OP_CONSTANT", chunk, offset);
    case OP_CONSTANT_LONG:
      return constant_long_instruction("OP_CONSTANT_LONG", chunk, offset);
    case OP_NEGATE:
      return simple_instruction("OP_NEGATE", offset);
    case OP_ADD:
      return simple_instruction("OP_ADD", offset);
    case OP_SUBTRACT:
      return simple_instruction("OP_SUBTRACT", offset);
    case OP_MULTIPLY:
      return simple_instruction("OP_MULTIPLY", offset);
    case OP_DIVIDE:
      return simple_instruction("OP_DIVIDE", offset);
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
  printf("%s const_idx=%d, value='", name, constant_index);
  print_value(chunk->constants.values[constant_index]);
  printf("'\n");
  return offset + 2;
}


static int constant_long_instruction(const char* name, chunk_t* chunk, int offset) {
  // The constant itself for the OP_CONSTANT_LONG op_code is the next value in
  // the array of codes.
  uint8_t* constant_index_p = &(chunk->code[offset + 1]);
  // recast from byte to uint32.
  uint32_t constant_index = UINT8_POINTER_TO_UINT32(constant_index_p);
  printf("%s const_idx=%d, value='", name, constant_index);
  print_value(chunk->constants.values[constant_index]);
  printf("'\n");
  // 1 byte for the operator and 4 bytes for the operation (uint32_t).
  return offset + 5;
}
