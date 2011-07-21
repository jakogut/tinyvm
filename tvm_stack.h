#ifndef TVM_STACK_H_
#define TVM_STACK_H_

typedef struct tvm_stack_s
{
	int* items;
	int num_items;

	int num_slots;
} tvm_stack_t;

tvm_stack_t* create_stack();
void destroy_stack(tvm_stack_t* s);

void stack_push(tvm_stack_t* s, int* item);
void stack_pop(tvm_stack_t* s, int* dest);

#endif
