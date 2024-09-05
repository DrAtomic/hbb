#ifndef HBB_LIST_H
#define HBB_LIST_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct hbb_node {
	struct hbb_node *next;
	void *el;
} hbb_node;

void hbb_list_push(hbb_node **l, void *el, size_t el_size);
void hbb_list_append(hbb_node **l, void *el, size_t el_size);
hbb_node *hbb_list_pop(hbb_node **l);
void hbb_list_print(hbb_node *l, void (*print_func)(void *));
void hbb_free_node(hbb_node *n);
void hbb_list_free(hbb_node *l);

#define hbb_list_remove(l, data, type)				\
do {								\
	hbb_node **cur = l, *del;				\
								\
	while (*cur && *(type *)(*cur)->el != *(type *)data)	\
		cur = &(*cur)->next;				\
								\
	if (*cur) {						\
		del = *cur;					\
		*cur = del->next;				\
		del->next = NULL;				\
		hbb_free_node(del);				\
	}							\
} while(0)

#ifdef HBB_LIST_IMPLEMENTATION

static hbb_node *create_node(void *el, size_t el_size)
{
	hbb_node *node = malloc(sizeof(hbb_node));

	if (!node) {
		fprintf(stderr, "%s: %d failed to malloc\n", __func__, __LINE__);
		exit(1);
	}

	node->el = malloc(el_size);
	if (!node->el) {
		fprintf(stderr, "%s: %d failed to malloc\n", __func__, __LINE__);
		exit(1);
	}
	memcpy(node->el, el, el_size);
	node->next = NULL;

	return node;
}

void hbb_list_push(hbb_node **l, void *el, size_t el_size)
{
	hbb_node *node = create_node(el, el_size);

	node->next = *l;
	*l = node;
}

void hbb_list_append(hbb_node **l, void *el, size_t el_size)
{
	hbb_node *node = create_node(el, el_size);

	while (*l)
		l = &(*l)->next;

	*l = node;
}

void hbb_list_print(hbb_node *l, void (*print_func)(void *))
{
	while (l) {
		print_func(l->el);
		l = l->next;
	}
}

hbb_node *hbb_list_pop(hbb_node **l)
{
	hbb_node *head = (*l);
	if (head) {
		*l = head->next;
		head->next = NULL;
	}
	return head;
}

void hbb_free_node(hbb_node *n)
{
	free(n->el);
	free(n);
}

void hbb_list_free(hbb_node *l)
{
	hbb_node *cur;

	while (l) {
		cur = l;
		l = l->next;
		hbb_free_node(cur);
	}
}

#endif /* HBB_LIST_IMPLENTATION */
#endif /* HBB_LIST_H */
