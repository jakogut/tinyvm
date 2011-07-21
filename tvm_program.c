#include "tvm_file.h"
#include "tvm_program.h"

#define MAX_ARGS 2
#define MAX_TOKENS 4

static void tokenize_line(char** tokens, char* line);
static void parse_labels(tvm_program_t* p, char** tokens);
static void parse_instructions(tvm_program_t* p, char** tokens, tvm_memory_t* pMemory);

tvm_program_t* create_program()
{
	// Allocate space for our program
	tvm_program_t* p = (tvm_program_t*)calloc(1, sizeof(tvm_program_t));

	// Create our label hash table
	p->label_htab = create_htab();

	return p;
}

int interpret_program(tvm_program_t* p, char* filename, tvm_memory_t* pMemory)
{
	FILE* pFile = NULL;

	// Attempt to open the file. If the file cannot be opened, try once more.
	int i;
	for(i = 0; i < 2; i++)
		if(!pFile) pFile = tvm_fopen(filename, ".vm", "r");

	if(!pFile)
	{
		printf("File was not found, or does not exist. Unable to interpret.\n");
		return 1;
	}

	char** tokens = calloc(MAX_TOKENS, sizeof(char*));

	for(i = 0; i < MAX_TOKENS; i++)
		tokens[i] = calloc(32, sizeof(char));

	char line[128] = {0};

	// Get one line from the source file
	while(fgets(line, 128, pFile))
	{
		p->instr = realloc(p->instr, sizeof(int) * (p->num_instructions + 1));
		p->args = realloc(p->args, sizeof(int**) * (p->num_instructions + 1));
		p->args[p->num_instructions] = calloc(MAX_ARGS, sizeof(int*));

		tokenize_line(tokens, line);
		parse_labels(p, tokens);
		parse_instructions(p, tokens, pMemory);
	}

	for(i = 0; i < MAX_TOKENS; i++)
		if(tokens[i]) free(tokens[i]);

	if(tokens) free(tokens);

	// Specify the end of the program
	p->instr = realloc(p->instr, sizeof(int) * (p->num_instructions + 1));
	p->instr[p->num_instructions] = END;

	fclose(pFile);

	return 0;
}

void destroy_program(tvm_program_t* p)
{
	destroy_htab(p->label_htab);

	int i;
	for(i = 0; i < p->num_values; i++) free(p->values[i]);
	free(p->values);

	for(i = 0; i < p->num_instructions; i++) free(p->args[i]);
	free(p->args);

	free(p->instr);
	free(p);
}

void tokenize_line(char** tokens, char* line)
{
	int token_idx = 0;
	char* pToken;

	pToken = strtok(line, "	 ,");

	while(pToken)
	{
		// Ignore comments
		char* comment_delimiter = strchr(pToken, '#');

		if(comment_delimiter)
			*comment_delimiter = 0;

		if(pToken)
			strcpy(tokens[token_idx], pToken);

		if(comment_delimiter) break;
		else pToken = strtok(NULL, "	 ,");

		++token_idx;
	}
}

void parse_labels(tvm_program_t* p, char** tokens)
{
	int token_idx;

	for(token_idx = 0; token_idx < MAX_TOKENS; token_idx++)
	{
		// Figure out if the token we're dealing with is a label
		char* label_delimiter = strchr(tokens[token_idx], ':');

		if(label_delimiter != NULL)
		{
			// Get rid of the label delimiter
			*label_delimiter = 0;

			// If the label is "start," set the program to begin there.
			if(strcmp(tokens[token_idx], "start") == 0) p->start = p->num_instructions;

			// Add that fucker to the hash table with the corresponding instruction index
			htab_add(p->label_htab, tokens[token_idx], p->num_instructions);

			// Advance to the next token
			++token_idx;
		}
	}
}

