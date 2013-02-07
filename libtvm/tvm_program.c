#include <tvm/tvm_file.h>
#include <tvm/tvm_program.h>
#include <tvm/tvm_lexer.h>

const char *tvm_opcode_map[] = {"nop", "int", "mov", "push", "pop", "pushf", "popf", "inc", "dec", "add", "sub", "mul", "div", "mod", "rem",
				"not", "xor", "or", "and", "shl", "shr", "cmp", "jmp", "call", "ret", "je", "jne", "jg", "jge", "jl", "jle", "prn", 0};

const char *tvm_register_map[] = {"eax", "ebx", "ecx", "edx", "esi", "edi", "esp", "ebp", "eip", "r08", "r09", "r10", "r11", "r12", "r13", "r14", "r15", 0};

static int parse_labels(tvm_program_t *p, const char ***tokens);
static int parse_instructions(tvm_program_t *p, const char ***tokens, tvm_memory_t *pMemory);

static int *token_to_register(const char *token, tvm_memory_t *pMemory);
static int instr_to_opcode(const char *instr);

tvm_program_t *program_create()
{
	tvm_program_t *p = (tvm_program_t *)calloc(1, sizeof(tvm_program_t));
	p->label_htab = htab_create();

	return p;
}

void program_destroy(tvm_program_t *p)
{
	int i = 0;
	htab_destroy(p->label_htab);

	if(p->values)
	{
		for(i = 0; i < p->num_values; i++) free(p->values[i]);
		free(p->values);
	}

	if(p->args)
	{
		for(i = 0; p->args[i]; i++) free(p->args[i]);
		free(p->args);
	}

	if(p->instr) free(p->instr);
	free(p);
}

int program_interpret(tvm_program_t *p, char *filename, tvm_memory_t *pMemory)
{
	int i;
	FILE *pFile = NULL;

	int source_length = 0;
	char *source = NULL;

	tvm_lexer_t *lexer = NULL;

	/* Attempt to open the file. If the file cannot be opened, try once more. */
	if(filename)
		for(i = 0; i < 2; i++)
			if(!pFile) pFile = tvm_fopen(filename, ".vm", "r");

	if(!pFile)
	{
		printf("File was not found, or does not exist. Unable to interpret.\n");
		return 1;
	}

	source_length = tvm_flength(pFile);
	source = malloc(source_length);

	tvm_fcopy(source, source_length, pFile);

	lexer = lexer_create();
	lex(lexer, source);

	free(source);
	fclose(pFile);

	if(parse_labels(p, (const char ***)lexer->tokens) != 0)
		return 1;

	if(parse_instructions(p, (const char ***)lexer->tokens, pMemory) != 0)
		return 1;

	lexer_destroy(lexer);
	return 0;
}

int parse_labels(tvm_program_t *p, const char ***tokens)
{
	int num_instructions = 0;
	for(int i = 0; tokens[i]; i++)
	{
		int valid_instruction = 0;
		for(int token_idx = 0; token_idx < MAX_TOKENS; token_idx++)
		{
			/* If the token is empty, or non-existent, skip it */
			if(!tokens[i][token_idx]) continue;

			/* Figure out if the source line we're on contains a valid instruction */
			if(instr_to_opcode(tokens[i][token_idx]) != -1) valid_instruction = 1;

			/* Figure out if the token we're dealing with has a label delimiter */
			char* label_delimiter = strchr(tokens[i][token_idx], ':');
			if(label_delimiter == NULL) continue;

			*label_delimiter = 0;

			/* If the label is "start," set the program to begin there */
			if(strcmp(tokens[i][token_idx], "start") == 0) p->start = num_instructions;

			/* Check if the label already exists */
			if(htab_find(p->label_htab, tokens[i][token_idx]) != -1)
			{
				printf("Label '%s' defined twice\n", tokens[i][token_idx]);
				return 1;
			}
			else
			{
				/* Add that fucker to the hash table with the corresponding instruction index */
				htab_add(p->label_htab, tokens[i][token_idx], num_instructions);
			}
		}

		if(valid_instruction) num_instructions++;
	}

	return 0;
}

