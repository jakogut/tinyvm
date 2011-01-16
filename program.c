#include "program.h"

#define MAX_ARGS 2

program* create_program(char* filename, memory* pMemory)
{
	// Open our file in read-only mode
	FILE* pFile = fopen(filename, "r");

	if(!pFile)
	{
		printf("File was not found, or does not exist. Unable to interpret.\n");
		return NULL;
	}

	// Allocate space for our program
	program* p = (program*)malloc(sizeof(program));

	// Initialize the members of program
	p->start = 0;
	p->num_instructions = 0;
	p->num_values = 0;

	// Create our label hash table
	p->label_htab = create_htab();

	char line[128];
	memset(line, 0, 128);

	// Get one line from the source file
	while(fgets(line, 128, pFile))
	{
		p->instr = realloc(p->instr, sizeof(int) * (p->num_instructions + 1));
		p->args = realloc(p->args, sizeof(int**) * (p->num_instructions + 1));
		p->args[p->num_instructions] = malloc(sizeof(int*) * MAX_ARGS);

		line[strlen(line) - 1] = '\0';

		char tokens[4][32];
		memset(tokens, 0, 4 * 32);

		char* pToken = 0;
		int token_idx = 0;

		// Tokenize our source line
		pToken = strtok(line, "	 ,");

		while(pToken)
		{
			if(pToken) strcpy(tokens[token_idx], pToken);
			else strcpy(tokens[token_idx], "");

			pToken = strtok(NULL, "	 ,");

			++token_idx;
		}

		int valid_opcode = 1;
		for(token_idx = 0; token_idx < 4; token_idx++)
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

			// Figure out if the token we're dealing with is an opcode
			if(strcmp(tokens[token_idx], "mov") == 0)
				p->instr[p->num_instructions] = MOV;
			else if(strcmp(tokens[token_idx], "push") == 0)
				p->instr[p->num_instructions] = PUSH;
			else if(strcmp(tokens[token_idx], "pop") == 0)
				p->instr[p->num_instructions] = POP;
			else if(strcmp(tokens[token_idx], "inc") == 0)
				p->instr[p->num_instructions] = INC;
			else if(strcmp(tokens[token_idx], "dec") == 0)
				p->instr[p->num_instructions] = DEC;
			else if(strcmp(tokens[token_idx], "add") == 0)
				p->instr[p->num_instructions] = ADD;
			else if(strcmp(tokens[token_idx], "sub") == 0)
				p->instr[p->num_instructions] = SUB;
			else if(strcmp(tokens[token_idx], "mul") == 0)
				p->instr[p->num_instructions] = MUL;
			else if(strcmp(tokens[token_idx], "div") == 0)
				p->instr[p->num_instructions] = DIV;
			else if(strcmp(tokens[token_idx], "mod") == 0)
				p->instr[p->num_instructions] = MOD;
			else if(strcmp(tokens[token_idx], "rem") == 0)
				p->instr[p->num_instructions] = REM;
			else if(strcmp(tokens[token_idx], "not") == 0)
				p->instr[p->num_instructions] = NOT;
			else if(strcmp(tokens[token_idx], "xor") == 0)
				p->instr[p->num_instructions] = XOR;
			else if(strcmp(tokens[token_idx], "or") == 0)
				p->instr[p->num_instructions] = OR;
			else if(strcmp(tokens[token_idx], "and") == 0)
				p->instr[p->num_instructions] = AND;
			else if(strcmp(tokens[token_idx], "shl") == 0)
				p->instr[p->num_instructions] = SHL;
			else if(strcmp(tokens[token_idx], "shr") == 0)
				p->instr[p->num_instructions] = SHR;
			else if(strcmp(tokens[token_idx], "cmp") == 0)
				p->instr[p->num_instructions] = CMP;
			else if(strcmp(tokens[token_idx], "jmp") == 0)
				p->instr[p->num_instructions] = JMP;
			else if(strcmp(tokens[token_idx], "je") == 0)
				p->instr[p->num_instructions] = JE;
			else if(strcmp(tokens[token_idx], "jne") == 0)
				p->instr[p->num_instructions] = JNE;
			else if(strcmp(tokens[token_idx], "jg") == 0)
				p->instr[p->num_instructions] = JG;
			else if(strcmp(tokens[token_idx], "jge") == 0)
				p->instr[p->num_instructions] = JGE;
			else if(strcmp(tokens[token_idx], "jl") == 0)
				p->instr[p->num_instructions] = JL;
			else if(strcmp(tokens[token_idx], "jle") == 0)
				p->instr[p->num_instructions] = JLE;
			else
				valid_opcode = 0;

			// If it *is* an opcode, parse the arguments
			if(valid_opcode)
			{
				int i;

				for(i = ++token_idx; i < (token_idx + 2); i++)
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

						p->args[p->num_instructions][i - token_idx] = &pMemory->int32[parse_value(tokens[i] + 1)];
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
							p->args[p->num_instructions][i - token_idx] = add_value(p, instr_idx);
							continue;
						}
					}

					// If it's not an address, and it's not a label, parse it as a value
					int value = parse_value(tokens[i]);
					p->args[p->num_instructions][i - token_idx] = add_value(p, value);
				}
			}
		}

		++p->num_instructions;
	}

	// Specify the end of the program
	p->instr = realloc(p->instr, sizeof(int) * (p->num_instructions + 1));
	p->instr[p->num_instructions] = END;

	fclose(pFile);
	return p;
}

void destroy_program(program* p)
{
	destroy_htab(p->label_htab);

	int i;

	for(i = 0; i < p->num_values; i++) free(p->values[i]);
	if(p->values) free(p->values);

	for(i = 0; i < p->num_instructions; i++) free(p->args[i]);
	if(p->args) free(p->args);

	if(p->instr) free(p->instr);
	if(p) free(p);
}

int* add_value(program* p, const int val)
{
	p->values = realloc(p->values, sizeof(int*) * (p->num_values + 1));
	p->values[p->num_values] = malloc(sizeof(int));

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

void print_warning(char* str, int line_number)
{
	printf("WARNING: %i: %s\n\n", line_number, str);
}

void print_error(char* str, int line_number)
{
	printf("ERROR: %i: %s\n\n", line_number, str);
}
