#ifndef TVM_STACK_H_
#define TVM_STACK_H_

typedef struct
{
	int* items;
	int num_items;
} stack;

stack* create_stack();
void destroy_stack(stack* s);

inline void stack_push(stack* s, int* item);
inline void stack_pop(stack* s, int* dest);

#endif
