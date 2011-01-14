#include "virtual_machine.h"

virtual_machine* create_vm(char* filename)
{
	virtual_machine* vm;

	vm = (virtual_machine*)malloc(sizeof(virtual_machine));

	vm->pStack = create_stack();
	vm->pMemory = create_memory(512000);
	vm->pProgram = create_program(filename, vm->pMemory);

	// Make sure the program was interpreted properly
	if(!vm->pProgram) return NULL;

	populate_instruction_table(vm->instr_table);

	return vm;
}

void destroy_vm(virtual_machine* vm)
{
	if(vm)
	{
		if(vm->pMemory) destroy_memory(vm->pMemory);
		if(vm->pStack) destroy_stack(vm->pStack);
		if(vm->pProgram) destroy_program(vm->pProgram);

		free(vm);
	}
}

void run_vm(virtual_machine* vm)
{
	int instr_idx = vm->pProgram->start;

	for(;vm->pProgram->instr[instr_idx] != END; ++instr_idx)
               	vm->instr_table[vm->pProgram->instr[instr_idx]](vm->pProgram, vm->pMemory, vm->pStack, &instr_idx);
}
