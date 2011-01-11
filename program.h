#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hash_table.h"
#include "memory.h"

typedef struct
{
        unsigned int num_instructions;
        unsigned int* instr;

	hash_table* label_htab;

	int** values;
	int num_values;

        int*** args;

} program;

program* create_program(char* filename, memory* pMemory);
void destroy_program(program* p);

void print_warning(char* str, int line_number);
void print_error(char* str, int line_number);

#endif
