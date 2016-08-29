#ifndef TVM_LEXER_H_
#define TVM_LEXER_H_

#define MAX_ARGS 2
#define MAX_TOKENS 4

#include "tvm_htab.h"

struct tvm_lexer_ctx {
	char **source_lines;
	char ***tokens;
};

struct tvm_lexer_ctx *lexer_create();
void tvm_lexer_destroy(struct tvm_lexer_ctx *l);

/* Tokenize the character array "source" into lines and tokens */
void tvm_lex(struct tvm_lexer_ctx *lexer,
	char *source, struct tvm_htab_ctx *defines);

#endif
