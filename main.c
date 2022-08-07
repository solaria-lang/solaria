#include "common.h"
#include "chunk.h"
#include "debug.h"


int main(int argc, const char* argv[]) {
  chunk_t chunk;
  init_chunk(&chunk);

  int constant_index = add_constant(&chunk, 1.2);
  write_chunk(&chunk, OP_CONSTANT, 777);
  write_chunk(&chunk, constant_index, 777);

  for (double i = 0.0; i < 300; i++) {
    // this shows the limitation of having an OP_CONSTANT that only
    // accepts up to one byte (256 indexes) of data.
    int index = add_constant(&chunk, i);
    write_chunk(&chunk, OP_CONSTANT, 777);
    write_chunk(&chunk, index, 777);
  }

  write_chunk(&chunk, OP_RETURN, 777);

  disassemble_chunk(&chunk, "test chunk");

  free_chunk(&chunk);
  return 0;
}
