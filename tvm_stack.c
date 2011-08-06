#include <stdlib.h>

#include "tvm_stack.h"

void create_stack(tvm_memory_t* mem, size_t size)
{
	mem->registers[0x7].i32_ptr = ((int32_t*)mem->mem_space) + (size / sizeof(int32_t));
	mem->registers[0x6].i32_ptr = mem->registers[0x7].i32_ptr;
}

void stack_push(tvm_memory_t* mem, int* item)
{
	mem->registers[0x6].i32_ptr -= 1;
	*mem->registers[0x6].i32_ptr = *item;
}

void stack_pop(tvm_memory_t* mem, int* dest)
{
	*dest = *mem->registers[0x6].i32_ptr;
	mem->registers[0x6].i32_ptr += 1;
}
