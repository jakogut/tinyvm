#ifndef VIRTUAL_MACHINE_H_
#define VIRTUAL_MACHINE_H_

#include "memory.h"
#include "program.h"
#include "stack.h"
#include "instructions.h"
#include "instruction_table.h"

typedef struct
{
	program* pProgram;
	instruction_table instr_table;

	stack* pStack;

	memory* pMemory;

} virtual_machine;

virtual_machine* create_vm(char* filename);
void destroy_vm(virtual_machine* vm);

void run_vm(virtual_machine* vm);

#endif
