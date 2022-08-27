#include "common.h"
#include "vm.h"
#include <stdio.h>
#include <stdarg.h>
#include "compiler.h"
#include "debug.h"


/*
 * The virtual machine is a singleton.
 * At first glance, this will make it simpler to implement the language. It
 * would be a more mature idea to operate the vm as a pointer and pass it
 * around to functions. This can be a future improvement.
 */
vm_t vm;

static void reset_stack() {
  vm.stack_top = vm.stack;
}


static void runtime_error(const char* format, ...) {
  // Variadic functions take a varying number of arguments.
  // Thus we will use the variadic flavour of printf called vfprintf
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputs("\n", stderr);

  size_t instruction = vm.ip - vm.chunk->code -1;
  int line = get_line_num(&(vm.chunk->lines), instruction);
  fprintf(stderr, "[line %d] in script\n", line);
  reset_stack();
}


void init_vm() {
  reset_stack();
}


void free_vm() {
}


void push(value_t value) {
  *vm.stack_top = value;
  vm.stack_top++;
}


value_t pop() {
  vm.stack_top--;
  return *vm.stack_top;
}


static value_t peek(int distance) {
  // `distance` is how far down from the top of the stack to look:
  // zero is the top, one is one slot down, etc.
  return vm.stack_top[-1 - distance];
}


static interpret_result_t run() {
// returns the byte currently pointed by ip and then advances the
// instruction pointer to the next byte.
#define READ_BYTE() (*vm.ip++)
// calls READ_BYTE, treating the result number as an index for the constants
// array. Returns the constant value from the constant pool.
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
// The following macro comes in a do/while block so that it can be used with
// trailing ";" without causing problems even if inside `if` conditionals.
#define BINARY_OP(valueType, op) \
    do { \
      if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
        runtime_error("Operands must be numbers."); \
        return INTERPRET_RUNTIME_ERROR; \
      } \
      double b = AS_NUMBER(pop()); \
      double a = AS_NUMBER(pop()); \
      push(valueType(a op b)); \
    } while (false)

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("        ");
    for (value_t* slot = vm.stack; slot < vm.stack_top; slot++) {
      printf("[ ");
      print_value(*slot);
      printf(" ]");
    }
    printf("\n");
    // pointer math to convert ip back to a relative offset from the beginning
    // of the bytecode.
    disassemble_instruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
      case OP_CONSTANT: {
        value_t constant = READ_CONSTANT();
        push(constant);
        break;
      };
      case OP_NEGATE:
        if (!IS_NUMBER(peek(0))) {
          runtime_error("Operant must be a number.");
          return INTERPRET_RUNTIME_ERROR;
        }
        push(NUMBER_VAL(-AS_NUMBER(pop())));
        break;
      case OP_ADD: {
        BINARY_OP(NUMBER_VAL, +);
        break;
      }
      case OP_SUBTRACT: {
        BINARY_OP(NUMBER_VAL, -);
        break;
      }
      case OP_MULTIPLY: {
        BINARY_OP(NUMBER_VAL, *);
        break;
      }
      case OP_DIVIDE: {
        BINARY_OP(NUMBER_VAL, /);
        break;
      }
      case OP_RETURN: {
        print_value(pop());
        printf("\n");
        return INTERPRET_OK;
      }
    }
  }
#undef BINARY_OP
#undef READ_BYTE
#undef READ_CONSTANT
}


interpret_result_t interpret(const char* source) {
  chunk_t chunk;
  init_chunk(&chunk);

  if (!compile(source, &chunk)) {
    free_chunk(&chunk);
    return INTERPRET_COMPILE_ERROR;
  }

  vm.chunk = &chunk;
  vm.ip = vm.chunk->code;

  interpret_result_t result = run();

  free_chunk(&chunk);
  return result;
}
