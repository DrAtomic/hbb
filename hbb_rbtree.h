#ifndef HBB_RBTREE_H
#define HBB_RBTREE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum {
	HBB_BLACK = 0,
	HBB_RED,
} hbb_node_color;

typedef struct hbb_rbtree_node {
	void *el;
	hbb_node_color color;
	struct hbb_rbtree_node *parent;
	struct hbb_rbtree_node *left;
	struct hbb_rbtree_node *right;
} hbb_rbtree_node;


#ifdef HBB_RBTREE_IMPLEMENTATION

static hbb_rbtree_node *create_rbtree_node(void *el, size_t el_size)
{
	hbb_rbtree_node *node = malloc(sizeof(hbb_rbtree_node));

	node->el = malloc(el_size);
	if (!node->el) {
		fprintf(stderr, "%s: %d failed to malloc\n", __func__, __LINE__);
		exit(1);
	}
	memcpy(node->el, el, el_size);
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	node->color = HBB_BLACK;

	return node;
}

static void hbb_rbtree_free_node(hbb_rbtree_node *n)
{
	free(n->el);
	free(n);
}

void hbb_rbtree_left_rotate(hbb_rbtree_node **t, hbb_rbtree_node *x)
{
	hbb_rbtree_node *y = x->right;
	x->right = y->left;

	if (y->left)
		y->left->parent = x;

	y->parent = x->parent;

	if (x->parent == NULL)
		(*t) = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	y->left = x;
	x->parent = y;

}

void hbb_rbtree_right_rotate(hbb_rbtree_node **t, hbb_rbtree_node *x)
{
	hbb_rbtree_node *y = x->left;
	x->left = y->right;

	if (y->right)
		y->right->parent = x;

	y->parent = x->parent;

	if (x->parent == NULL)
		(*t) = y;
	else if (x == x->parent->right)
		x->parent->right = y;
	else
		x->parent->left = y;
	y->right = x;
	x->parent = y;
}

void hbb_rbtree_insert_fixup(hbb_rbtree_node **t, hbb_rbtree_node *z)
{
	while (z != *t && z != (*t)->left && z != (*t)->right && z->parent->color == HBB_RED) {
		if (z->parent && z->parent->parent && z->parent == z->parent->parent->left) {
			hbb_rbtree_node *y = z->parent->parent->right;
			if (y->color == HBB_RED) {
				z->parent->color = HBB_BLACK;
				y->color = HBB_BLACK;
				z->parent->parent->color = HBB_RED;
				z = z->parent->parent;
			} else {
				if (z == z->parent->right) {
					z = z->parent;
					hbb_rbtree_left_rotate(t, z);
				}
				z->parent->color = HBB_BLACK;
				z->parent->parent->color = HBB_RED;
				hbb_rbtree_right_rotate(t, z->parent->parent);
			}
		} else {
			hbb_rbtree_node *y = z->parent->parent->left;
			if (y->color == HBB_RED) {
				z->parent->color = HBB_BLACK;
				y->color = HBB_BLACK;
				z->parent->parent->color = HBB_RED;
				z = z->parent->parent;
			} else {
				if (z == z->parent->left) {
					z = z->parent;
					hbb_rbtree_right_rotate(t, z);
				}
				z->parent->color = HBB_BLACK;
				z->parent->parent->color = HBB_RED;
				hbb_rbtree_left_rotate(t, z->parent->parent);
			}
		}
	}
	(*t)->color = HBB_BLACK;
}

void hbb_rbtree_insert(hbb_rbtree_node **t, void *el, size_t el_size, int (*compare_func)(void *, void *))
{
	hbb_rbtree_node *z = create_rbtree_node(el, el_size);
	hbb_rbtree_node *x = *t;
	hbb_rbtree_node *y = NULL;

	while (x) {
		y = x;
		if (compare_func(z->el, x->el) < 0)
			x = x->left;
		else
			x = x->right;
	}
	z->parent = y;

	if (y == NULL)
		*t = z;
	else if (compare_func(z->el, y->el) < 0)
		y->left = z;
	else
		y->right = z;
	z->left = NULL;
	z->right = NULL;
	z->color = HBB_RED;

	hbb_rbtree_insert_fixup(t, z);
}

void hbb_rbtree_free(hbb_rbtree_node *t)
{
	hbb_rbtree_node *prev;
	hbb_rbtree_node *cur = t;

	while (t) {
		while(cur->left)
			cur = cur->left;

		cur->left = t->right;
		prev = t;
		t = t->left;

		hbb_rbtree_free_node(prev);
	}
}

#endif /* HBB_RBTREE_IMPLEMENTATION */
#endif /* HBB_RBTREE_H */
