#include <stdio.h>
#include "tdb.h"

int main(int argc, char **argv)
{
	tvm_t *vm = tvm_create();
	if(vm != NULL && tvm_interpret(vm, argv[1]) == 0) tdb_shell(vm);

	tvm_destroy(vm);

	return 0;
}
