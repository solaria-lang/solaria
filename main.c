#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"


int main(int argc, const char* argv[]) {
  init_vm();

  chunk_t chunk;
  init_chunk(&chunk);

  for (double i = 0.0; i < 10; i++) {
    write_constant(&chunk, i, 777);
    write_chunk(&chunk, OP_NEGATE, 777);
  }
  write_chunk(&chunk, OP_RETURN, 777);

  // disassemble_chunk(&chunk, "test chunk");
  interpret(&chunk);

  free_vm();
  free_chunk(&chunk);
  return 0;
}
