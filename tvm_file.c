#include "tvm_file.h"

FILE* tvm_openfile(const char* filename, const char* extension, const char* mode)
{
	size_t fname_chars = strlen(filename) + strlen(extension) + 1;
	char* fname = malloc(sizeof(char) * fname_chars);
	strcpy(fname, filename);

	FILE* pFile;
	pFile = fopen(fname, mode);

	if(!pFile)
	{
		strcat(fname, extension);
		pFile = fopen(fname, mode);
	}

	free(fname);
	return pFile;
}
