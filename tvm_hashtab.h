#ifndef TVM_HASHTAB_H_
#define TVM_HASHTAB_H_

#define KEY_LENGTH 64
#define HTAB_SIZE 4096

typedef struct
{
	char* key;
	int value;
} htable_node;

typedef struct
{
	int num_nodes;
	htable_node* nodes[HTAB_SIZE];
} hash_table;

hash_table* create_htab();
void destroy_htab(hash_table* htab);

inline int htab_add(hash_table* htab, char* key, int value);
inline int htab_find(hash_table* htab, char* key);

inline unsigned int htab_hash(char* key);

#endif
