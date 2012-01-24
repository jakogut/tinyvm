#ifndef TVM_HASHTAB_H_
#define TVM_HASHTAB_H_

#define KEY_LENGTH 64
#define HTAB_SIZE 4096

typedef struct tvm_htable_node_s
{
	char* key;
	int value;
	struct tvm_htable_node_s* next;
} tvm_htable_node_t;

typedef struct tvm_htab_s
{
	unsigned int num_nodes;
	unsigned int size;
	tvm_htable_node_t** nodes;
} tvm_htab_t;

tvm_htab_t* create_htab();
void destroy_htab(tvm_htab_t* htab);

int htab_add(tvm_htab_t* htab, const char* key, int value);
int htab_find(tvm_htab_t* htab, const char* key);

unsigned int htab_hash(const char* key, const unsigned int size);

#endif
