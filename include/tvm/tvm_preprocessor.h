#ifndef TVM_PREPROCESSOR_H_
#define TVM_PREPROCESSOR_H_

#include "tvm_htab.h"

int tvm_preprocess(char *src, int *src_len, struct tvm_htab_ctx *defines);

#endif
