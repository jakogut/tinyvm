#ifndef INSTRUCTION_TABLE_H_
#define INSTRUCTION_TABLE_H_

#include "instruction_index.h"

#include "program.h"
#include "memory.h"
#include "stack.h"

// A type defining an array of pointers to instruction functions
typedef void (*instruction_table[NUM_INSTRUCTIONS])(program* p, memory* m, stack* s, int* instr_idx);

#endif
