#include <tvm/tvm_file.h>
#include <tvm/tvm_program.h>
#include <tvm/tvm_preprocessor.h>
#include <tvm/tvm_lexer.h>
#include <tvm/tvm_parser.h>
#include <tvm/tvm.h>

struct tvm_prog *tvm_prog_create()
{
	struct tvm_prog *p = calloc(1, sizeof(struct tvm_prog));

	p->label_htab = tvm_htab_create();
	p->defines = tvm_htab_create();

	return p;
}

void tvm_prog_destroy(struct tvm_prog *p)
{
	tvm_htab_destroy(p->label_htab);

	if (p->values) {
		for (int i = 0; i < p->num_values; i++)
			free(p->values[i]);
		free(p->values);
	}

	if (p->args) {
		for (int i = 0; p->args[i]; i++)
			free(p->args[i]);
		free(p->args);
	}

	if (p->instr)
		free(p->instr);
	free(p);
}
