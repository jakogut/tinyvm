#include "tvm_file.h"
#include "tvm_program.h"
#include "tvm_lexer.h"

const char* tvm_opcode_map[] = {"nop", "int", "mov", "push", "pop", "pushf", "popf", "inc", "dec", "add", "sub", "mul", "div", "mod", "rem",
				"not", "xor", "or", "and", "shl", "shr", "cmp", "jmp", "call", "ret", "je", "jne", "jg", "jge", "jl", "jle", "prn", 0};

const char* tvm_register_map[] = {"eax", "ebx", "ecx", "edx", "esi", "edi", "esp", "ebp", "eip", 0};

static int parse_labels(tvm_program_t* p, const char*** tokens);
static void parse_instruction(tvm_program_t* p, const char** tokens, tvm_memory_t* pMemory);

static int instr_to_opcode(const char* instr);

tvm_program_t* create_program()
{
	tvm_program_t* p = (tvm_program_t*)calloc(1, sizeof(tvm_program_t));
	p->label_htab = create_htab();

	return p;
}

int interpret_program(tvm_program_t* p, char* filename, tvm_memory_t* pMemory)
{
	int i;
	FILE* pFile = NULL;

	int source_length = 0;
	char* source = NULL;

	tvm_lexer_t* lexer = NULL;

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

	if(parse_labels(p, (const char***)lexer->tokens) != 0)
		return 1;

	for(i = 0; lexer->tokens[i]; i++)
	{
		p->instr = (int*)realloc(p->instr, sizeof(int) * (i + 2));
		p->instr[i] = 0;

		p->args = (int***)realloc(p->args, sizeof(int**) * (i + 2));
		p->args[i] = (int**)calloc(MAX_ARGS, sizeof(int*));

		parse_instruction(p, (const char**)lexer->tokens[i], pMemory);
	}

	lexer_destroy(lexer);

	p->args[i] = NULL;
	p->instr[i] = -0x1;

	return 0;
}

void destroy_program(tvm_program_t* p)
{
	int i = 0;
	destroy_htab(p->label_htab);

	for(i = 0; i < p->num_values; i++) free(p->values[i]);
	free(p->values);

	for(i = 0; p->args[i]; i++) free(p->args[i]);
	free(p->args);

	free(p->instr);
	free(p);
}

int parse_labels(tvm_program_t* p, const char*** tokens)
{
	int i, num_instructions = 0;
	for(i = 0; tokens[i]; i++)
	{
		int token_idx, valid_instruction = 0;
		for(token_idx = 0; token_idx < MAX_TOKENS; token_idx++)
		{
			char* label_delimiter;

			/* If the token is empty, or non-existent, skip it */
			if(!tokens[i][token_idx]) continue;

			/* Figure out if the source line we're on contains a valid instruction */
			if(instr_to_opcode(tokens[i][token_idx]) != -1) valid_instruction = 1;

			/* Figure out if the token we're dealing with has a label delimiter */
			label_delimiter = strchr(tokens[i][token_idx], ':');

			if(label_delimiter != NULL)
			{
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

				continue;
			}
		}

		if(valid_instruction) num_instructions++;
	}

	return 0;
}

void parse_instruction(tvm_program_t* p, const char** tokens, tvm_memory_t* pMemory)
{
	int token_idx;
	for(token_idx = 0; token_idx < MAX_TOKENS; token_idx++)
	{
		int opcode = 0;

		/* Skip empty tokens */
		if(!tokens[token_idx]) continue;

		opcode = instr_to_opcode(tokens[token_idx]);

		/* If it *is* an opcode, parse the arguments */
		if(opcode != -1)
		{
			int i, instr_idx = 0, num_instr = p->num_instructions;
			p->instr[p->num_instructions++] = opcode;

			for(i = ++token_idx; i < (token_idx + 2); ++i)
			{
				char* newline;
				int valid_register = 0, j = 0;

				if(!tokens[i] || strlen(tokens[i]) <= 0) continue;

				newline = strchr(tokens[i], '\n');
				if(newline) *newline = 0;

				/* Check to see if the token specifies a register */
				while(tvm_register_map[j])
				{
					if(strcmp(tokens[i], tvm_register_map[j]) == 0)
					{
						p->args[num_instr][i - token_idx] = &pMemory->registers[j].i32;
						valid_register = 1;

						break;

					} else j++;
				}
				if(valid_register) continue;

				/* Check to see whether the token specifies an address */
				if(tokens[i][0] == '[')
				{
					char* end_symbol = strchr(tokens[i], ']');
					if(end_symbol)
					{
						*end_symbol = 0;
						p->args[num_instr][i - token_idx] = &((int*)pMemory->mem_space)[parse_value(tokens[i] + 1)];

						continue;
					}
				}

				/* Check if the argument is a label */
				instr_idx = htab_find(p->label_htab, tokens[i]);

				if(instr_idx >= 0)
				{
					p->args[num_instr][i - token_idx] = add_value(p, instr_idx);
					continue;
				}

				/* Fuck it, parse it as a value */
				p->args[num_instr][i - token_idx] = add_value(p, parse_value(tokens[i]));
			}
		}
	}
}

int instr_to_opcode(const char* instr)
{
	int i;
	for(i = 0; tvm_opcode_map[i]; i++)
		if(strcmp(instr, tvm_opcode_map[i]) == 0)
			return i;

	return -1;
}

int* add_value(tvm_program_t* p, const int val)
{
	p->values = realloc(p->values, sizeof(int*) * (p->num_values + 1));
	p->values[p->num_values] = (int*)calloc(1, sizeof(int));

	*p->values[p->num_values] = val;

	return p->values[p->num_values++];
}

int parse_value(const char* str)
{
	char* delimiter = strchr(str, '|'), base = 0;

	if(delimiter)
	{
		char* identifier = delimiter + 1;

		switch(*identifier)
		{
			case 'h': base = 16; break;
			case 'b': base = 2;  break;
			default:  base = 0;  break;
		}
	}

	return strtoul(str, NULL, base);
}
