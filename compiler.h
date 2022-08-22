#ifndef SOLARIA_COMPILER_H
#define SOLARIA_COMPILER_H

#include "vm.h"
#include "scanner.h"


typedef struct parser_t {
  token_t current;
  token_t previous;
  bool had_error;
  bool panic_mode;
} parser_t;


typedef enum precedence_t {
  // In order from lowest to highest.
  PREC_NONE,
  PREC_ASSIGNMENT,  // =
  PREC_OR,          // or
  PREC_AND,         // and
  PREC_EQUALITY,    // == !=
  PREC_COMPARISON,  // < > <= >=
  PREC_TERM,        // + -
  PREC_FACTOR,      // * /
  PREC_UNARY,       // ! -
  PREC_CALL,        // . ()
  PREC_PRIMARY
} precedence_t;


typedef void (*parse_fn_t)();


typedef struct parse_rule_t {
  parse_fn_t prefix;
  parse_fn_t infix;
  precedence_t precedence;
} parse_rule_t;


bool compile(const char* source, chunk_t* chunk);

#endif
