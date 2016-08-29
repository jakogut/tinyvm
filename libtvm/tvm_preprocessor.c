#include <tvm/tvm_preprocessor.h>
#include <tvm/tvm_tokens.h>
#include <tvm/tvm_file.h>

#include <string.h>

static int process_includes(
	char *src, int *src_len, struct tvm_htab_ctx *defines)
{
	char *pp_directive_delimiter = strstr(src, TOK_INCLUDE);

	if (!pp_directive_delimiter)
		return 0;

	char *strbegin = pp_directive_delimiter,
		 *strend = strchr(strbegin, '\n');

	if (!strend || !strbegin)
		return 0;

	int linelen = strend - strbegin;
	char *temp_str = calloc(linelen + 1, sizeof(char));

	memcpy(temp_str, strbegin, linelen);

	char *filename = (strchr(temp_str, ' ') + 1);
	FILE *filp = tvm_fopen(filename, ".vm", "r");

	if (!filp) {
		printf("Unable to open file \"%s\"\n", filename);
		return -1;
	}

	free(temp_str);

	size_t addition_len = tvm_flength(filp);
	char *addition_str = calloc(addition_len, sizeof(char));

	tvm_fcopy(addition_str, addition_len, filp);
	fclose(filp);

	size_t first_block_len = (strbegin - src);
	size_t second_block_len = ((src + *src_len) - strend);
	size_t new_src_len = (
		first_block_len + addition_len + second_block_len);

	src = (char *)realloc((char *)src, sizeof(char) * new_src_len);
	src[new_src_len] = 0;

	memmove(&src[first_block_len + addition_len], strend, second_block_len);
	memcpy(&src[first_block_len], addition_str, addition_len);

	/* Fuckin' hack */
	for (int i = 0; i < new_src_len; i++) {
		if (src[i] == 0)
			src[i] = ' ';
	}

	*src_len = strlen(src);
	return 1;
}

static int process_defines(
	char *src, int *src_len, struct tvm_htab_ctx *defines)
{
	char *pp_directive_delimiter = strstr(src, TOK_DEFINE);

	if (!pp_directive_delimiter)
		return 0;

	char *begin = pp_directive_delimiter;
	char *end = strchr(begin, '\n');

	if (!end)
		return 0;

	int offset = strlen(TOK_DEFINE) + 1;

	if (begin + offset >= end) {
		printf("Define missing arguments.\n");
		return -1;
	}

	int length = (end - (begin + offset));
	char tempstr[length + 1];

	memset(tempstr, 0, length + 1);
	memcpy(tempstr, begin + offset, length);

	char *keystr = tempstr;
	char *valstr = strchr(tempstr, ' ');

	/* If there is a value, seperate the key and value
	 * with a null character.
	 */
	if (valstr) {
		*valstr = 0;
		valstr += 1;
	}

	if (!keystr || !valstr) {
		printf("Define missing arguments.\n");
		return -1;
	}

	if (tvm_htab_find(defines, keystr) < 0)
		tvm_htab_add_ref(defines, keystr, valstr, strlen(valstr) + 1);
	else {
		printf("Multiple definitions for %s.\n", keystr);
		return -1;
	}

	/* Remove the define line so it is not processed again. */
	size_t new_length = *src_len - (end - begin);
	size_t first_block_len = begin - src;
	size_t second_block_len = (src + *src_len) - end;

	memmove(&src[first_block_len], end, second_block_len);

	src = realloc(src, sizeof(char) * new_length);
	*src_len = new_length;

	return 1;
}

static int preprocess_pass(
	char *src, int *src_len, struct tvm_htab_ctx *defines)
{
	int ret = 0;

	ret += process_includes(src, src_len, defines);
	ret += process_defines(src, src_len, defines);
	return ret;
}


int tvm_preprocess(char *src, int *src_len, struct tvm_htab_ctx *defines)
{
	int ret = 1;

	while (ret > 0)
		ret = preprocess_pass(src, src_len, defines);

	return ret;
}
