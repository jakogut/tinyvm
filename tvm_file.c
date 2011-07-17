#include "tvm_file.h"

FILE* tvm_fopen(const char* filename, const char* extension, const char* mode)
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

void tvm_fclose(FILE* f)
{
	fclose(f);
}

int tvm_fcopy(char* dest, size_t size, FILE* src)
{
	fpos_t pos;
	fgetpos(src, &pos);

	int i;
	for(i = 0; i < size && !feof(src); i++)
	{
		dest[i] = fgetc(src);
	}

	fsetpos(src, &pos);

	return 0;
}

int tvm_flength(FILE* f)
{
	fpos_t pos;
	fgetpos(f, &pos);

	int length = 0;

	while(!feof(f))
	{
		fgetc(f);
		length++;
	}

	fsetpos(f, &pos);

	return length;
}
