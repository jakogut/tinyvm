#include <string.h>
#include <stdlib.h>

#include <tvm/tvm_tree.h>

tvm_tree_t *tvm_tree_create(tvm_tree_t *node, const char *str, const void *val, int len)
{
	int keylen = strlen(str);
	node = calloc(1, sizeof(tvm_tree_t) + len + keylen + 1);

	if(val && len)
	{
		node->val = (char *)node + sizeof(tvm_tree_t);
		memcpy(node->val, val, len);
	}

	node->keystr = (char *)node + sizeof(tvm_tree_t) + len;
	memcpy(node->keystr, str, keylen);

	return node;
}

int tvm_tree_add(tvm_tree_t *node, const char *str, const void *val, int len)
{
	if (!str || !node)
		return 1;

	int done = 0, diff;
	tvm_tree_t *current = node;
	while(!done)
	{
		diff = strcmp(current->keystr, str);
		if(!diff)
			done = 1;
		else if(diff < 0)
		{
			if(current->left)
			{
				current = current->left;
			}
			else
			{
				current->left = tvm_tree_create(NULL, str, val, len);
				done = 1;
			}
		}
		else if(diff > 0)
		{
			if(current->right)
			{
				current = current->right;
			}
			else
			{
				current->right = tvm_tree_create(NULL, str, val, len);
				done = 1;
			}
		}
	}

	/* String already exists in the tree. */
	if(!diff)
		return 2;

	return 0;
}

void *tvm_tree_find(tvm_tree_t *node, const char *str)
{
	if(!node || !str)
		return 0;

	int diff;
	void *value = NULL;
	tvm_tree_t *current = node;
	while(current && !value)
	{
		diff = strcmp(current->keystr, str);
		if(!diff)
			value = current->val;
		else if(diff < 0)
			current = current->left;
		else if(diff > 0)
			current = current->right;
	}

	return value;
}

void tvm_destroy(tvm_tree_t *node)
{
	if(node)
	{
		if (node->right)
			tvm_destroy(node->right);
		if (node->left)
			tvm_destroy(node->left);

		free(node);
	}
}