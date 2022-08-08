#include "lines.h"
#include "common.h"
#include "memory.h"


void init_lines(lines_t* lines) {
  lines->count = 0;
  lines->capacity = 0;
  lines->line_num = NULL;
  lines->num_of_reps = NULL;
}


void add_line(lines_t* lines, int line_num) {
  /*
   * Similar to a run-length encoding algorithm.
   * When adding a line the algorithm verifies:
   * - If the arrays are empty, initialise with the line num and reps = 1
   * - If the line num is the same as the last, only increase the reps.
   * - Else start a new element in the array with the line num and reps = 1.
   */
  if (lines->capacity < lines->count + 1) {
    int old_capacity = lines->capacity;
    lines->capacity = GROW_CAPACITY(old_capacity);
    lines->line_num = GROW_ARRAY(int, lines->line_num, old_capacity, lines->capacity);
    lines->num_of_reps = GROW_ARRAY(int, lines->num_of_reps, old_capacity, lines->capacity);
  }
  if (lines->count > 0 && lines->line_num[lines->count - 1] == line_num) {
    lines->num_of_reps[lines->count - 1]++;
  } else {
    lines->line_num[lines->count] = line_num;
    lines->num_of_reps[lines->count] = 1;
  }
  lines->count++;
}


void free_lines(lines_t* lines) {
  FREE_ARRAY(int, lines->line_num, lines->capacity);
  FREE_ARRAY(int, lines->num_of_reps, lines->capacity);
  // Call init_lines to leave the type in a nice well-defined empty state.
  init_lines(lines);
}


int get_line_num(lines_t* lines, int index) {
  /*
   * Similar to a run-length decoding algorithm.
   * Finds the real line number for the given index by checking whether the
   * index is within the range of the line analysed
   */
  int lines_counter = 0;
  int matched_line_num = -1;

  for (int i=0; i < lines->count; i++) {
    int lines_so_far = lines->num_of_reps[i] + lines_counter;
    if (index <= lines_so_far) {
      matched_line_num = lines->line_num[i];
      break;
    } else {
      lines_counter = lines_so_far;
    }
  }
  return matched_line_num;
}
