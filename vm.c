#include "common.h"
#include "vm.h"
#include <stdio.h>
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


static interpret_result_t run() {
// returns the byte currently pointed by ip and then advances the
// instruction pointer to the next byte.
#define READ_BYTE() (*vm.ip++)
// calls READ_BYTE, treating the result number as an index for the constants
// array. Returns the constant value from the constant pool.
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
// The following macro comes in a do/while block so that it can be used with
// trailing ";" without causing problems even if inside `if` conditionals.
#define BINARY_OP(op) \
  do { \
    value_t last_value = pop(); \
    value_t first_value = pop(); \
    push(first_value op last_value); \
  } while(false) ;

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
      case OP_NEGATE: {
        push(-pop());
        break;
      }
      case OP_ADD: {
        BINARY_OP(+);
        break;
      }
      case OP_SUBTRACT: {
        BINARY_OP(-);
        break;
      }
      case OP_MULTIPLY: {
        BINARY_OP(*);
        break;
      }
      case OP_DIVIDE: {
        BINARY_OP(/);
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


interpret_result_t interpret(chunk_t* chunk) {
  vm.chunk = chunk;
  vm.ip = chunk->code;
  return run();
}
