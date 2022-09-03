#ifndef SOLARIA_OBJECT_H
#define SOLARIA_OBJECT_H

#include "common.h"
#include "value.h"


typedef enum obj_type_t {
  OBJ_STRING,
} obj_type_t;


// Re-definition of `value.h::obj_t` due to cyclic dependencies.
struct obj_t {
  obj_type_t type;
  struct obj_t* next;
};


struct obj_string_t {
  // obj is declared first on purpose, so that a pointer to obj_string_t can
  // be converted back to obj_t safely and vice-versa. This comes from the fact
  // that C pointers will point to the initial member of a struct.
  // See `AS_CSTRING` macro below for an example.
  obj_t obj;
  int length;
  char* chars;
  // we'll cash the hash in the object, so that we don't need to generate it
  // every time a string needs to be hashed.
  uint32_t hash;
};


// Similar to the macros defined in values.h
#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_STRING(value) is_obj_type(value, OBJ_STRING)

#define AS_STRING(value) ((obj_string_t*)AS_OBJ(value))
#define AS_CSTRING(value) (((obj_string_t*)AS_OBJ(value))->chars)


static inline bool is_obj_type(value_t value, obj_type_t type) {
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}


obj_string_t* take_string(char* chars, int length);
obj_string_t* copy_string(const char* chars, int length);
void print_object(value_t value);

#endif
