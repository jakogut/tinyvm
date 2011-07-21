#include <stdlib.h>

#include "tvm_stack.h"

#define STACK_CACHE 64

void stack_push(tvm_stack_t* s, int* item)
{
	s->num_items++;

	if(s->num_slots <= s->num_items)
	{
		s->num_slots = s->num_items + STACK_CACHE;
		s->items = (int*)realloc(s->items, s->num_slots * sizeof(int));
	}

	s->items[s->num_items - 1] = *item;
}

void stack_pop(tvm_stack_t* s, int* dest)
{
	if(s->num_items > 0)
	{
		*dest = s->items[s->num_items - 1];

		if(s->num_slots >= s->num_items + STACK_CACHE)
		{
			s->items = (int*)realloc(s->items, s->num_items * sizeof(int));
			s->num_slots = --s->num_items;
		}
	}
}

tvm_stack_t* create_stack()
{
	tvm_stack_t* s = (tvm_stack_t*)calloc(1, sizeof(tvm_stack_t));

	return s;
}

void destroy_stack(tvm_stack_t* s)
{
	if(s->items) free(s->items);
	free(s);
}
