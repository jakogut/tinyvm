#ifndef TVM_PARSER_H_
#define TVM_PARSER_H_

#include <tvm/tvm_program.h>

int parse_labels(tvm_program_t *p, const char ***tokens);
int parse_instructions(tvm_program_t *pProgram, const char ***tokens, tvm_memory_t *pMemory);

int *tvm_add_value(tvm_program_t *p, const int val);
int tvm_parse_value(const char *str);

#endif
