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
			case  0x3: tvm_step(vm, &vm->pMemory->registers[0x8].i32);
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

		tvm_step(vm, instr_idx);

	}

	return 0;
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
