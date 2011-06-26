#include "tvm_file.h"

FILE* tvm_openfile(const char* filename, const char* extension, const char* mode)
{
	char* fname = malloc(sizeof(char) * (strlen(filename) + 5));
	strcpy(fname, filename);

	FILE* pFile;
	pFile = fopen(fname, mode);

	if(!pFile)
	{
		strcat(fname, extension);
		pFile = fopen(fname, mode);

		if(!pFile) return NULL;
	}

	free(fname);
	return pFile;
}
