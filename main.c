#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"


int main(int argc, const char* argv[]) {
  init_vm();

  chunk_t chunk;
  init_chunk(&chunk);

  int line = 777;

  // This test represents -( (1.2 + 3.4) / 5.6 ) = -0.82...
  write_constant(&chunk, 1.2, line);
  write_constant(&chunk, 3.4, line);
  write_chunk(&chunk, OP_ADD, line);
  write_constant(&chunk, 5.6, line);
  write_chunk(&chunk, OP_DIVIDE, line);
  write_chunk(&chunk, OP_NEGATE, line);
  write_chunk(&chunk, OP_RETURN, 777);

  // disassemble_chunk(&chunk, "test chunk");
  interpret(&chunk);

  free_vm();
  free_chunk(&chunk);
  return 0;
}
