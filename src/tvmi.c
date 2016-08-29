#include <stdlib.h>
#include <stdio.h>

#include <tvm/tvm.h>

int main(int argc, char **argv)
{
	struct tvm_ctx *vm = tvm_vm_create();

	if (vm != NULL && tvm_vm_interpret(vm, argv[1]) == 0)
		tvm_vm_run(vm);

	tvm_vm_destroy(vm);

	return 0;
}
