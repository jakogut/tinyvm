#ifndef TVM_STACK_H_
#define TVM_STACK_H_

#define MIN_STACK_SIZE (2 * 1024 * 1024) /* 2 MB */

#include "tvm_memory.h"

/* Initialize our stack by setting the base pointer and stack pointer */

static inline void tvm_stack_create(struct tvm_mem *mem, size_t size)
{
	mem->registers[0x7].i32_ptr =
		((int32_t *)mem->mem_space) + (size / sizeof(int32_t));
	mem->registers[0x6].i32_ptr = mem->registers[0x7].i32_ptr;
}

static inline void tvm_stack_push(struct tvm_mem *mem, int *item)
{
	mem->registers[0x6].i32_ptr -= 1;
	*mem->registers[0x6].i32_ptr = *item;
}

static inline void tvm_stack_pop(struct tvm_mem *mem, int *dest)
{
	*dest = *mem->registers[0x6].i32_ptr;
	mem->registers[0x6].i32_ptr += 1;
}

#endif
