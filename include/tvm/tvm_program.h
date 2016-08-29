#ifndef TVM_PROGRAM_H_
#define TVM_PROGRAM_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tvm_htab.h"
#include "tvm_memory.h"

struct tvm_prog {
	int start;

	int num_instr;
	int *instr;
	int ***args;

	int **values;
	int num_values;

	struct tvm_htab_ctx *defines;
	struct tvm_htab_ctx *label_htab;
};

/* Create and initialize an empty program object */
struct tvm_prog *tvm_prog_create();

void tvm_prog_destroy(struct tvm_prog *p);

#endif
