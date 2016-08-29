#ifndef TDB_H_
#define TDB_H_

#include <tvm/tvm.h>
#include "tdb_breakpoint.h"

void tdb_shell(struct tvm_ctx *vm);

int tdb_run(struct tvm_ctx *vm, struct tdb_breakpoint *breakpoints, int num_breakpoints);
void tdb_step(struct tvm_ctx *vm);

#endif
