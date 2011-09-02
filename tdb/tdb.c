#include "tdb.h"
#include <tvm/tvm.h>

#include <stdio.h>

#define TOKEN_LENGTH 32
#define MAX_TOKENS 3
#define MAX_INPUT_LENGTH 256

const char* commands[] = {"q", "run", "break", "step", 0};

static void tokenize(char* str, char** tokens);
static int cmd_to_idx(char* cmd);

void tdb_shell(tvm_t* vm)
{
	int run = 1, i;

	char* tokens[MAX_TOKENS];
	char str[MAX_INPUT_LENGTH];

	int num_breakpoints = 0;
	tdb_breakpoint_t* breakpoints = NULL;

	for(i = 0; i < MAX_TOKENS; i++) tokens[i] = malloc(TOKEN_LENGTH);

        vm->pMemory->registers[0x8].i32 = vm->pProgram->start;

	while(run)
	{
		int retcode = 0;

		printf(": ");

		for(i = 0; i < MAX_TOKENS; i++) memset(tokens[i], 0, TOKEN_LENGTH);

		fgets(str, MAX_INPUT_LENGTH, stdin);
		tokenize(str, (char**)tokens);

		switch(cmd_to_idx(tokens[0]))
		{
			case -0x1: printf("\nWARNING: \"%s\" is not a valid command.\n", tokens[0]); break;
			case  0x0: run = 0; break;
			case  0x1: retcode = tdb_run(vm, breakpoints, num_breakpoints); break;
			case  0x2: breakpoints = realloc(breakpoints, sizeof(tdb_breakpoint_t) * ++num_breakpoints);
				   breakpoints[num_breakpoints - 1].address = tvm_parse_value(tokens[1]); break;
			case  0x3: tdb_step(vm);
				   vm->pMemory->registers[0x8].i32++;
				   printf("Advancing instruction pointer to %i\n", vm->pMemory->registers[0x8].i32); break;
		};

		if(retcode == 1) printf("Breakpoint hit at address: %i\n", vm->pMemory->registers[0x8].i32);

		printf("\n");
	}

	free(breakpoints);

	for(i = 0; i < MAX_TOKENS; i++) free(tokens[i]);
}

int tdb_run(tvm_t* vm, tdb_breakpoint_t* breakpoints, int num_breakpoints)
{
	int* instr_idx = &vm->pMemory->registers[0x8].i32;

	for(;vm->pProgram->instr[*instr_idx] != -0x1; ++(*instr_idx))
	{
		int i;
		for(i = 0; i < num_breakpoints; i++)
			if(breakpoints[i].address == *instr_idx) return 1; /* Breakpoint hit */

		tdb_step(vm);

	}

	return 0;
}

void tdb_step(tvm_t* vm)
{
	int* instr_idx = &vm->pMemory->registers[0x8].i32;
	int *arg0 = vm->pProgram->args[*instr_idx][0], *arg1 = vm->pProgram->args[*instr_idx][1];

	switch(vm->pProgram->instr[*instr_idx])
	{
/* nop   */	case 0x0:  break;
/* int   */	case 0x1:  break;
/* mov   */	case 0x2:  *arg0 = *arg1; break;
/* push  */	case 0x3:  stack_push(vm->pMemory, arg0); break;
/* pop   */	case 0x4:  stack_pop(vm->pMemory, arg0); break;
/* pushf */	case 0x5:  stack_push(vm->pMemory, &vm->pMemory->FLAGS); break;
/* popf  */	case 0x6:  stack_pop(vm->pMemory, arg0); break;
/* inc   */	case 0x7:  ++(*arg0); break;
/* dec   */	case 0x8:  --(*arg0); break;
/* add   */	case 0x9:  *arg0 += *arg1; break;
/* sub   */	case 0xA:  *arg0 -= *arg1; break;
/* mul   */	case 0xB:  *arg0 *= *arg1; break;
/* div   */	case 0xC:  *arg0 /= *arg1; break;
/* mod   */	case 0xD:  vm->pMemory->remainder = *arg0 % *arg1; break;
/* rem   */	case 0xE:  *arg0 = vm->pMemory->remainder; break;
/* not   */	case 0xF:  *arg0 = ~(*arg0); break;
/* xor   */	case 0x10:  *arg0 ^= *arg1;  break;
/* or    */	case 0x11: *arg0 |= *arg1;   break;
/* and   */	case 0x12: *arg0 &= *arg1;   break;
/* shl   */	case 0x13: *arg0 <<= *arg1;  break;
/* shr   */	case 0x14: *arg0 >>= *arg1;  break;
/* cmp   */	case 0x15: vm->pMemory->FLAGS = ((*arg0 == *arg1) | (*arg0 > *arg1) << 1); break;
/* jmp   */	case 0x17: stack_push(vm->pMemory, instr_idx);
/* call  */	case 0x16: *instr_idx = *arg0 - 1; break;
/* ret   */	case 0x18: stack_pop(vm->pMemory, instr_idx);
/* je    */	case 0x19: if(vm->pMemory->FLAGS   & 0x1)  *instr_idx = *arg0 - 1; break;
/* jne   */	case 0x1A: if(!(vm->pMemory->FLAGS & 0x1)) *instr_idx = *arg0 - 1; break;
/* jg    */	case 0x1B: if(vm->pMemory->FLAGS   & 0x2)  *instr_idx = *arg0 - 1; break;
/* jge   */	case 0x1C: if(vm->pMemory->FLAGS   & 0x3)  *instr_idx = *arg0 - 1; break;
/* jl    */	case 0x1D: if(!(vm->pMemory->FLAGS & 0x3)) *instr_idx = *arg0 - 1; break;
/* jle   */	case 0x1E: if(!(vm->pMemory->FLAGS & 0x2)) *instr_idx = *arg0 - 1; break;
/* prn   */	case 0x1F: printf("%i\n", *arg0);
	};
}

void tokenize(char* str, char** tokens)
{
	int i = 0;
	char* token = strtok(str, " 	\n");

	for(i = 0; token && (i < MAX_TOKENS); i++)
	{
		strcpy(tokens[i], token);
		token = strtok(NULL, " 	\n");
	}
}

int cmd_to_idx(char* cmd)
{
	int i;
	for(i = 0; commands[i]; i++)
		if(strcmp(cmd, commands[i]) == 0) return i;

	return -1;
}
