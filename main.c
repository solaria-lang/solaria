#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"


static void repl() {
  /*
   * A good REPL handles multi-line input and doesn't have a hardcoded line
   * length limit. This is not the case of this repl. It is very bare at the
   * moment.
   */
  char line[1024];
  for (;;) {
    printf("> ");
    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }
    interpret(line);
  }
}


static char* read_file(const char* path) {
  FILE* file = fopen(path, "rb");
  if (file == NULL) {
    // file doesn't exist or user doesn't have access to it.
    fprintf(stderr, "Could not open file \"%s\".\n", path);
    exit(74);
  }

  fseek(file, 0L, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  // read the whole file + 1 to save space for the null byte.
  char* buffer = (char*)malloc(file_size + 1);
  if (buffer == NULL) {
    fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
    exit(74);
  }

  size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
  if (bytes_read < file_size) {
    fprintf(stderr, "Could not read file \"%s\".\n", path);
    exit(74);
  }

  buffer[bytes_read] = '\0';

  fclose(file);
  return buffer;
}

static void run_file(const char* path) {
  char* source = read_file(path);
  interpret_result_t result = interpret(source);
  free(source);

  if (result == INTERPRET_COMPILE_ERROR) exit(65);
  if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}


int main(int argc, const char* argv[]) {
  init_vm();

  if (argc == 1) {
    // No arguments. Drop the caller in the REPL shell.
    repl();
  } else if (argc == 2) {
    // A single argument is passed, meaning that this is the path of a solaria
    // script to be run.
    run_file(argv[1]);
  } else {
    fprintf(stderr, "Usage: solaria: [path]\n");
    exit(64);
  }

  free_vm();
  return 0;
}


void test_arithmetic_instruction_1();
void test_arithmetic_instruction_2();
void test_arithmetic_instruction_3();
void test_arithmetic_instruction_4();
void test_arithmetic_instruction_5();
void test_vm_stack_overflow();

int old_main(int argc, const char* argv[]) {
  init_vm();

  test_arithmetic_instruction_1(); // -0.82
  test_arithmetic_instruction_2(); // 5
  test_arithmetic_instruction_3(); // 9
  test_arithmetic_instruction_4(); // 0
  test_arithmetic_instruction_5(); // -1
  test_vm_stack_overflow();

  free_vm();
  return 0;
}


void test_arithmetic_instruction_1() {
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

  // interpret(&chunk);
  free_chunk(&chunk);
}


void test_arithmetic_instruction_2() {
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

  // interpret(&chunk);
  free_chunk(&chunk);
}


void test_arithmetic_instruction_3() {
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

  // interpret(&chunk);
  free_chunk(&chunk);
}


void test_arithmetic_instruction_4() {
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

  // interpret(&chunk);
  free_chunk(&chunk);
}


void test_arithmetic_instruction_5() {
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

  // interpret(&chunk);
  free_chunk(&chunk);
}


void test_vm_stack_overflow() {
  // Our vm stack only supports 256 values at the moment.
  chunk_t chunk;
  int line = 1;
  init_chunk(&chunk);

  for (int i=0; i < 130; i++) {
    write_constant(&chunk, 1, line);
    write_constant(&chunk, 1, line);
    write_chunk(&chunk, OP_SUBTRACT, line);
  }
  write_chunk(&chunk, OP_RETURN, line);

  // interpret(&chunk);
  free_chunk(&chunk);
}
