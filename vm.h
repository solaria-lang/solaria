#ifndef SOLARIA_VM_H
#define SOLARIA_VM_H

#include "chunk.h"


/*
 * This is a limitation. It's possible that some sequence of instructions end
 * up pushing too many values and run out of stack space a.k.a "stack
 * overflow". An improvement would be to grow the stack dynamically as needed.
 */
#define STACK_MAX 256


typedef struct vm_t {
  chunk_t* chunk;
  /*
   * This variable keeps track of the instruction about to be executed. If
   * we want to squeeze more speed, we'd move this out of the struct and onto
   * a local variable closer to the function that uses it, so that this
   * variable could be kept in a register. The name *ip* is a traditional name
   * in computer science and it means "instruction pointer". A pointer to a
   * byte is used instead of an integer index because it's faster to
   * dereference a pointer than look up an element in an array by index.
   */
  uint8_t* ip;
  /*
   * This is a stack-based virtual machine. This decision will help us when
   * executing instructions with left-to-right evaluation.
   */
  value_t stack[STACK_MAX];
  /*
   * As the stack gets evaluated and thus shrinks/grows, the stack_top will
   * help keeping track of where the top of the stack is. Analogously to "ip",
   * we chose a pointer rather than an array index since dereference a pointer
   * is faster than looking up an index. This pointer will point at the array
   * element immediately past the top of the stack, which means we can verify
   * that the stack is empty by pointing at element zero in the array, and we
   * can interpret this point as "the place where the next value to be pushed
   * into the stack will go".
   */
  value_t* stack_top;
} vm_t;


typedef enum interpret_result_t {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} interpret_result_t;


void init_vm();
void free_vm();
interpret_result_t interpret(const char* source);
void push(value_t value);
value_t pop();

#endif
