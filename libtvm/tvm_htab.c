#include <stdlib.h>
#include <string.h>

#include <tvm/tvm_htab.h>

#define HTAB_LOAD_FACTOR 0.7

struct tvm_htab_ctx  *tvm_htab_create()
{
	struct tvm_htab_ctx *htab =
		(struct tvm_htab_ctx *)calloc(1, sizeof(struct tvm_htab_ctx));

	htab->size = HTAB_SIZE;
	htab->nodes = (struct tvm_htab_node **)calloc(
		htab->size, sizeof(struct tvm_htab_node *));
	htab->num_nodes = 0;
	return htab;
}

void tvm_htab_destroy(struct tvm_htab_ctx *htab)
{
	struct tvm_htab_node *node, *next;

	for (int i = 0; i < htab->size; i++) {
		node = htab->nodes[i];
		while (node) {
			next = node->next;
			if (node->valptr)
				free(node->valptr);
			free(node->key);
			free(node);
			node = next;
		}
	}

	free(htab->nodes);
	free(htab);
}

static inline unsigned int htab_hash(const char *k, const unsigned int size)
{
	unsigned int hash = 1;

	char *c; for (c = (char *)k; *c; c++)
		hash += (hash << *c) - *c;

	return hash % size;
}

static void htab_rehash(struct tvm_htab_ctx  *orig, unsigned int size)
{
	int i;
	struct tvm_htab_node *node, *next;
	struct tvm_htab_ctx  *new;

	new = (struct tvm_htab_ctx  *)calloc(1, sizeof(struct tvm_htab_ctx));
	new->nodes = (struct tvm_htab_node **)calloc(
		size, sizeof(struct tvm_htab_node *));
	new->size = size;
	new->num_nodes = 0;

	/* Traverse the original hash table, rehashing
	 * every entry into the new table and deleting
	 * original entries
	 */
	for (i = 0; i < orig->size; i++) {
		node = orig->nodes[i];
		while (node) {
			next = node->next;
			if (node->valptr) {
				tvm_htab_add_ref(
					new, node->key,
					node->valptr,
					strlen(node->valptr) + 1
				);

				free(node->valptr);

			} else
				tvm_htab_add(new, node->key, node->value);

			free(node->key);
			free(node);
			node = next;
		}
	}

	free(orig->nodes);

	/* Transpose the new hash table onto the old one */
	memcpy(orig, new, sizeof(struct tvm_htab_ctx));
	free(new);
}

static struct tvm_htab_node *htab_add_core(
	struct tvm_htab_ctx *htab, const char *k)
{
	/* Increase bucket count and rehash if the
	 * load factor is too high
	 */
	if ((float) ++htab->num_nodes / htab->size > HTAB_LOAD_FACTOR)
		htab_rehash(htab, htab->num_nodes * 2);

	int hash = htab_hash(k, htab->size);
	struct tvm_htab_node *node = htab->nodes[hash];
	struct tvm_htab_node *prev = NULL;

	if (node) {
		while (node->next)
			node = node->next;

		prev = node;
	}

	/* Allocate space, and copy the key/value pair. */

	node = calloc(1, sizeof(struct tvm_htab_node));

	node->key = (char *)calloc((strlen(k) + 1), sizeof(char));
	strcpy(node->key, k);

	if (prev)
		prev->next = node;
	else
		htab->nodes[hash] = node;	/* root node */

	node->next = NULL;

	return node;
}

int tvm_htab_add(struct tvm_htab_ctx *htab, const char *key, int value)
{
	struct tvm_htab_node *node = htab_add_core(htab, key);

	if (!node)
		return -1;

	node->value = value;

	return 0;
}

int tvm_htab_add_ref(
	struct tvm_htab_ctx *htab, const char *key, const void *valptr, int len)
{
	struct tvm_htab_node *node = htab_add_core(htab, key);

	if (!node)
		return -1;

	node->valptr = calloc(len, sizeof(char));
	memcpy(node->valptr, valptr, len);

	return 0;
}

static struct tvm_htab_node *htab_find_core(
	struct tvm_htab_ctx *htab, const char *key)
{
	int hash = htab_hash(key, htab->size);
	struct tvm_htab_node *node = htab->nodes[hash];

	while (node) {
		if (!strcmp(node->key, key))
			return node;

		node = node->next;
	}

	return NULL;
}

int tvm_htab_find(struct tvm_htab_ctx  *htab, const char *key)
{
	struct tvm_htab_node *node = htab_find_core(htab, key);

	if (!node)
		return -1;

	return node->value;
}

char *tvm_htab_find_ref(struct tvm_htab_ctx  *htab, const char *key)
{
	struct tvm_htab_node *node = htab_find_core(htab, key);

	if (!node)
		return NULL;

	return node->valptr;
}
