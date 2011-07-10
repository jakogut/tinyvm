#include "tvm_memory.h"

#include <string.h>

#define NUM_REGISTERS 16

memory* create_memory(size_t size)
{
	memory* m =  (memory*)malloc(sizeof(memory));

	int num_words = size / sizeof(int);

	m->mem_space_size = size;
	m->mem_space = (int*)malloc(num_words * sizeof(int));

	m->registers = malloc(sizeof(tvm_register) * NUM_REGISTERS);

	m->FLAGS = 0;
	m->remainder = 0;

	return m;
}

void destroy_memory(memory* m)
{
	free(m->mem_space);
	free(m->registers);
	free(m);
}
