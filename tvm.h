#ifndef TVM_H_
#define TVM_H_

#include "tvm_memory.h"
#include "tvm_program.h"
#include "tvm_stack.h"

typedef struct
{
	program* pProgram;

	stack* pStack;

	memory* pMemory;

} virtual_machine;

virtual_machine* create_vm(char* filename);
void destroy_vm(virtual_machine* vm);

void run_vm(virtual_machine* vm);

#endif
