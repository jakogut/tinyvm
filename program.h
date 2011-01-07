#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hash_table.h"
#include "argument.h"
#include "label.h"

typedef struct
{
        unsigned int num_instructions;
        unsigned int* instr;

	unsigned int num_labels;
	label** pLabels;

        argument** args;

	hash_table* label_htab;

} program;

program* create_program(char* filename);
void destroy_program(program* p);

void print_warning(char* str, int line_number);
void print_error(char* str, int line_number);

#endif