void parse_instructions(tvm_program_t* p, char** tokens, tvm_memory_t* pMemory)
{
	int token_idx;
	for(token_idx = 0; token_idx < MAX_TOKENS; token_idx++)
	{
		// Figure out if the token we're dealing with is an opcode
		int valid_opcode = 1;

		if(strcmp(tokens[token_idx], "mov") == 0)	p->instr[p->num_instructions] = MOV;
		else if(strcmp(tokens[token_idx], "push") == 0)	p->instr[p->num_instructions] = PUSH;
		else if(strcmp(tokens[token_idx], "pop") == 0)	p->instr[p->num_instructions] = POP;
		else if(strcmp(tokens[token_idx], "inc") == 0)	p->instr[p->num_instructions] = INC;
		else if(strcmp(tokens[token_idx], "dec") == 0)	p->instr[p->num_instructions] = DEC;
		else if(strcmp(tokens[token_idx], "add") == 0)	p->instr[p->num_instructions] = ADD;
		else if(strcmp(tokens[token_idx], "sub") == 0)	p->instr[p->num_instructions] = SUB;
		else if(strcmp(tokens[token_idx], "mul") == 0)	p->instr[p->num_instructions] = MUL;
		else if(strcmp(tokens[token_idx], "div") == 0)	p->instr[p->num_instructions] = DIV;
		else if(strcmp(tokens[token_idx], "mod") == 0)	p->instr[p->num_instructions] = MOD;
		else if(strcmp(tokens[token_idx], "rem") == 0)	p->instr[p->num_instructions] = REM;
		else if(strcmp(tokens[token_idx], "not") == 0)	p->instr[p->num_instructions] = NOT;
		else if(strcmp(tokens[token_idx], "xor") == 0)	p->instr[p->num_instructions] = XOR;
		else if(strcmp(tokens[token_idx], "or") == 0)	p->instr[p->num_instructions] = OR;
		else if(strcmp(tokens[token_idx], "and") == 0)	p->instr[p->num_instructions] = AND;
		else if(strcmp(tokens[token_idx], "shl") == 0)	p->instr[p->num_instructions] = SHL;
		else if(strcmp(tokens[token_idx], "shr") == 0)	p->instr[p->num_instructions] = SHR;
		else if(strcmp(tokens[token_idx], "cmp") == 0)	p->instr[p->num_instructions] = CMP;
		else if(strcmp(tokens[token_idx], "jmp") == 0)	p->instr[p->num_instructions] = JMP;
		else if(strcmp(tokens[token_idx], "je") == 0)	p->instr[p->num_instructions] = JE;
		else if(strcmp(tokens[token_idx], "jne") == 0)	p->instr[p->num_instructions] = JNE;
		else if(strcmp(tokens[token_idx], "jg") == 0)	p->instr[p->num_instructions] = JG;
		else if(strcmp(tokens[token_idx], "jge") == 0)	p->instr[p->num_instructions] = JGE;
		else if(strcmp(tokens[token_idx], "jl") == 0)	p->instr[p->num_instructions] = JL;
		else if(strcmp(tokens[token_idx], "jle") == 0)	p->instr[p->num_instructions] = JLE;
		else
			valid_opcode = 0;

		// If it *is* an opcode, parse the arguments
		if(valid_opcode)
		{
			int i, num_instr = p->num_instructions;
			++p->num_instructions;

			for(i = ++token_idx; i < (token_idx + 2); ++i)
			{
				// If the token is empty, do not attempt to parse it
				if(strlen(tokens[i]) <= 0) continue;

				// Remove any trailing newline from the token
				char* newline = strchr(tokens[i], '\n');
				if(newline) *newline = 0;

				// Check to see whether the token specifies an address
				if(tokens[i][0] == '[')
				{
					char* end_symbol = strchr(tokens[i], ']');
					if(end_symbol) *end_symbol = 0;

					p->args[num_instr][i - token_idx] = &((int*)pMemory->mem_space)[parse_value(tokens[i] + 1)];
					continue;
				}
				// If it's not an address, check if the argument is a label
				else
				{
					// Search the hash map for the label
					int instr_idx = htab_find(p->label_htab, tokens[i]);

					// If the label was found, create the argument
					if(instr_idx >= 0)
					{
						p->args[num_instr][i - token_idx] = add_value(p, instr_idx);
						continue;
					}
				}

				// If it's not an address, and it's not a label, parse it as a value
				int value = parse_value(tokens[i]);
				p->args[num_instr][i - token_idx] = add_value(p, value);
			}
		}
	}
}

int* add_value(tvm_program_t* p, const int val)
{
	p->values = realloc(p->values, sizeof(int*) * (p->num_values + 1));
	p->values[p->num_values] = calloc(1, sizeof(int));

	*p->values[p->num_values] = val;

	return p->values[p->num_values++];
}

int parse_value(char* str)
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
