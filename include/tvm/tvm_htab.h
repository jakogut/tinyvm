#ifndef TVM_HTAB_H_
#define TVM_HTAB_H_

#define KEY_LENGTH 64
#define HTAB_SIZE 4096

typedef struct tvm_htab_node_s
{
	char *key;
	int value;
	void *valptr;
	struct tvm_htab_node_s *next;
} tvm_htab_node_t;

typedef struct tvm_htab_s
{
	unsigned int num_nodes;
	unsigned int size;
	tvm_htab_node_t **nodes;
} tvm_htab_t;

tvm_htab_t* htab_create();
void htab_destroy(tvm_htab_t *htab);

int htab_add(tvm_htab_t *htab, const char *key, int value);
int htab_add_str(tvm_htab_t *htab, const char *key, const void *valptr, int len);
int htab_find(tvm_htab_t *htab, const char *key);

#endif
