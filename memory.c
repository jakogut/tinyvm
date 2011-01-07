#include "memory.h"

memory* create_memory(size_t size)
{
	memory* mem;

	mem = (memory*)malloc(sizeof(memory));
	mem->int32 = (int*)malloc(size / sizeof(int));

	return mem;
}

void destroy_memory(memory* mem)
{
	free(mem->int32);
	free(mem);
}

int* arg_to_addr(memory* mem, argument* arg)
{
        if(arg->type == ARG_TYPE_ADDRESS) return &(mem->int32[arg->value]);
	else return &arg->value;
}
