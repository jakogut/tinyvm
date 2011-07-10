#include <stdlib.h>

#include "tvm_stack.h"

#define STACK_CACHE 64

void stack_push(stack* s, int* item)
{
	s->num_items++;

	if(s->num_slots <= s->num_items)
	{
		s->num_slots = s->num_items + STACK_CACHE;
		s->items = (int*)realloc(s->items, s->num_slots * sizeof(int));
	}

	s->items[s->num_items - 1] = *item;
}

void stack_pop(stack* s, int* dest)
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

stack* create_stack()
{
	stack* s;

	s = (stack*)malloc(sizeof(stack));

	s->items = NULL;
	s->num_items = 0;
    s->num_slots = 0;

	return s;
}

void destroy_stack(stack* s)
{
	if(s->items) free(s->items);
	free(s);
}
