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
void hbb_list_print(hbb_node *l, void (*print_func)(void *));
void hbb_list_free(hbb_node *l);

#ifdef HBB_LIST_IMPLEMENTATION

static hbb_node *create_node(void *el, size_t el_size)
{
	hbb_node *node = malloc(sizeof(hbb_node));

	node->el = malloc(el_size);
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

	if (*l == NULL) {
		*l = node;
		return;
	}

	while ((*l)->next != NULL)
		(*l) = (*l)->next;
	(*l)->next = node;
}

void hbb_list_print(hbb_node *l, void (*print_func)(void *))
{
	while(l != NULL) {
		print_func(l->el);
		l = l->next;
	}
}

void hbb_list_free(hbb_node *l)
{
	hbb_node *cur;
	while (l != NULL) {
		cur = l;
		l = l->next;
		free(cur->el);
		free(cur);
	}
}

#endif /* HBB_LIST_IMPLENTATION */
#endif /* HBB_LIST_H */
