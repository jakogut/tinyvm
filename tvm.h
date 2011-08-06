#ifndef TVM_H_
#define TVM_H_

#include <stddef.h>

#include "tvm_memory.h"
#include "tvm_program.h"
#include "tvm_stack.h"

typedef struct tvm_s
{
	tvm_program_t* pProgram;
	tvm_memory_t* pMemory;
} tvm_t;

tvm_t* create_vm(char* filename);
void destroy_vm(tvm_t* vm);

void run_vm(tvm_t* vm);

#endif
