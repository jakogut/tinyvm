#include <tvm/tvm.h>

#include <tvm/tvm_lexer.h>
#include <tvm/tvm_parser.h>

struct tvm_ctx *tvm_vm_create()
{
	struct tvm_ctx *vm =
		(struct tvm_ctx *)calloc(1, sizeof(struct tvm_ctx));

	vm->mem = tvm_mem_create(MIN_MEMORY_SIZE);
	vm->prog = tvm_prog_create();

	tvm_stack_create(vm->mem, MIN_STACK_SIZE);

	if (!vm || !vm->mem || !vm->prog)
		return NULL;

	return vm;
}

int tvm_vm_interpret(struct tvm_ctx *vm, char *filename)
{
	FILE *filp = NULL;
	int source_length = 0;

	/* Attempt to open the file.
	 * If the file cannot be opened, try once more.
	 */
	if (filename) {
		for (int i = 0; i < 2; i++) {
			if (!filp)
				filp = tvm_fopen(filename, ".vm", "r");
		}
	}

	if (!filp) {
		printf("File was not found, or does not exist. Unable to interpret.\n");
		return 1;
	}

	source_length = tvm_flength(filp);
	char *source = calloc(source_length, sizeof(char));

	tvm_fcopy(source, source_length, filp);
	fclose(filp);

	int err = tvm_preprocess(&source, &source_length, vm->prog->defines);

	/* The preprocessor encountered a problem. */
	if (err < 0)
		return 1;

	struct tvm_lexer_ctx *lexer_ctx = lexer_create();

	tvm_lex(lexer_ctx, source, vm->prog->defines);
	free(source);

	if (tvm_parse_labels(vm, (const char ***)lexer_ctx->tokens) != 0)
		return 1;
	if (tvm_parse_program(vm, (const char ***)lexer_ctx->tokens) != 0)
		return 1;

	tvm_lexer_destroy(lexer_ctx);
	return 0;
}

void tvm_vm_run(struct tvm_ctx *vm)
{
	int *instr_idx = &vm->mem->registers[0x8].i32;
	*instr_idx = vm->prog->start;

	for (; vm->prog->instr[*instr_idx] != -0x1; ++(*instr_idx))
		tvm_step(vm, instr_idx);
}


void tvm_vm_destroy(struct tvm_ctx *vm)
{
	if (vm && vm->mem)
		tvm_mem_destroy(vm->mem);
	if (vm && vm->prog)
		tvm_prog_destroy(vm->prog);
	if (vm)
		free(vm);
}
