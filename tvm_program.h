#ifndef TVM_PROGRAM_H_
#define TVM_PROGRAM_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tvm_instr_idx.h"
#include "tvm_hashtab.h"
#include "tvm_memory.h"

typedef struct
{
	int start;

        int* instr;
	int num_instructions;

        int*** args;

	int** values;
	int num_values;

	hash_table* label_htab;
	hash_table* lookup_table;

} program;

program* create_program(char* filename, memory* pMemory);
void destroy_program(program* p);

int* add_value(program* p, const int val);
int parse_value(char* str);
void parse_line(program* p, char** tokens, memory* pMemory);
#endif
