#ifndef TVM_MEMORY_H_
#define TVM_MEMORY_H_

#include <stdint.h>
#include <stddef.h>

#define MIN_MEMORY_SIZE (64 * 1024 * 1024) /* 64 MB */

typedef union
{
	int32_t i32;
	int32_t *i32_ptr;

	union
	{
		int16_t h;
		int16_t l;
	} i16;

} tvm_register_t;

typedef struct
{
	/*
	Similar to x86 FLAGS register

	0x1	EQUAL
	0x2	GREATER

	*/

	int FLAGS;
	int remainder;

	void *mem_space;
	int mem_space_size;

	tvm_register_t *registers;
} tvm_memory_t;

tvm_memory_t *memory_create(size_t size);
void memory_destroy(tvm_memory_t *mem);

#endif
