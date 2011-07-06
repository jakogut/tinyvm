#ifndef TVM_MEMORY_H_
#define TVM_MEMORY_H_

#include <stdlib.h>

typedef struct
{
	/*
	Similar to x86 FLAGS register

	0x1	EQUAL
	0x2	GREATER

	*/

	unsigned int FLAGS;
	int remainder;

	// System memory
	int* int32;
	int max;

} memory;

memory* create_memory(size_t size);
void destroy_memory(memory* mem);

#endif
