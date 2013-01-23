#include <tvm/tvm_memory.h>

#include <stdlib.h>
#include <string.h>

#define NUM_REGISTERS 17

tvm_memory_t *memory_create(size_t size)
{
	tvm_memory_t *m =  (tvm_memory_t *)calloc(1, sizeof(tvm_memory_t));

	m->registers = calloc(NUM_REGISTERS, sizeof(tvm_register_t));

	m->mem_space_size = size;
	m->mem_space = (int *)calloc(size, 1);

	return m;
}

void memory_destroy(tvm_memory_t *m)
{
	free(m->mem_space);
	free(m->registers);
	free(m);
}
