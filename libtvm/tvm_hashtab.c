#include <stdlib.h>
#include <string.h>

#include <tvm/tvm_hashtab.h>

tvm_htab_t* create_htab()
{
	return (tvm_htab_t*)calloc(1, sizeof(tvm_htab_t));
}

void destroy_htab(tvm_htab_t* htab)
{
	int i;
	tvm_htable_node_t *node, *next;

	for(i = 0; i < HTAB_SIZE; i++)
	{
		node = htab->nodes[i];
		while(node)
		{
			next = node->next;
			free(node->key);
			free(node);
			node = next;
		}
	}

	free(htab);
}

int htab_add(tvm_htab_t* htab, const char* k, int v)
{
	int hash = htab_hash(k);
	tvm_htable_node_t *node = htab->nodes[hash];
	tvm_htable_node_t *prev = NULL;

	if(node)
	{
		while(node->next)
			node = node->next;

		prev = node;
		node = node->next;
	}

	/* Allocate space, and copy the key/value pair. */

	node = calloc(1, sizeof(tvm_htable_node_t));

	node->key = (char*)malloc(sizeof(char) * (strlen(k) + 1));
	strcpy(node->key, k);

	node->value = v;

	if(prev)
		prev->next = node;
	else
		htab->nodes[hash] = node;	/* root node */

	node->next = NULL;

	return 0;
}

int htab_find(tvm_htab_t* htab, const char* key)
{
	int hash = htab_hash(key);
	tvm_htable_node_t *node = htab->nodes[hash];	

	while(node)
	{
		if(!strcmp(node->key, key))
			return node->value;
		node = node->next;
	}

	return -1;
}

unsigned int htab_hash(const char* k)
{
	unsigned int hash = 1;

	char* c; for(c = (char*)k; *c; c++)
		hash += (hash << *c) - *c;

	return hash % HTAB_SIZE;
}