int parse_instructions(tvm_program_t *p, const char ***tokens, tvm_memory_t *pMemory)
{
	int line_idx;
	for(line_idx = 0; tokens[line_idx]; line_idx++)
	{
		p->instr = (int *)realloc(p->instr, sizeof(int) * (line_idx + 2));
		p->instr[line_idx] = 0;

		p->args = (int ***)realloc(p->args, sizeof(int **) * (line_idx + 2));
		p->args[line_idx] = (int **)calloc(MAX_ARGS, sizeof(int *));

		int token_idx;
		for(token_idx = 0; token_idx < MAX_TOKENS; token_idx++)
		{
			int opcode = 0;

			/* Skip empty tokens */
			if(!tokens[line_idx][token_idx]) continue;

			opcode = instr_to_opcode(tokens[line_idx][token_idx]);

			/* If it *isn't* an opcode, skip trying to parse arguments */
			if(opcode == -1) continue;

			int i, instr_idx = 0, num_instr = p->num_instructions;
			p->instr[p->num_instructions++] = opcode;

			for(i = ++token_idx; i < (token_idx + 2); ++i)
			{
				if(!tokens[line_idx][i] || strlen(tokens[line_idx][i]) <= 0) continue;

				char *newline = strchr(tokens[line_idx][i], '\n');
				if(newline) *newline = 0;

				/* Check to see if the token specifies a register */
				int *pRegister = token_to_register(tokens[line_idx][i], pMemory);
				if(pRegister)
				{
					p->args[num_instr][i - token_idx] = pRegister;
					continue;
				}

				/* Check to see whether the token specifies an address */
				if(tokens[line_idx][i][0] == '[')
				{
					char *end_symbol = strchr(tokens[line_idx][i], ']');
					if(end_symbol)
					{
						*end_symbol = 0;
						p->args[num_instr][i - token_idx] = &((int *)pMemory->mem_space)[tvm_parse_value(tokens[line_idx][i] + 1)];

						continue;
					}
				}

				/* Check if the argument is a label */
				instr_idx = htab_find(p->label_htab, tokens[line_idx][i]);

				if(instr_idx >= 0)
				{
					p->args[num_instr][i - token_idx] = tvm_add_value(p, instr_idx);
					continue;
				}

				/* Fuck it, parse it as a value */
				p->args[num_instr][i - token_idx] = tvm_add_value(p, tvm_parse_value(tokens[line_idx][i]));
			}
		}
	}

	p->args[line_idx] = NULL;
	p->instr[line_idx] = -0x1;

	return 0;
}

int* token_to_register(const char *token, tvm_memory_t *pMemory)
{
	for(int i = 0; tvm_register_map[i]; i++)
	{
		if(strcmp(token, tvm_register_map[i]) == 0)
			return &pMemory->registers[i].i32;
	}

	return NULL;
}

int instr_to_opcode(const char *instr)
{
	for(int i = 0; tvm_opcode_map[i]; i++)
		if(strcmp(instr, tvm_opcode_map[i]) == 0)
			return i;

	return -1;
}

int *tvm_add_value(tvm_program_t *p, const int val)
{
	p->values = realloc(p->values, sizeof(int *) * (p->num_values + 1));
	p->values[p->num_values] = (int *)calloc(1, sizeof(int));

	*p->values[p->num_values] = val;

	return p->values[p->num_values++];
}

int tvm_parse_value(const char *str)
{
	char *delimiter = strchr(str, '|'), base = 0;

	if(delimiter)
	{
		char *identifier = delimiter + 1;

		switch(*identifier)
		{
			case 'h': base = 16; break;
			case 'b': base = 2;  break;
			default:  base = 0;  break;
		}
	}

	return strtoul(str, NULL, base);
}
