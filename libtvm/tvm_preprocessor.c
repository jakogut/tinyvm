#include <tvm/tvm_preprocessor.h>
#include <tvm/tvm_file.h>

#include <string.h>

int tvm_preprocess(char *src, int *src_len, tvm_htab_t *htab)
{
	char* pp_directive_delimiter = NULL;
	if((pp_directive_delimiter = strstr(src, "%include")))
	{
		char *strbegin = pp_directive_delimiter, *strend = strchr(strbegin, '\n');

		if(!strend || !strbegin)  return 0;

		int linelen = strend - strbegin;
		char* temp_str = calloc(linelen + 1, sizeof(char));
		memcpy(temp_str, strbegin, linelen);

		char *filename = (strchr(temp_str, ' ') + 1);

		FILE* pFile = tvm_fopen(filename, ".vm", "r");
		if(!pFile)
		{
			printf("Unable to open file \"%s\"\n", filename);
			return 0;
		}

		free(temp_str);

		size_t addition_len = tvm_flength(pFile);
		char *addition_str = calloc(addition_len, sizeof(char));
		tvm_fcopy(addition_str, addition_len, pFile);
		fclose(pFile);

		size_t first_block_len = (strbegin - src);
		size_t second_block_len = ((src + *src_len) - strend);
		size_t new_src_len = (first_block_len + addition_len + second_block_len);

		src = (char *)realloc((char *)src, sizeof(char) * new_src_len);
		src[new_src_len] = 0;

		memmove(&src[first_block_len + addition_len], strend, second_block_len);
		memcpy(&src[first_block_len], addition_str, addition_len);

		// Fuckin' hack
		for(int i = 0; i < new_src_len; i++) if(src[i] == 0) src[i] = ' ';

		*src_len = strlen(src);
		return 1;
	}
	else if ((pp_directive_delimiter = strstr(src, "%define")))
	{
		char *strbegin = pp_directive_delimiter, *strend = strchr(strbegin, '\n');

		if (!strend || !strbegin) return 0;

		char *define = (strchr(strbegin, ' ') + 1);
		char *valstr = define ? (strchr(define, ' ') + 1) : NULL;

		//Limit searching for value string to define's line
		if (!define || !valstr || (valstr > strend)) {
			*strend = 0;
			printf("Define missing arguments: \"%s\"\n", strbegin);
			*strend = '\n';
			return 0;
		}

		*strend = '\n';

		char *tmp = valstr - 1;
		*tmp = 0;

		int value = atoi(valstr);

		htab_add(htab, define, value);

		size_t first_block_len = (strbegin - src);
		size_t second_block_len = ((src + *src_len) - strend);
		size_t new_src_len = (first_block_len + second_block_len);

		memcpy(&src[first_block_len], strend, second_block_len);

		src = (char *)realloc((char *)src, sizeof(char) * new_src_len);
		src[new_src_len] = 0;

		*src_len = new_src_len;

		return 1;
	}

	return 0;

}
