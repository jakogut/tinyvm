#ifndef TVM_PREPROCESSOR_H_
#define TVM_PREPROCESSOR_H_

#include "tvm_htab.h"

int tvm_preprocess(char *src, int *src_len, tvm_htab_t *defines);

#endif
