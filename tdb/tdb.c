#include <stdio.h>

#include <tvm/tvm.h>

#define TOKEN_LENGTH 32
#define MAX_TOKENS 3

const char* commands[] = {"q", "run", "break", "step", 0};

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

void tdb_shell(tvm_t* vm)
{
	int run = 1, i;

	char* tokens[3];
	char str[256];

	for(i = 0; i < MAX_TOKENS; i++) tokens[i] = malloc(TOKEN_LENGTH);

	while(run)
	{
		printf(": ");

		for(i = 0; i < MAX_TOKENS; i++) memset(tokens[i], 0, TOKEN_LENGTH);

		fgets(str, 256, stdin);
		tokenize(str, (char**)tokens);

		switch(cmd_to_idx(tokens[0]))
		{
			case -0x1: printf("\nWARNING: \"%s\" is not a valid command.\n", tokens[0]); break;
			case  0x0: run = 0; break;
			case  0x1: tvm_run(vm); break;
			case  0x2: break;
			case  0x3: break;
		};

		printf("\n");
	}

	for(i = 0; i < MAX_TOKENS; i++) free(tokens[i]);
}

int main(int argc, char** argv)
{
	tvm_t* vm = tvm_create(argv[1]);
	if(vm)
	{
		tdb_shell(vm);
		tvm_destroy(vm);
	}

	return 0;
}
