#ifndef _TVM_TOKENS_H_
#define _TVM_TOKENS_H_

#define TOK_INCLUDE "%include"
#define TOK_DEFINE "%define"

static const char *tvm_opcode_map[] = {
	"nop", "int", "mov",
	"push", "pop", "pushf", "popf",
	"inc", "dec", "add", "sub", "mul", "div", "mod", "rem",
	"not", "xor", "or", "and", "shl", "shr",
	"cmp", "jmp", "call", "ret",
	"je", "jne", "jg", "jge", "jl", "jle",
	"prn", 0
};

static const char *tvm_register_map[] = {
	"eax", "ebx", "ecx", "edx",
	"esi", "edi", "esp", "ebp",
	"eip",
	"r08", "r09", "r10", "r11",
	"r12", "r13", "r14", "r15", 0};

#endif
