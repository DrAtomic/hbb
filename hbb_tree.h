#ifndef HBB_TREE_H
#define HBB_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*compare_func)(void *, void *);

typedef struct hbb_tree_node {
	void *el;
	struct hbb_tree_node *left;
	struct hbb_tree_node *right;
} hbb_tree_node;

typedef struct hbb_tree_traverser {
	hbb_tree_node *root;
	compare_func compare;
	size_t el_size;
} hbb_tree_traverser;

hbb_tree_traverser *hbb_tree_create(compare_func compare, size_t el_size);
void hbb_tree_insert(hbb_tree_traverser *t, void *el);
void *hbb_tree_find(hbb_tree_traverser *t, void *el);
void hbb_tree_delete(hbb_tree_traverser *t, void *el);
void hbb_tree_free(hbb_tree_traverser *t);

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

hbb_tree_traverser *hbb_tree_create(compare_func compare, size_t el_size)
{
	hbb_tree_traverser *t = malloc(sizeof(hbb_tree_traverser));
	t->compare = compare;
	t->el_size = el_size;
	t->root = NULL;
	return t;
}

void hbb_tree_delete(hbb_tree_traverser *t, void *el)
{
	hbb_tree_node *parent = NULL;
	hbb_tree_node *cur = t->root;
	while (cur && t->compare(el, cur->el) != 0) {
		parent = cur;
		if (t->compare(el, cur->el) < 0)
			cur = cur->left;
		else
			cur = cur->right;
	}

	if (cur == NULL)
		return;

	// 0 or 1 child case
	if (cur->left == NULL || cur->right == NULL) {
		hbb_tree_node *new_child = NULL;
		if (cur->left == NULL)
			new_child = cur->right;
		else
			new_child = cur->left;

		if (parent == NULL)
			return;

		if (cur == parent->left)
			parent->left = new_child;
		else
			parent->right = new_child;
		hbb_tree_free_node(cur);
	} else {
		hbb_tree_node *parent = NULL;
		hbb_tree_node *del = cur->right;
		while(del->left) {
			parent = del;
			del = del->left;
		}
		if (parent)
			parent->left = del->right;
		else
			cur->right = del->right;
		memcpy(cur->el, del->el, t->el_size);
		hbb_tree_free_node(del);
	}
}

void *hbb_tree_find(hbb_tree_traverser *t, void *el)
{
	hbb_tree_node **cur = &t->root;

	while (*cur) {
		if (t->compare(el, (*cur)->el) < 0)
			cur = &(*cur)->left;
		else if (t->compare(el, (*cur)->el) > 0)
			cur = &(*cur)->right;
		else {
			return (*cur)->el;
		}
	}
	return NULL;
}

void hbb_tree_insert(hbb_tree_traverser *t, void *el)
{
	hbb_tree_node **cur = &t->root;
	while (*cur) {
		if (t->compare(el, (*cur)->el) < 0)
			cur = &(*cur)->left;
		else if (t->compare(el, (*cur)->el) > 0)
			cur = &(*cur)->right;
		else
			return;
	}

	hbb_tree_node *node = create_tree_node(el, t->el_size);
	*cur = node;
}

void hbb_tree_free(hbb_tree_traverser *t)
{
	hbb_tree_node *prev;
	hbb_tree_node *cur = t->root;

	while (t->root) {
		while(cur->left)
			cur = cur->left;

		cur->left = t->root->right;
		prev = t->root;
		t->root = t->root->left;

		hbb_tree_free_node(prev);
	}
	free(t);
}

#endif /* HBB_TREE_IMPLEMENTATION */
#endif /* HBB_TREE_H */
