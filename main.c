#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

void arithmetic_instruction_1();
void arithmetic_instruction_2();
void arithmetic_instruction_3();
void arithmetic_instruction_4();
void arithmetic_instruction_5();


int main(int argc, const char* argv[]) {
  init_vm();

  arithmetic_instruction_1(); // -0.82
  arithmetic_instruction_2(); // 5
  arithmetic_instruction_3(); // 9
  arithmetic_instruction_4(); // 0
  arithmetic_instruction_5(); // -1

  free_vm();
  return 0;
}


void arithmetic_instruction_1() {
  // This test represents the equation -( (1.2 + 3.4) / 5.6 ) = -0.82...
  chunk_t chunk;
  int line = 777;
  init_chunk(&chunk);

  write_constant(&chunk, 1.2, line);
  write_constant(&chunk, 3.4, line);
  write_chunk(&chunk, OP_ADD, line);
  write_constant(&chunk, 5.6, line);
  write_chunk(&chunk, OP_DIVIDE, line);
  write_chunk(&chunk, OP_NEGATE, line);
  write_chunk(&chunk, OP_RETURN, line);

  interpret(&chunk);
  free_chunk(&chunk);
}


void arithmetic_instruction_2() {
  // This test represent the equation 1 * 2 + 3 = 5
  chunk_t chunk;
  int line = 321;
  init_chunk(&chunk);

  write_constant(&chunk, 1, line);
  write_constant(&chunk, 2, line);
  write_chunk(&chunk, OP_MULTIPLY, line);
  write_constant(&chunk, 3, line);
  write_chunk(&chunk, OP_ADD, line);
  write_chunk(&chunk, OP_RETURN, line);

  interpret(&chunk);
  free_chunk(&chunk);
}


void arithmetic_instruction_3() {
  // This test represent the equation 1 + 2 * 3 = 9
  // (remember stack works from left to right)
  chunk_t chunk;
  int line = 123;
  init_chunk(&chunk);

  write_constant(&chunk, 1, line);
  write_constant(&chunk, 2, line);
  write_chunk(&chunk, OP_ADD, line);
  write_constant(&chunk, 3, line);
  write_chunk(&chunk, OP_MULTIPLY, line);
  write_chunk(&chunk, OP_RETURN, line);

  interpret(&chunk);
  free_chunk(&chunk);
}


void arithmetic_instruction_4() {
  // This test represent the equation 3 - 2 - 1 = 0
  chunk_t chunk;
  int line = 555;
  init_chunk(&chunk);

  write_constant(&chunk, 3, line);
  write_constant(&chunk, 2, line);
  write_chunk(&chunk, OP_SUBTRACT, line);
  write_constant(&chunk, 1, line);
  write_chunk(&chunk, OP_SUBTRACT, line);
  write_chunk(&chunk, OP_RETURN, line);

  interpret(&chunk);
  free_chunk(&chunk);
}


void arithmetic_instruction_5() {
  // This test represent the equation 1 + 2 * 3 - 4 / -5 = -1
  // (remember stack works from left to right)
  chunk_t chunk;
  int line = 555;
  init_chunk(&chunk);

  write_constant(&chunk, 1, line);
  write_constant(&chunk, 2, line);
  write_chunk(&chunk, OP_ADD, line);
  write_constant(&chunk, 3, line);
  write_chunk(&chunk, OP_MULTIPLY, line);
  write_constant(&chunk, 4, line);
  write_chunk(&chunk, OP_SUBTRACT, line);
  write_constant(&chunk, 5, line);
  write_chunk(&chunk, OP_NEGATE, line);
  write_chunk(&chunk, OP_DIVIDE, line);

  write_chunk(&chunk, OP_RETURN, line);

  interpret(&chunk);
  free_chunk(&chunk);
}
