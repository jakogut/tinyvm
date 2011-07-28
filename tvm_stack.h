#ifndef TVM_STACK_H_
#define TVM_STACK_H_

#define MIN_STACK_SIZE 1024

#include "tvm_memory.h"

void create_stack(tvm_memory_t* mem, size_t size);

void stack_push(tvm_memory_t* mem, int* item);
void stack_pop(tvm_memory_t* mem, int* dest);

#endif
