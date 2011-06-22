#include <stdlib.h>
#include <stdio.h>

#include "tvm.h"

int main(int argc, char** argv)
{
	char* filename = "";

	if(argc == 2)
		filename = argv[1];

	virtual_machine* vm = create_vm(filename);
	if(vm != NULL) run_vm(vm);

	// Print the first memory address
	if(vm) printf("%i\n", vm->pMemory->int32[0]);

	destroy_vm(vm);

	return 0;
}
