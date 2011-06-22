#ifndef TVM_FILE_H_
#define TVM_FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* tvm_openfile(const char* filename, const char* extension, const char* mode);

#endif
