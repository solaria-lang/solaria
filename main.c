#include "common.h"
#include "chunk.h"
#include "debug.h"


int main(int argc, const char* argv[]) {
  chunk_t chunk;
  init_chunk(&chunk);
  int constant_index = add_constant(&chunk, 1.2);
  write_chunk(&chunk, OP_CONSTANT);
  write_chunk(&chunk, constant_index);
  write_chunk(&chunk, OP_RETURN);
  disassemble_chunk(&chunk, "test chunk");
  free_chunk(&chunk);
  return 0;
}
