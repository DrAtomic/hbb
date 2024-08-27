#ifndef HBB_TREE_H
#define HBB_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct hbb_tree_node {
	void *el;
	struct hbb_tree_node *left;
	struct hbb_tree_node *right;
} hbb_tree_node;

void hbb_tree_insert(hbb_tree_node **t, void *el, size_t el_size, int (*compare_func)(void *, void *));
int hbb_tree_find_number(hbb_tree_node *t, void *el, int (*compare_func)(void *, void *));
void hbb_tree_delete(hbb_tree_node *t, void *el, size_t el_size, int (*compare_func)(void *, void *));
void hbb_tree_free(hbb_tree_node *t);

#ifdef HBB_TREE_IMPLEMENTATION

static hbb_tree_node *create_tree_node(void *el, size_t el_size)
{
	hbb_tree_node *node = malloc(sizeof(hbb_tree_node));

	node->el = malloc(el_size);
	if (!node->el) {
		fprintf(stderr, "%s: %d failed to malloc\n", __func__, __LINE__);
		exit(1);
	}
	memcpy(node->el, el, el_size);
	node->left = NULL;
	node->right = NULL;

	return node;
}

static void hbb_tree_free_node(hbb_tree_node *n)
{
	free(n->el);
	free(n);
}

void hbb_tree_delete(hbb_tree_node *t, void *el, size_t el_size, int (*compare_func)(void *, void *))
{
	hbb_tree_node *parent = NULL;
	while (t && compare_func(el, t->el) != 0) {
		parent = t;
		if (compare_func(el, t->el) < 0)
			t = t->left;
		else
			t = t->right;
	}

	if (t == NULL)
		return;

	// 0 or 1 child case
	if (t->left == NULL || t->right == NULL) {
		hbb_tree_node *new_child = NULL;
		if (t->left == NULL)
			new_child = t->right;
		else
			new_child = t->left;

		if (parent == NULL)
			return;

		if (t == parent->left)
			parent->left = new_child;
		else
			parent->right = new_child;
		hbb_tree_free_node(t);
	} else {
		hbb_tree_node *parent = NULL;
		hbb_tree_node *del = t->right;
		while(del->left) {
			parent = del;
			del = del->left;
		}
		if (parent)
			parent->left = del->right;
		else
			t->right = del->right;
		memcpy(t->el, del->el, el_size);
		hbb_tree_free_node(del);
	}
}

#ifdef HBB_TREE_RECURSIVE
void hbb_tree_insert(hbb_tree_node **t, void *el, size_t el_size, int (*compare_func)(void *, void *))
{
	hbb_tree_node *root = *t;

	if (root == NULL) {
		hbb_tree_node *node = create_tree_node(el, el_size);
		*t = node;
		return;
	}

	if (compare_func(el, (*t)->el) < 0) {
		hbb_tree_insert(&(root->left), el, el_size, compare_func);
		return;
	} else if (compare_func(el, (*t)->el) > 0) {
		hbb_tree_insert(&(root->right), el, el_size, compare_func);
		return;
	} else {
		return;
	}
}

int hbb_tree_find_number(hbb_tree_node *t, void *el, int (*compare_func)(void *, void *))
{
	if (t == NULL)
		return 0;
	if (compare_func(el, t->el) == 0)
		return 1;

	if (compare_func(el, t->el) < 0)
		return hbb_tree_find_number(t->left, el, compare_func);
	else
		return hbb_tree_find_number(t->right, el, compare_func);
}

void hbb_tree_free(hbb_tree_node *t)
{
	if (t == NULL)
		return;

	hbb_tree_free(t->left);
	hbb_tree_free(t->right);

	hbb_tree_free_node(t);
}

#else /* non recursive */

int hbb_tree_find_number(hbb_tree_node *t, void *el, int (*compare_func)(void *, void *))
{
	int found = 0;
	while (t) {
		if (compare_func(el, t->el) < 0)
			t = t->left;
		else if (compare_func(el, t->el) > 0)
			t = t->right;
		else {
			found = 1;
			break;
		}
	}

	return found;
}

void hbb_tree_insert(hbb_tree_node **t, void *el, size_t el_size, int (*compare_func)(void *, void *))
{
	while (*t) {
		if (compare_func(el, (*t)->el) < 0)
			t = &(*t)->left;
		else if (compare_func(el, (*t)->el) > 0)
			t = &(*t)->right;
		else
			return;
	}

	hbb_tree_node *node = create_tree_node(el, el_size);
	*t = node;
}

void hbb_tree_free(hbb_tree_node *t)
{
	hbb_tree_node *prev;
	hbb_tree_node *cur = t;

	while (t) {
		while(cur->left)
			cur = cur->left;

		cur->left = t->right;
		prev = t;
		t = t->left;

		hbb_tree_free_node(prev);
	}
}

#endif /* HBB_TREE_RECURSIVE */
#endif /* HBB_TREE_IMPLEMENTATION */
#endif /* HBB_TREE_H */
