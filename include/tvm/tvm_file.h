#ifndef TVM_FILE_H_
#define TVM_FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* tvm_fopen(const char* filename, const char* extension, const char* mode);

/* Copy the contents of a file from file handle "src" to char array "dest" */
int tvm_fcopy(char* dest, size_t size, FILE* src);

/* Get the length of a file in bytes */
int tvm_flength(FILE* f);

#endif
