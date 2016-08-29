#ifndef TVM_HTAB_H_
#define TVM_HTAB_H_

#define KEY_LENGTH 64
#define HTAB_SIZE 4096

struct tvm_htab_node {
	char *key;
	int value;
	void *valptr;
	struct tvm_htab_node *next;
};

struct tvm_htab_ctx {
	unsigned int num_nodes;
	unsigned int size;
	struct tvm_htab_node **nodes;
};

struct tvm_htab_ctx *tvm_htab_create();
void tvm_htab_destroy(struct tvm_htab_ctx *htab);

int tvm_htab_add(struct tvm_htab_ctx *htab, const char *key, int value);
int tvm_htab_add_ref(struct tvm_htab_ctx *htab,
	const char *key, const void *valptr, int len);
int tvm_htab_find(struct tvm_htab_ctx *htab, const char *key);
char *tvm_htab_find_ref(struct tvm_htab_ctx *htab, const char *key);

#endif
