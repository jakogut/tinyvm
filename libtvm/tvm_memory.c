#include <tvm/tvm_memory.h>

#include <stdlib.h>
#include <string.h>

#define NUM_REGISTERS 17

struct tvm_mem *tvm_mem_create(size_t size)
{
	struct tvm_mem *m =
		(struct tvm_mem *)calloc(1, sizeof(struct tvm_mem));

	m->registers = calloc(NUM_REGISTERS, sizeof(union tvm_reg_u));

	m->mem_space_size = size;
	m->mem_space = (int *)calloc(size, 1);

	return m;
}

void tvm_mem_destroy(struct tvm_mem *m)
{
	free(m->mem_space);
	free(m->registers);
	free(m);
}
