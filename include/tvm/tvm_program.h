#ifndef TVM_PROGRAM_H_
#define TVM_PROGRAM_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tvm_htab.h"
#include "tvm_memory.h"

typedef struct tvm_program_s
{
	int start;

	int num_instructions;
        int *instr;

        int ***args;

	int **values;
	int num_values;

	tvm_htab_t *label_htab;

} tvm_program_t;

/* Create and initialize an empty program object */
tvm_program_t *program_create();

/* Interpret a source file into bytecode, and store it in a program object */
int program_interpret(tvm_program_t *p, char *filename, tvm_memory_t *pMemory);

void program_destroy(tvm_program_t *p);

#endif
