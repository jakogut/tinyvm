#include <tvm/tvm.h>

tvm_t* tvm_create(char* filename)
{
	tvm_t* vm = (tvm_t*)malloc(sizeof(tvm_t));

	vm->pMemory = create_memory(MIN_MEMORY_SIZE);
	vm->pProgram = create_program();

	create_stack(vm->pMemory, MIN_STACK_SIZE);

	if(!vm || !vm->pMemory || !vm->pProgram) return NULL;

	return vm;
}

int tvm_interpret(tvm_t* vm, char* filename)
{
	if(interpret_program(vm->pProgram, filename, vm->pMemory) != 0) return 1;
	return 0;
}

void tvm_destroy(tvm_t* vm)
{
	if(vm && vm->pMemory)destroy_memory(vm->pMemory);
	if(vm && vm->pProgram)destroy_program(vm->pProgram);
	if(vm) free(vm);
}

void tvm_run(tvm_t* vm)
{
	int* instr_idx = &vm->pMemory->registers[0x8].i32; *instr_idx = vm->pProgram->start;

	for(;vm->pProgram->instr[*instr_idx] != -0x1; ++(*instr_idx)) tvm_step(vm, instr_idx);
}

void tvm_step(tvm_t* vm, int* instr_idx)
{
	int *arg0 = vm->pProgram->args[*instr_idx][0], *arg1 = vm->pProgram->args[*instr_idx][1];

	switch(vm->pProgram->instr[*instr_idx])
	{
/* nop   */	case 0x0:  break;
/* int   */	case 0x1:  /* unimplemented */ break;
/* mov   */	case 0x2:  *arg0 = *arg1; break;
/* push  */	case 0x3:  stack_push(vm->pMemory, arg0); break;
/* pop   */	case 0x4:  stack_pop(vm->pMemory, arg0); break;
/* pushf */	case 0x5:  stack_push(vm->pMemory, &vm->pMemory->FLAGS); break;
/* popf  */	case 0x6:  stack_pop(vm->pMemory, arg0); break;
/* inc   */	case 0x7:  ++(*arg0); break;
/* dec   */	case 0x8:  --(*arg0); break;
/* add   */	case 0x9:  *arg0 += *arg1; break;
/* sub   */	case 0xA:  *arg0 -= *arg1; break;
/* mul   */	case 0xB:  *arg0 *= *arg1; break;
/* div   */	case 0xC:  *arg0 /= *arg1; break;
/* mod   */	case 0xD:  vm->pMemory->remainder = *arg0 % *arg1; break;
/* rem   */	case 0xE:  *arg0 = vm->pMemory->remainder; break;
/* not   */	case 0xF:  *arg0 = ~(*arg0); break;
/* xor   */	case 0x10:  *arg0 ^= *arg1;  break;
/* or    */	case 0x11: *arg0 |= *arg1;   break;
/* and   */	case 0x12: *arg0 &= *arg1;   break;
/* shl   */	case 0x13: *arg0 <<= *arg1;  break;
/* shr   */	case 0x14: *arg0 >>= *arg1;  break;
/* cmp   */	case 0x15: vm->pMemory->FLAGS = ((*arg0 == *arg1) | (*arg0 > *arg1) << 1); break;
/* jmp   */	case 0x17: stack_push(vm->pMemory, instr_idx);
/* call  */	case 0x16: *instr_idx = *arg0 - 1; break;
/* ret   */	case 0x18: stack_pop(vm->pMemory, instr_idx); break;
/* je    */	case 0x19: if(vm->pMemory->FLAGS   & 0x1)  *instr_idx = *arg0 - 1; break;
/* jne   */	case 0x1A: if(!(vm->pMemory->FLAGS & 0x1)) *instr_idx = *arg0 - 1; break;
/* jg    */	case 0x1B: if(vm->pMemory->FLAGS   & 0x2)  *instr_idx = *arg0 - 1; break;
/* jge   */	case 0x1C: if(vm->pMemory->FLAGS   & 0x3)  *instr_idx = *arg0 - 1; break;
/* jl    */	case 0x1D: if(!(vm->pMemory->FLAGS & 0x3)) *instr_idx = *arg0 - 1; break;
/* jle   */	case 0x1E: if(!(vm->pMemory->FLAGS & 0x2)) *instr_idx = *arg0 - 1; break;
/* prn   */	case 0x1F: printf("%i\n", *arg0);
	};
}
