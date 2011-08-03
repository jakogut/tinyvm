#include <stdlib.h>

#include "tvm_stack.h"
#include "tvm_reg_idx.h"

void create_stack(tvm_memory_t* mem, size_t size)
{
	mem->registers[BP].i32_ptr = ((int32_t*)mem->mem_space) + (size / sizeof(int32_t));
	mem->registers[SP].i32_ptr = mem->registers[BP].i32_ptr;
}

void stack_push(tvm_memory_t* mem, int* item)
{
	mem->registers[SP].i32_ptr -= 1;
	*mem->registers[SP].i32_ptr = *item;
}

void stack_pop(tvm_memory_t* mem, int* dest)
{
	*dest = *mem->registers[SP].i32_ptr;
	mem->registers[SP].i32_ptr += 1;
}
