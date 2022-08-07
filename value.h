#ifndef SOLARIA_VALUE_H
#define SOLARIA_VALUE_H


#include "common.h"


typedef double value_t;


typedef struct value_array_t {
  /*
   * This struct holds a "constant pool". E.g., it stores literals like "3"
   * and "4" so that operations like "3 + 4" can draw these values from the
   * pool directly. To compile such operations to bytecode, an instruction like
   * "produce a constant" is required, and these literals need to be stored
   * somewhere. This structure is this "somewhere".
   */
  int capacity;
  int count;
  value_t* values;
} value_array_t;


void init_value_array(value_array_t* array);
void write_value_array(value_array_t* array, value_t value);
void free_value_array(value_array_t* array);
void print_value(value_t value);


#endif
