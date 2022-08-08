#ifndef SOLARIA_LINES_H
#define SOLARIA_LINES_H


typedef struct lines_t {
  /*
   * This struct is aimed to hold a run-length encoding version of lines.
   * Often times, chunk_t will hold many codes and constants for the same line.
   * Storing lines as an array of integers is too memory consuming. Instead,
   * this approach saves memory in exchange for some CPU cost for encoding and
   * decoding the line for a particular instruction. However, the decoding part
   * will only ever happen in debug mode, or when an error in the source code
   * is raised and its line number needs to be returned to the user.
   */

  int* line_num;
  // Amount of times a line number was repeated for a sequence of instructions
  int* num_of_reps;
  int count;
  int capacity;
} lines_t;


void init_lines(lines_t* lines);
void free_lines(lines_t* lines);
void add_line(lines_t* lines, int line_num);
int get_line_num(lines_t* lines, int index);


#endif
