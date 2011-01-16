#include <stdlib.h>

#include "stack.h"

void stack_push(stack* s, int* item)
{
	s->num_items++;
	s->items = (int*)realloc(s->items, s->num_items * sizeof(int));

	s->items[s->num_items - 1] = *item;
}

void stack_pop(stack* s, int* dest)
{
	*dest = s->items[s->num_items - 1];

	s->num_items--;
	s->items = (int*)realloc(s->items, s->num_items);
}

stack* create_stack()
{
	stack* s;

	s = (stack*)malloc(sizeof(stack));

	s->items = NULL;
	s->num_items = 0;

	return s;
}

void destroy_stack(stack* s)
{
	if(s->items) free(s->items);
	free(s);
}
