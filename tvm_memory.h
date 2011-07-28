#ifndef TVM_MEMORY_H_
#define TVM_MEMORY_H_

#include <stdint.h>
#include <stddef.h>

#define MIN_MEMORY_SIZE 16384

#define BYTE int8_t
#define WORD int32_t
#define DBYTE int16_t
#define DWORD int64_t

#define WORD_PTR int32_t*

typedef union
{
	WORD i32;
	WORD_PTR i32_ptr;

	union
	{
		DBYTE h;
		DBYTE l;
	} i16;

} tvm_register_t;

typedef struct
{

	/*
	Similar to x86 FLAGS register

	0x1	EQUAL
	0x2	GREATER

	*/

	unsigned int FLAGS;
	int remainder;

	void* mem_space;
	int mem_space_size;

	tvm_register_t* registers;

} tvm_memory_t;

tvm_memory_t* create_memory(size_t size);
void destroy_memory(tvm_memory_t* mem);

#endif
