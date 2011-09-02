#include <stdio.h>

#include "tdb.h"

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
