#include "program.h"
#include "instruction_index.h"

program* create_program(char* filename)
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

	p->num_labels = 0;
	p->num_instructions = 0;

	char line[128];
	memset(line, 0, 128);

	// Create our label hash table
	p->label_htab = create_htab();

	p->instr = (unsigned*)malloc(sizeof(unsigned int) * 1);
	p->args = (argument**)malloc(sizeof(argument*) * 1);
	p->args[p->num_instructions] = (argument*)malloc(sizeof(argument) * MAX_ARGS);

	// Get one line from the source file
	while(fgets(line, 128, pFile))
	{
		line[strlen(line)] = '\0';

		char tokens[4][32];
		char* token = 0;

		int token_idx = 0;

		// Tokenize our source line
		token = strtok(line, "	 ,");

		while(token)
		{
			if(token) strcpy(tokens[token_idx++], token);
			token = strtok(NULL, "	 ,");
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
					argument* pArg = &p->args[p->num_instructions][i - token_idx];

					if(tokens[i][0] == '#')
					{
						pArg->type = ARG_TYPE_VALUE;
						pArg->value = atoi(tokens[i] + 1);
					}
					else if(tokens[i][0] == '[' && tokens[i][2] == ']')
					{
						pArg->type = ARG_TYPE_ADDRESS;
						pArg->value = atoi(tokens[i] + 1);
					}
					// If it's not a value, and it's not an address, it must be a label!
					// (NOTE: This is a *very* bad assumption, and must be fixed later.)
					else
					{
						// Remove any troublesome trailing newline character
						char* newline = strchr(tokens[i], '\n');
						if(newline) *newline = 0;

						// Search the hash map for the label
						int instr_idx =  htab_find(p->label_htab, tokens[i]);

						// If the label was found, create the argument
						if(instr_idx >= 0)
						{
							pArg->value = instr_idx;
							pArg->type = ARG_TYPE_LABEL;
						}
					}
				}
			}
		}

		++p->num_instructions;

		p->instr = (unsigned*)realloc(p->instr, sizeof(unsigned int) * p->num_instructions + 1);
		p->args = (argument**)realloc(p->args, sizeof(argument*) * p->num_instructions + 1);
		p->args[p->num_instructions] = (argument*)realloc(p->args[p->num_instructions], sizeof(argument) * MAX_ARGS);

	}

	/* DEBUG */ printf("%i instructions\n", p->num_instructions); /* DEBUG */

	fclose(pFile);
	return p;
}

void destroy_program(program* p)
{
	if(p->label_htab) destroy_htab(p->label_htab);

	int i;
	if(p->args[0])
		for(i = 0; i < p->num_instructions; i++) free(p->args[i]);

	if(p->args) free(p->args);
	if(p->instr) free(p->instr);

	if(p) free(p);
}

void print_warning(char* str, int line_number)
{
	printf("WARNING: %i: %s\n\n", line_number, str);
}

void print_error(char* str, int line_number)
{
	printf("ERROR: %i: %s\n\n", line_number, str);
}
