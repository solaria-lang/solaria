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
