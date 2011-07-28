#include "tvm_file.h"
#include "tvm_program.h"
#include "tvm_lexer.h"

const char* tvm_opcode_map[] = {"int", "mov", "push", "pop", "inc", "dec", "add", "sub", "mul", "div", "mod", "rem",
				"not", "xor", "or", "and", "shl", "shr", "cmp", "jmp", "je", "jne", "jg", "jge", "jl", "jle", 0};

const char* tvm_register_map[] = {"eax", "ebx", "ecx", "edx", "esi", "edi", "esp", "ebp", "eip", 0};

static void parse_labels(tvm_program_t* p, const char** tokens);
static void parse_instructions(tvm_program_t* p, const char** tokens, tvm_memory_t* pMemory);

tvm_program_t* create_program()
{
	tvm_program_t* p = (tvm_program_t*)calloc(1, sizeof(tvm_program_t));
	if(!p) return NULL;

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

	/* If no file name was specified, accept standard input */
	if(filename == NULL)
	{
		pFile = stdin;
	}
	else
	{
		/* Attempt to open the file. If the file cannot be opened, try once more. */
		for(i = 0; i < 2; i++)
			if(!pFile) pFile = tvm_fopen(filename, ".vm", "r");
	}

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

	i = 0;
	while(lexer->tokens[i])
	{
		p->instr = (int*)realloc(p->instr, sizeof(int) * (p->num_instructions + 1));
		p->args = (int***)realloc(p->args, sizeof(int**) * (p->num_instructions + 1));
		p->args[p->num_instructions] = (int**)calloc(MAX_ARGS, sizeof(int*));

		parse_labels(p, (const char**)lexer->tokens[i]);
		parse_instructions(p, (const char**)lexer->tokens[i], pMemory);
		i++;
	}

	lexer_destroy(lexer);
	free(source);

	/* Specify the end of the program */
	p->instr = realloc(p->instr, sizeof(int) * (p->num_instructions + 1));
	p->instr[p->num_instructions] = END;

	fclose(pFile);

	return 0;
}

void destroy_program(tvm_program_t* p)
{
	int i;

	destroy_htab(p->label_htab);

	for(i = 0; i < p->num_values; i++) free(p->values[i]);
	free(p->values);

	for(i = 0; i < p->num_instructions; i++) free(p->args[i]);
	free(p->args);

	free(p->instr);
	free(p);
}

void parse_labels(tvm_program_t* p, const char** tokens)
{
	int token_idx;

	for(token_idx = 0; token_idx < MAX_TOKENS; token_idx++)
	{
		/* Pointer to the label delimiter in the token */
		char* label_delimiter;

		/* Skip empty tokens */
		if(!tokens[token_idx]) continue;

		/* Figure out if the token we're dealing with has a label delimiter */
		label_delimiter = strchr(tokens[token_idx], ':');

		if(label_delimiter != NULL)
		{
			*label_delimiter = 0;

			/* If the label is "start," set the program to begin there */
			if(strcmp(tokens[token_idx], "start") == 0) p->start = p->num_instructions;

			/* Add that fucker to the hash table with the corresponding instruction index */
			htab_add(p->label_htab, tokens[token_idx], p->num_instructions);

			++token_idx;
		}
	}
}

void parse_instructions(tvm_program_t* p, const char** tokens, tvm_memory_t* pMemory)
{
	int token_idx;
	for(token_idx = 0; token_idx < MAX_TOKENS; token_idx++)
	{
		int valid_opcode = 0, i = 0;

		/* Skip empty tokens */
		if(!tokens[token_idx]) continue;

		while(tvm_opcode_map[i])
		{
			if(strcmp(tokens[token_idx], tvm_opcode_map[i]) == 0)
			{
				p->instr[p->num_instructions] = i;
				valid_opcode = 1;

				break;
			}
			else i++;
		}

		/* If it *is* an opcode, parse the arguments */
		if(valid_opcode)
		{
			int i, num_instr = p->num_instructions;
			++p->num_instructions;

			for(i = ++token_idx; i < (token_idx + 2); ++i)
			{
				int j;
				char* newline;
				int valid_register;

				if(!tokens[i] || strlen(tokens[i]) <= 0) continue;

				newline = strchr(tokens[i], '\n');
				if(newline) *newline = 0;

				/* Check to see if the token specifies a register */
				j = 0;
				valid_register = 0;

				while(tvm_register_map[j])
				{
					if(strcmp(tokens[i], tvm_register_map[j]) == 0)
					{
						p->args[num_instr][i - token_idx] = &pMemory->registers[j].i32;
						valid_register = 1;

						break;
					}
					else j++;
				}
				if(valid_register) continue;

				/* Check to see whether the token specifies an address */
				if(tokens[i][0] == '[')
				{
					char* end_symbol = strchr(tokens[i], ']');
					if(end_symbol) *end_symbol = 0;

					p->args[num_instr][i - token_idx] = &((int*)pMemory->mem_space)[parse_value(tokens[i] + 1)];
					continue;
				}
				/* If it's not an address, check if the argument is a label */
				else
				{
					int instr_idx = htab_find(p->label_htab, tokens[i]);

					if(instr_idx >= 0)
					{
						p->args[num_instr][i - token_idx] = add_value(p, instr_idx);
						continue;
					}
				}

				/* If it's not an address, and it's not a label, parse it as a value */
				p->args[num_instr][i - token_idx] = add_value(p, parse_value(tokens[i]));
			}
		}
	}
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
	char* delimiter = strchr(str, ':');
	int base = 0;

	if(delimiter)
	{
		char* identifier = delimiter + 1;

		switch(*identifier)
		{
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
