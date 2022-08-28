#include <stdlib.h>


#include "common.h"
#include "object.h"
#include "memory.h"
#include "vm.h"


void* reallocate(void* pointer, size_t old_size, size_t new_size) {
  /*
   * This function is the single stop for all dynamic memory management in
   * Solaria. You can use the macro "GROW_ARRAY" to prettify this function
   * call.
   */
  if (new_size == 0) {
    // Getting rid of this pointer manually.
    // We could have called realloc(pointer, 0), but this type of call (with
    // size 0), isn't portable see `man realloc`.
    free(pointer);
    return NULL;
  }
  void* result = realloc(pointer, new_size);
  if (result == NULL) {
    // No available memory. The program must exit.
    exit(1);
  }
  return result;
};


static void free_object(obj_t* object) {
  switch (object->type) {
    case OBJ_STRING: {
      obj_string_t* string = (obj_string_t*)object;
      FREE_ARRAY(char, string->chars, string->length + 1);
      FREE(obj_string_t, object);
      break;
    }
  }
}


void free_objects() {
  obj_t* object = vm.objects;
  while (object != NULL) {
    obj_t* next = object->next;
    free_object(object);
    object = next;
  }
}
