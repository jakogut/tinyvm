#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

hash_table* create_htab()
{
	hash_table* htab;

	htab = (hash_table*)malloc(sizeof(hash_table));
	int i; for(i = 0; i < HTAB_SIZE; i++) htab->nodes[i] = NULL;

	return htab;
}

void destroy_htab(hash_table* htab)
{
	int i; for(i = 0; i < HTAB_SIZE; i++)
	{
		if(htab->nodes[i])
		{
			free(htab->nodes[i]->key);
			free(htab->nodes[i]);
		}
	}

	free(htab);

	htab = NULL;
}

int htab_add(hash_table* htab, char* k, int v)
{
	int hash = htab_hash(k);

	// If the node is not already occupied, allocate space, and copy the key/value pair.
	if(htab->nodes[hash] == NULL) htab->nodes[hash] = malloc(sizeof(htable_node));
	else return 1;

	htab->nodes[hash]->key = (char*)malloc(sizeof(char) * (strlen(k) + 1));

	strcpy(htab->nodes[hash]->key, k);
	htab->nodes[hash]->value = v;

	return 0;
}

int htab_find(hash_table* htab, char* key)
{
	int hash = htab_hash(key);

	if(htab->nodes[hash]) return htab->nodes[hash]->value;
	else return -1;
}

unsigned int htab_hash(char* k)
{
	unsigned int hash = 1;

	char* c; for(c = k; *c; c++)
		hash += (hash << *c) - *c;

	return hash % HTAB_SIZE;
}
