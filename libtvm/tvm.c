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

