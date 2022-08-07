#ifndef SOLARIA_DEBUG_H
#define SOLARIA_DEBUG_H


#include "chunk.h"


static int simple_instruction(const char* name, int offset);
static int constant_instruction(const char* name, chunk_t* chunk, int offset);

int disassemble_instruction(chunk_t* chunk, int offset);
void disassemble_chunk(chunk_t* chunk, const char* name);


#endif
