#include <tvm/tvm_lexer.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tvm_lexer_ctx *lexer_create()
{
	return (struct tvm_lexer_ctx *)calloc(1, sizeof(struct tvm_lexer_ctx));
}

void tvm_lexer_destroy(struct tvm_lexer_ctx *lexer)
{
	for (int i = 0; lexer->source_lines[i]; i++)
		free(lexer->source_lines[i]);
	free(lexer->source_lines);

	for (int i = 0; lexer->tokens[i]; i++) {
		for (int j = 0; j < MAX_TOKENS; j++)
			free(lexer->tokens[i][j]);

		free(lexer->tokens[i]);
	}

	free(lexer->tokens);
	free(lexer);
}

void tvm_lex(
	struct tvm_lexer_ctx *lexer, char *source, struct tvm_htab_ctx *defines)
{
	int i, j;
	char *tokp, *linep = strtok(source, "\n");

	/* Split the source into individual lines */
	for (i = 0; linep; i++) {
		lexer->source_lines = (char **)realloc(
			lexer->source_lines, sizeof(char *) * (i + 2));
		lexer->source_lines[i] = (char *)calloc(1, strlen(linep) + 1);

		strcpy(lexer->source_lines[i], linep);

		/* Ignore comments delimited by '#' */
		char *comment_delimiter = strchr(lexer->source_lines[i], '#');

		if (comment_delimiter)
			*comment_delimiter = 0;

		linep = strtok(NULL, "\n");
	}

	/* NULL terminate the array to make iteration later easier*/
	lexer->source_lines[i] = NULL;

	/* Split the source into individual tokens */
	for (i = 0; lexer->source_lines[i]; i++) {
		lexer->tokens = (char ***)realloc(
			lexer->tokens, sizeof(char **) * (i + 2));
		lexer->tokens[i] = (char **)calloc(MAX_TOKENS, sizeof(char *));

		tokp = strtok(lexer->source_lines[i], " \t,");

		for (j = 0; (tokp && j < MAX_TOKENS); j++) {
			char *token = tvm_htab_find_ref(defines, tokp);

			token = token ? token : tokp;
			lexer->tokens[i][j] = calloc(1, (strlen(token) + 1));
			strcpy(lexer->tokens[i][j], token);

			tokp = strtok(NULL, " \t,");
		}
	}

	lexer->tokens[i] = NULL;
	tvm_htab_destroy(defines);
}
