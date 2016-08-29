#ifndef TVM_PARSER_H_
#define TVM_PARSER_H_

#include "tvm.h"
#include "tvm_program.h"

int tvm_parse_labels(struct tvm_ctx *vm, const char ***tokens);
int tvm_parse_program(struct tvm_ctx *vm, const char ***tokens);

int *tvm_add_value(struct tvm_ctx *vm, const int val);
int tvm_parse_value(const char *str);

#endif
