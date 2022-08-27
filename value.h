#ifndef SOLARIA_VALUE_H
#define SOLARIA_VALUE_H


#include "common.h"


// This value types only cover _built-in_ types. They do not cover classes.
typedef enum value_type_t {
  VAL_BOOL,
  VAL_NULL,
  VAL_NUMBER,
} value_type_t;


typedef struct value_t {
  value_type_t type;
  // We use a union rather than distinct fields in the struct here because it
  // saves memory.
  union {
    bool boolean;
    double number;
  // `as` is used for the name of the union field because it reads nicely,
  // almost like a cast.
  } as;
} value_t;

// Macros for handling value types.
#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NULL(value) ((value).type == VAL_NULL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)

// Macros for converting a native C value to a Solaria value.
// Note that since C99 we can use "designated initialisation" to initialise
// structs by keywords directly, thus the use of `.boolean = value`
#define BOOL_VAL(value) ((value_t){VAL_BOOL, {.boolean = value}})
#define NULL_VAL ((value_t){VAL_NULL, {.number = 0}})
#define NUMBER_VAL(value) ((value_t){VAL_NUMBER, {.number = value}})

// Analogously to the above, this converts Solaria values back to C.
// Note that there's no "AS_NULL" because VAL_NULL doesn't carry extra data.
#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)


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


bool values_equal(value_t a, value_t b);
void init_value_array(value_array_t* array);
void write_value_array(value_array_t* array, value_t value);
void free_value_array(value_array_t* array);
void print_value(value_t value);


#endif
