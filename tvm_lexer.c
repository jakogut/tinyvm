#include "tvm_lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

tvm_lexer_t* lexer_create()
{
	return (tvm_lexer_t*)calloc(1, sizeof(tvm_lexer_t));
}

void lexer_destroy(tvm_lexer_t* lexer)
{
	if(lexer) free(lexer);
	lexer = NULL;
}

int lex(tvm_lexer_t* lexer, char* source)
{
	int i = 0, j = 0;

	char* pToken;
	char* pLine = strtok(source, "\n");

	/* Split the source into individual lines */
	while(pLine)
	{
		char* comment_delimiter;

		lexer->source_lines = (char**)realloc(lexer->source_lines, sizeof(char*) * (i + 1));
		lexer->source_lines[i] = (char*)calloc(1, strlen(pLine) + 1);

		strcpy(lexer->source_lines[i], pLine);

		/* Ignore comments delimited by '#' */
		comment_delimiter = strchr(lexer->source_lines[i], '#');

		if(comment_delimiter)
			*comment_delimiter = 0;

		pLine = strtok(NULL, "\n");
		i++;
	}

	lexer->source_lines[i] = NULL;

	/* Split the source into individual tokens */
	i = 0;
	while(lexer->source_lines[i])
	{
		lexer->tokens = (char***)realloc(lexer->tokens, sizeof(char**) * (i + 1));
		lexer->tokens[i] = (char**)calloc(MAX_TOKENS, sizeof(char*));

		pToken = strtok(lexer->source_lines[i], " 	,");

		j = 0;
		while(pToken && j < MAX_TOKENS)
		{
			lexer->tokens[i][j] = (char*)calloc(1, (strlen(pToken) + 1));
			strcpy(lexer->tokens[i][j], pToken);

			pToken = strtok(NULL, " 	,");
			j++;

		}

		i++;
	}

	lexer->tokens[i] = NULL;
	return 0;
}
