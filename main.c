#include <stdlib.h>
#include <stdio.h>

#include "tvm.h"

int main(int argc, char** argv)
{
	tvm_t* vm = create_vm(argv[1]);
	if(vm != NULL) run_vm(vm);

	destroy_vm(vm);

	return 0;
}
