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
	int i, j;
	for(i = 0; lexer->source_lines[i]; i++) free(lexer->source_lines[i]);
	free(lexer->source_lines);

	for(i = 0; lexer->tokens[i]; i++)
	{
		for(j = 0; j < MAX_TOKENS; j++)
			free(lexer->tokens[i][j]);

		free(lexer->tokens[i]);
	}

	free(lexer->tokens);
	free(lexer);
}

void lex(tvm_lexer_t* lexer, char* source)
{
	int i, j;
	char *pToken, *pLine = strtok(source, "\n");

	/* Split the source into individual lines */
	for(i = 0; pLine; i++)
	{
		char* comment_delimiter;

		lexer->source_lines = (char**)realloc(lexer->source_lines, sizeof(char*) * (i + 2));
		lexer->source_lines[i] = (char*)calloc(1, strlen(pLine) + 1);

		strcpy(lexer->source_lines[i], pLine);

		/* Ignore comments delimited by '#' */
		comment_delimiter = strchr(lexer->source_lines[i], '#');

		if(comment_delimiter) *comment_delimiter = 0;

		pLine = strtok(NULL, "\n");
	}

	/* NULL terminate the array to make iteration later easier*/
	lexer->source_lines[i] = NULL;

	/* Split the source into individual tokens */
	for(i = 0; lexer->source_lines[i]; i++)
	{
		lexer->tokens = (char***)realloc(lexer->tokens, sizeof(char**) * (i + 2));
		lexer->tokens[i] = (char**)calloc(MAX_TOKENS, sizeof(char*));

		pToken = strtok(lexer->source_lines[i], " 	,");

		for(j = 0; (pToken && j < MAX_TOKENS); j++)
		{
			lexer->tokens[i][j] = (char*)calloc(1, (strlen(pToken) + 1));
			strcpy(lexer->tokens[i][j], pToken);

			pToken = strtok(NULL, " 	,");
		}
	}

	lexer->tokens[i] = NULL;
}
