#include <stdlib.h>
#include <string.h>

#include <tvm/tvm_htab.h>

#define HTAB_LOAD_FACTOR 0.7

tvm_htab_t *htab_create()
{
	tvm_htab_t *htab = (tvm_htab_t *)calloc(1, sizeof(tvm_htab_t));
	htab->size = HTAB_SIZE;
	htab->nodes = (tvm_htab_node_t **)calloc(htab->size, sizeof(tvm_htab_node_t *));
	htab->num_nodes = 0;
	return htab;
}

void htab_destroy(tvm_htab_t *htab)
{
	int i;
	tvm_htab_node_t *node, *next;

	for(i = 0; i < htab->size; i++)
	{
		node = htab->nodes[i];
		while(node)
		{
			next = node->next;
			if(node->valptr)
				free(node->valptr);
			free(node->key);
			free(node);
			node = next;
		}
	}

	free(htab->nodes);
	free(htab);
}

static inline unsigned htab_hash(const char *k, const unsigned int size)
{
	unsigned int hash = 1;

	char *c; for(c = (char*)k; *c; c++)
		hash += (hash << *c) - *c;

	return hash % size;
}

static void htab_rehash(tvm_htab_t *orig, unsigned int size)
{
	int i;
	tvm_htab_node_t *node, *next;
	tvm_htab_t *new;

	new = (tvm_htab_t *)calloc(1, sizeof(tvm_htab_t));
	new->nodes = (tvm_htab_node_t **)calloc(size, sizeof(tvm_htab_node_t *));
	new->size = size;
	new->num_nodes = 0;

	/* Traverse the original hash table, rehashing
	   every entry into the new table and deleting
	   original entries */
	for(i = 0; i < orig->size; i++)
	{
		node = orig->nodes[i];
		while(node)
		{
			next = node->next;
			if (node->valptr)
			{
				htab_add_ref(new, node->key, node->valptr, strlen(node->valptr) + 1);
				free(node->valptr);
			}
			else
				htab_add(new, node->key, node->value);
			free(node->key);
			free(node);
			node = next;
		}
	}

	free(orig->nodes);

	/* Transpose the new hash table onto the old one */
	memcpy(orig, new, sizeof(tvm_htab_t));
	free(new);
}

static tvm_htab_node_t *htab_add_core(tvm_htab_t *htab, const char *k)
{
	/* Increase bucket count and rehash if the
	   load factor is too high */
	if((float)++htab->num_nodes / htab->size > HTAB_LOAD_FACTOR)
		htab_rehash(htab, htab->num_nodes * 2);

	int hash = htab_hash(k, htab->size);
	tvm_htab_node_t *node = htab->nodes[hash];
	tvm_htab_node_t *prev = NULL;

	if(node)
	{
		while(node->next)
			node = node->next;

		prev = node;
		node = node->next;
	}

	/* Allocate space, and copy the key/value pair. */

	node = calloc(1, sizeof(tvm_htab_node_t));

	node->key = (char *)calloc((strlen(k) + 1), sizeof(char));
	strcpy(node->key, k);

	if(prev) prev->next = node;
	else htab->nodes[hash] = node;	/* root node */

	node->next = NULL;

	return node;
}

int htab_add(tvm_htab_t *htab, const char *key, int value)
{
	tvm_htab_node_t *node = htab_add_core(htab, key);

	if (!node)
		return -1;

	node->value = value;

	return 0;
}

int htab_add_ref(tvm_htab_t *htab, const char *key, const void *valptr, int len)
{
	tvm_htab_node_t *node = htab_add_core(htab, key);

	if (!node)
		return -1;

	node->valptr = calloc(len, sizeof(char));
	memcpy(node->valptr, valptr, len);

	return 0;
}

static tvm_htab_node_t *htab_find_core(tvm_htab_t *htab, const char *key)
{
	int hash = htab_hash(key, htab->size);
	tvm_htab_node_t *node = htab->nodes[hash];

	while (node)
	{
		if(!strcmp(node->key, key))
			return node;
		else
			node = node->next;
	}

	return NULL;
}

int htab_find(tvm_htab_t *htab, const char *key)
{
	tvm_htab_node_t *node = htab_find_core(htab, key);
	
	if(!node)
		return -1;

	return node->value;
}

char *htab_find_ref(tvm_htab_t *htab, const char *key)
{
	tvm_htab_node_t *node = htab_find_core(htab, key);

	if (!node)
		return NULL;

	return node->valptr;
}
