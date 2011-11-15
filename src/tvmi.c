#include <stdlib.h>
#include <stdio.h>

#include <tvm/tvm.h>

int main(int argc, char** argv)
{
	tvm_t* vm = tvm_create();
	if(vm != NULL && tvm_interpret(vm, argv[1]) == 0) tvm_run(vm);

	tvm_destroy(vm);

	return 0;
}
