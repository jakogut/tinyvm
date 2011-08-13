#include "tvm.h"

tvm_t* create_vm(char* filename)
{
	tvm_t* vm = (tvm_t*)malloc(sizeof(tvm_t));

	vm->pMemory = create_memory(MIN_MEMORY_SIZE);
	vm->pProgram = create_program();

	create_stack(vm->pMemory, MIN_STACK_SIZE);

	if(interpret_program(vm->pProgram, filename, vm->pMemory) != 0) return NULL;

	if(!vm || !vm->pMemory || !vm->pProgram) return NULL;

	return vm;
}

void destroy_vm(tvm_t* vm)
{
	if(vm && vm->pMemory)destroy_memory(vm->pMemory);
	if(vm && vm->pProgram)destroy_program(vm->pProgram);
	if(vm) free(vm);
}

void run_vm(tvm_t* vm)
{
	int* instr_idx = &vm->pMemory->registers[0x8].i32; *instr_idx = vm->pProgram->start;

	for(;vm->pProgram->instr[*instr_idx] != -0x1; ++(*instr_idx))
	{
		int *arg0 = vm->pProgram->args[*instr_idx][0], *arg1 = vm->pProgram->args[*instr_idx][1];

		switch(vm->pProgram->instr[*instr_idx])
		{
/* nop */		case 0x0:  break;
/* interrupt */		case 0x1:  break;
			case 0x2:  *arg0 = *arg1; break;
			case 0x3:  stack_push(vm->pMemory, arg0); break;
			case 0x4:  stack_pop(vm->pMemory, arg0); break;
			case 0x5:  stack_push(vm->pMemory, &vm->pMemory->FLAGS); break;
			case 0x6:  stack_pop(vm->pMemory, arg0); break;
			case 0x7:  ++(*arg0); break;
			case 0x8:  --(*arg0); break;
			case 0x9:  *arg0 += *arg1; break;
			case 0xA:  *arg0 -= *arg1; break;
			case 0xB:  *arg0 *= *arg1; break;
			case 0xC:  *arg0 /= *arg1; break;
			case 0xD:  vm->pMemory->remainder = *arg0 % *arg1; break;
			case 0xE:  *arg0 = vm->pMemory->remainder; break;
			case 0xF:  *arg0 = ~(*arg0); break;
			case 0x10:  *arg0 ^= *arg1;  break;
			case 0x11: *arg0 |= *arg1;   break;
			case 0x12: *arg0 &= *arg1;   break;
			case 0x13: *arg0 <<= *arg1;  break;
			case 0x14: *arg0 >>= *arg1;  break;
			case 0x15: vm->pMemory->FLAGS = ((*arg0 == *arg1) | (*arg0 > *arg1) << 1); break;
			case 0x17: stack_push(vm->pMemory, instr_idx);
			case 0x16: *instr_idx = *arg0 - 1; break;
			case 0x18: stack_pop(vm->pMemory, instr_idx);
			case 0x19: if(vm->pMemory->FLAGS   & 0x1)  *instr_idx = *arg0 - 1; break;
			case 0x1A: if(!(vm->pMemory->FLAGS & 0x1)) *instr_idx = *arg0 - 1; break;
			case 0x1B: if(vm->pMemory->FLAGS   & 0x2)  *instr_idx = *arg0 - 1; break;
			case 0x1C: if(vm->pMemory->FLAGS   & 0x3)  *instr_idx = *arg0 - 1; break;
			case 0x1D: if(!(vm->pMemory->FLAGS & 0x3)) *instr_idx = *arg0 - 1; break;
			case 0x1E: if(!(vm->pMemory->FLAGS & 0x2)) *instr_idx = *arg0 - 1; break;
			case 0x1F: printf("%i\n", *arg0);
		};
	}
}
