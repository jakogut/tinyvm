#include <tvm/tvm_file.h>

FILE *tvm_fopen(const char *filename, const char *extension, const char *mode)
{
	FILE *filp = NULL;
	size_t fname_chars = strlen(filename) + strlen(extension) + 1;
	char *fname = calloc(fname_chars, sizeof(char));
	int i;

	strcpy(fname, filename);

	for (i = 0; i < 2 && !filp; i++) {
		if (i > 0)
			strcat(fname, extension);
		filp = fopen(fname, mode);
	}

	free(fname);
	return filp;
}

int tvm_fcopy(char *dest, size_t size, FILE *filp)
{
	size_t i;
	long pos = ftell(filp);

	for (i = 0; i < size && !feof(filp); i++)
		dest[i] = fgetc(filp);
	dest[i - 1] = 0;

	fseek(filp, pos, SEEK_SET);

	return 0;
}

int tvm_flength(FILE *filp)
{
	int length;
	long pos = ftell(filp);

	for (length = 0; !feof(filp); length++)
		fgetc(filp);
	fseek(filp, pos, SEEK_SET);

	return length;
}
