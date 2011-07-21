#include <stdlib.h>
#include <stdio.h>

#include "tvm.h"

int main(int argc, char** argv)
{
	char* filename = NULL;
	tvm_t* vm;

	if(argc == 2)
		filename = argv[1];

	vm = create_vm(filename);
	if(vm != NULL) run_vm(vm);

	/* Print the first memory address (later this will be replaced by an interrupt)*/
	if(vm) printf("%i\n", ((int*)vm->pMemory->mem_space)[0]);

	destroy_vm(vm);

	return 0;
}
