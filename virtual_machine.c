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
		switch(vm->pProgram->instr[instr_idx])
		{
		case MOV:
			*arg0 = *arg1;
			break;
		case PUSH:
			stack_push(vm->pStack, arg0);
			break;
		case POP:
			stack_pop(vm->pStack, arg0);
			break;
		case INC:
			++(*arg0);
			break;
		case DEC:
			--(*arg0);
			break;
		case ADD:
			*arg0 += *arg1;
			break;
		case SUB:
			*arg0 -= *arg1;
			break;
		case MUL:
			*arg0 *= *arg1;
			break;
		case DIV:
			*arg0 /= *arg1;
			break;
		case MOD:
			vm->pMemory->remainder = *arg0 % *arg1;
			break;
		case REM:
			*arg0 = vm->pMemory->remainder;
			break;
		case NOT:
			*arg0 = ~(*arg0);
			break;
		case XOR:
			*arg0 ^= *arg1;
			break;
		case OR:
			*arg0 |= *arg1;
			break;
		case AND:
			*arg0 &= *arg1;
			break;
		case SHL:
			*arg0 <<= *arg1;
			break;
		case SHR:
			*arg0 >>= *arg1;
			break;
		case CMP:
			vm->pMemory->equal = (*arg0 == *arg1);
			vm->pMemory->greater = (*arg0 > *arg1);
			break;
		case JMP:
			instr_idx = *arg0 - 1;
			break;
		case JE:
			if(vm->pMemory->equal) instr_idx = *arg0 - 1;
			break;
		case JNE:
			if(!vm->pMemory->equal) instr_idx = *arg0 - 1;
			break;
		case JG:
			if(vm->pMemory->greater) instr_idx = *arg0 - 1;
			break;
		case JGE:
			if(vm->pMemory->greater || vm->pMemory->equal) instr_idx = *arg0 - 1;
			break;
		case JL:
			if(!vm->pMemory->greater && !vm->pMemory->equal) instr_idx = *arg0 - 1;
			break;
		case JLE:
			if(!vm->pMemory->greater) instr_idx = *arg0 - 1;
			break;
		}
	}
}
