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
	{
		int *arg0 = vm->pProgram->args[instr_idx][0], *arg1 = vm->pProgram->args[instr_idx][1];

		if(vm->pProgram->instr[instr_idx] == MOV)	*arg0 = *arg1;
		else if(vm->pProgram->instr[instr_idx] == PUSH) stack_push(vm->pStack, arg0);
		else if(vm->pProgram->instr[instr_idx] == POP)	stack_pop(vm->pStack, arg0);
		else if(vm->pProgram->instr[instr_idx] == INC)	++(*arg0);
		else if(vm->pProgram->instr[instr_idx] == DEC)	--(*arg0);
		else if(vm->pProgram->instr[instr_idx] == ADD)	*arg0 += *arg1;
		else if(vm->pProgram->instr[instr_idx] == SUB)	*arg0 -= *arg1;
		else if(vm->pProgram->instr[instr_idx] == MUL)	*arg0 *= *arg1;
		else if(vm->pProgram->instr[instr_idx] == DIV)	*arg0 /= *arg1;
		else if(vm->pProgram->instr[instr_idx] == MOD)	vm->pMemory->remainder = *arg0 % *arg1;
		else if(vm->pProgram->instr[instr_idx] == REM)	*arg0 = vm->pMemory->remainder;
		else if(vm->pProgram->instr[instr_idx] == NOT)	*arg0 = ~(*arg0);
		else if(vm->pProgram->instr[instr_idx] == XOR)	*arg0 ^= *arg1;
		else if(vm->pProgram->instr[instr_idx] == OR)	*arg0 |= *arg1;
		else if(vm->pProgram->instr[instr_idx] == AND)	*arg0 &= *arg1;
		else if(vm->pProgram->instr[instr_idx] == SHL)	*arg0 <<= *arg1;
		else if(vm->pProgram->instr[instr_idx] == SHR)	*arg0 >>= *arg1;
		else if(vm->pProgram->instr[instr_idx] == CMP)	vm->pMemory->FLAGS = ((*arg0 == *arg1) << 32 | (*arg0 > *arg1) << 31);
		else if(vm->pProgram->instr[instr_idx] == JMP)	instr_idx = *arg0 - 1;
		else if(vm->pProgram->instr[instr_idx] == JE
			&& (vm->pMemory->FLAGS & 0x100000000)) 	instr_idx = *arg0 - 1;
		else if(vm->pProgram->instr[instr_idx] == JNE
			&& !(vm->pMemory->FLAGS & 0x100000000))	instr_idx = *arg0 - 1;
		else if(vm->pProgram->instr[instr_idx] == JG
			&& (vm->pMemory->FLAGS & 0x80000000)) 	instr_idx = *arg0 - 1;
		else if(vm->pProgram->instr[instr_idx] == JGE
			&& (vm->pMemory->FLAGS & 0x180000000))	instr_idx = *arg0 - 1;
		else if(vm->pProgram->instr[instr_idx] == JL
			&& !(vm->pMemory->FLAGS & 0x180000000))	instr_idx = *arg0 - 1;
		else if(vm->pProgram->instr[instr_idx] == JLE
			&& !(vm->pMemory->FLAGS & 0x80000000))	instr_idx = *arg0 - 1;
	}
}
