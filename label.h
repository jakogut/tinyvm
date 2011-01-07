#ifndef LABEL_H_
#define LABEL_H_

typedef struct
{
	char* name;
	int instruction;
} label;

label* create_label(char* name, int* instruction);
void destroy_label(label* l);

#endif
