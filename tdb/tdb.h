#ifndef TDB_H_
#define TDB_H_

#include <tvm/tvm.h>
#include "tdb_breakpoint.h"

void tdb_shell(tvm_t *vm);

int tdb_run(tvm_t *vm, tdb_breakpoint_t *breakpoints, int num_breakpoints);
void tdb_step(tvm_t *vm);

#endif
