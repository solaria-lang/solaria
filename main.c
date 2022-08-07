#include "common.h"
#include "chunk.h"
#include "debug.h"


int main(int argc, const char* argv[]) {
  chunk_t chunk;
  init_chunk(&chunk);

  for (double i = 0.0; i < 300; i++) {
    write_constant(&chunk, i, 777);
  }

  write_chunk(&chunk, OP_RETURN, 777);

  disassemble_chunk(&chunk, "test chunk");

  free_chunk(&chunk);
  return 0;
}
