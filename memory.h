#ifndef VIRTUALMEMORY_H_
#define VIRTUALMEMORY_H_

#include <stdlib.h>

typedef struct
{
	/*
	Similar to x86 FLAGS register
	This stores certain flags used by operations in big endian format. (Bit 0x5 would be (1 << 27))

	0x1	EQUAL
	0x2	GREATER

	*/

	unsigned int FLAGS;
	int remainder;

	// System memory
	int* int32;

} memory;

memory* create_memory(size_t size);
void destroy_memory(memory* mem);

#endif
