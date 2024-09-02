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
		fprintf(stderr, "%s: failed to malloc\n", __func__);
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

void transplant(hbb_tree_node **p, hbb_tree_node *t, int dir)
{
	if (dir < 0)
		(*p)->left = t;
	else
		(*p)->right = t;
}

void hbb_tree_delete(hbb_tree_traverser *t, void *el)
{
	hbb_tree_node **cur = &t->root;
	hbb_tree_node **parent = NULL;
	int comp = 0;
	int dir = 0;

	while (*cur) {
		comp = t->compare(el, (*cur)->el);
		if (comp < 0) {
			dir = comp;
			parent = cur;
			cur = &(*cur)->left;
		} else if (comp > 0) {
			dir = comp;
			parent = cur;
			cur = &(*cur)->right;
		} else {
			break;
		}
	}

	if (*cur == NULL)
		return;

	hbb_tree_node *del = *cur;

	if ((*cur)->left == NULL && (*cur)->right == NULL) {
		transplant(parent, NULL, dir);
	}

	else if ((*cur)->left == NULL && (*cur)->right != NULL) {
		transplant(parent, (*cur)->right, dir);
	}

	else if ((*cur)->left != NULL && (*cur)->right == NULL) {
		transplant(parent, (*cur)->left, dir);
	}

	else if ((*cur)->left != NULL && (*cur)->right != NULL) {
		hbb_tree_node *s = *cur;
		hbb_tree_node *r = (*cur)->right;

		if (r->left == NULL) {
			r->left = (*cur)->left;
			transplant(parent, r, dir);
		} else {
			s = s->right;
			while (s->left != NULL)
				s = s->left;

			s->left = (*cur)->left;
			r->left = s->right;
			s->right = r;

			transplant(parent, s, dir);
		}
	}

	hbb_tree_free_node(del);
}

void *hbb_tree_find(hbb_tree_traverser *t, void *el)
{
	hbb_tree_node **cur = &t->root;
	int comp = 0;

	while (*cur) {
		comp = t->compare(el, (*cur)->el);
		if (comp < 0)
			cur = &(*cur)->left;
		else if (comp > 0)
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
	int comp = 0;

	while (*cur) {
		comp = t->compare(el, (*cur)->el);
		if (comp < 0)
			cur = &(*cur)->left;
		else if (comp > 0)
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
