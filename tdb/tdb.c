#include <stdio.h>

#include <tvm/tvm.h>

const char* commands[] = {"q", "run", "break", "step", 0};

int cmd_to_idx(char* cmd)
{
	int i;
	for(i = 0; commands[i]; i++)
		if(strcmp(cmd, commands[i]) == 0) return i;

	return 0;
}

void tdb_shell(tvm_t* vm)
{
	int run = 1;

	while(run)
	{
		printf(": ");

		char str[32];
		scanf("%s", str);

		switch(cmd_to_idx(str))
		{
			case 0x0: run = 0; 	break;
			case 0x1: tvm_run(vm); 	break;
			default: 		break;
		};

		printf("\n");
	}
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
