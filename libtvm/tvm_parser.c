#include <tvm/tvm_parser.h>
#include <tvm/tvm_file.h>
#include <tvm/tvm_lexer.h>
#include <tvm/tvm_tokens.h>

const char *tvm_opcode_map[] = {
	"nop", "int", "mov",
	"push", "pop", "pushf", "popf",
	"inc", "dec", "add", "sub", "mul", "div", "mod", "rem",
	"not", "xor", "or", "and", "shl", "shr",
	"cmp", "jmp", "call", "ret",
	"je", "jne", "jg", "jge", "jl", "jle",
	"prn", 0
};

const char *tvm_register_map[] = {
	"eax", "ebx", "ecx", "edx",
	"esi", "edi", "esp", "ebp",
	"eip",
	"r08", "r09", "r10", "r11",
	"r12", "r13", "r14", "r15", 0};


static int *token_to_register(const char *token, struct tvm_mem *mem);
static int instr_to_opcode(const char *instr);

int tvm_parse_labels(struct tvm_ctx *vm, const char ***tokens)
{
	int num_instr = 0;
	struct tvm_prog *p = vm->prog;

	for (int i = 0; tokens[i]; i++) {
		int valid_instruction = 0;

		for (int token_idx = 0; token_idx < MAX_TOKENS; token_idx++) {
			/* If the token is empty, or non-existent, skip it */
			if (!tokens[i][token_idx])
				continue;

			/* Check the source line for a valid instruction */
			if (instr_to_opcode(tokens[i][token_idx]) != -1)
				valid_instruction = 1;

			/* Check for a label delimiter*/
			char *label_delimiter = strchr(
				tokens[i][token_idx], ':');

			if (label_delimiter == NULL)
				continue;

			*label_delimiter = 0;

			/* If the label is "start," make it the entry point */
			if (strcmp(tokens[i][token_idx], "start") == 0)
				p->start = num_instr;

			/* Check if the label already exists */
			int label_addr = tvm_htab_find(
				p->label_htab, tokens[i][token_idx]);

			if (label_addr != -1) {
				printf("Label '%s' defined twice\n", tokens[i][token_idx]);
				return 1;
			}

			tvm_htab_add(
				p->label_htab,
				tokens[i][token_idx],
				num_instr
			);
		}

		if (valid_instruction)
			num_instr++;
	}

	return 0;
}

/* This function takes the instruction tokens, and location of the
 * instruction inside the line, parses the arguments, and returns a pointer
 * to the heap where they're stored.
 */
static int **tvm_parse_args(
	struct tvm_ctx *vm, const char **instr_tokens, int *instr_place)
{
	int **args = calloc(sizeof(int *), MAX_ARGS);

	for (int i = 0; i < MAX_ARGS; ++i) {
		if (!instr_tokens[*instr_place+1 + i]
			|| !strlen(instr_tokens[*instr_place+1 + i]))
			continue;

		char *newline = strchr(instr_tokens[*instr_place+1 + i], '\n');

		if (newline)
			*newline = 0;

		/* Check to see if the token specifies a register */
		int *regp = token_to_register(
			instr_tokens[*instr_place+1 + i], vm->mem);

		if (regp) {
			args[i] = regp;
			continue;
		}

		/* Check to see whether the token specifies an address */
		if (instr_tokens[*instr_place+1 + i][0] == '[') {
			char *end_symbol = strchr(
				instr_tokens[*instr_place+1 + i], ']');

			if (end_symbol) {
				*end_symbol = 0;
				args[i] = &((int *)vm->mem->mem_space)[
					tvm_parse_value(instr_tokens[
						*instr_place+1 + i] + 1)];

				continue;
			}
		}

		/* Check if the argument is a label */
		int addr = tvm_htab_find(
			vm->prog->label_htab, instr_tokens[*instr_place+1 + i]);

		if (addr != -1) {
			args[i] = tvm_add_value(vm, addr);
			continue;
		}

		/* Fuck it, parse it as a value */
		args[i] = tvm_add_value(
			vm, tvm_parse_value(instr_tokens[*instr_place+1 + i]));
	}

	return args;
}

/* This function frees the memory allocated by tvm_parse_args().
 */
static void tvm_free_args(int **args) {
	if(args) {
                for (int i = 0; args[i]; i++) {
			free(args[i]);
                }

	}
	free(args);
}




/* This is a helper function that converts one instruction,
 * from one line of code, to tvm bytecode
 */
static int tvm_parse_instr(
	struct tvm_ctx *vm, const char **instr_tokens, int *instr_place)
{
	for (int token_idx = 0; token_idx < MAX_TOKENS; token_idx++) {
		/* Skip empty tokens */
		if (!instr_tokens[token_idx])
			continue;

		int opcode = instr_to_opcode(instr_tokens[token_idx]);

		if (opcode == -1)
			continue;

		if (instr_place)
			*instr_place = token_idx;

		vm->prog->num_instr++;
		return opcode;
	}

	return -1;
}

int tvm_parse_program(
	struct tvm_ctx *vm, const char ***tokens)
{
	int line_idx;

	for (line_idx = 0; tokens[line_idx]; line_idx++) {
		int instr_place = 0;

		int opcode = tvm_parse_instr(
			vm, tokens[line_idx], &instr_place);

		if (opcode == -1)
			continue;

		int **args = tvm_parse_args(
			vm, tokens[line_idx], &instr_place);

		if (!args)
			continue;

		void *newptr;

		newptr = realloc(
			vm->prog->instr, sizeof(int) * (vm->prog->num_instr+1));

		if (newptr != NULL) {
			vm->prog->instr = newptr;
			vm->prog->instr[vm->prog->num_instr - 1] = opcode;
		} else {
			tvm_free_args(args);
			return -1;
                }

		newptr = realloc(
			vm->prog->args,
			sizeof(int **) * (vm->prog->num_instr+1));

		if (newptr != NULL)
			vm->prog->args = (int ***)newptr;
		else {
			tvm_free_args(args);
			return -1;
		}

		vm->prog->args[vm->prog->num_instr - 1] = args;
	}

	vm->prog->args[vm->prog->num_instr] = NULL;
	vm->prog->instr[vm->prog->num_instr] = -0x1;

	return 0;
}

int *token_to_register(const char *token, struct tvm_mem *mem)
{
	for (int i = 0; tvm_register_map[i]; i++) {
		if (strcmp(token, tvm_register_map[i]) == 0)
			return &mem->registers[i].i32;
	}

	return NULL;
}

int instr_to_opcode(const char *instr)
{
	for (int i = 0; tvm_opcode_map[i]; i++)
		if (strcmp(instr, tvm_opcode_map[i]) == 0)
			return i;

	return -1;
}

int *tvm_add_value(struct tvm_ctx *vm, const int val)
{
	struct tvm_prog *p = vm->prog;

	p->values = realloc(p->values, sizeof(int *) * (p->num_values + 1));
	p->values[p->num_values] = (int *)calloc(1, sizeof(int));

	*p->values[p->num_values] = val;

	return p->values[p->num_values++];
}

int tvm_parse_value(const char *str)
{
	char *delimiter = strchr(str, '|'), base = 0;

	if (delimiter) {
		char *identifier = delimiter + 1;

		switch (*identifier) {
		case 'h':
			base = 16;
			break;
		case 'b':
			base = 2;
			break;
		default:
			base = 0;
			break;
		}
	}

	return strtoul(str, NULL, base);
}
