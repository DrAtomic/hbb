#define HBB_QUEUE_IMPLEMENTATION
#include "hbb_queue.h"

#define HBB_ARENA_IMPLEMENTATION
#include "hbb_arena.h"

#define HBB_LIST_IMPLEMENTATION
#include "hbb_list.h"

#define HBB_TREE_IMPLEMENTATION
#define HBB_TREE_RECURSIVE
#include "hbb_tree.h"

#include <stdio.h>

hbb_node *create_persistant_list()
{
	hbb_node *l = {0};

	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	hbb_list_push(&l, &c, sizeof(int));
	hbb_list_append(&l, &b, sizeof(int));
	hbb_list_push(&l, &d, sizeof(int));
	hbb_list_push(&l, &c, sizeof(int));
	hbb_list_push(&l, &c, sizeof(int));
	hbb_list_push(&l, &c, sizeof(int));
	hbb_list_push(&l, &a, sizeof(int));
	hbb_list_push(&l, &a, sizeof(int));

	return l;
}

void print_int(void *el)
{
	printf("%d\n", *(int *)el);
}

int compare_int(void *a, void *b)
{
	return *(int *)a < *(int*)b ? -1 : *(int*)a > *(int*)b;
}

void print_tabs(size_t num_of_tabs)
{
	for (size_t i = 0; i < num_of_tabs; i++)
		printf("\t");
}

void hbb_tree_print(hbb_tree_node *t, size_t level)
{
	if (t == NULL) {
		print_tabs(level);
		printf("---empty---\n");
		return;
	}
	print_tabs(level);
	printf("value = %d\n", *(int*)t->el);
	print_tabs(level);
	printf("left\n");

	hbb_tree_print(t->left, level+1);
	print_tabs(level);
	printf("right\n");

	hbb_tree_print(t->right, level+1);
	print_tabs(level);
	printf("done\n");
}


int main(void)
{
	hbb_node *l = create_persistant_list();

	hbb_list_print(l, print_int);

	hbb_node *thing = hbb_list_pop(&l);
	printf("\n");
	printf("thing %d\n", *(int*)thing->el);

	hbb_list_print(l, print_int);

	int x = 1;
	int y = 3;
	hbb_list_remove(&l, &y, int);
	hbb_list_remove(&l, &x, int);

	printf("\n");
	hbb_list_print(l, print_int);
	hbb_free_node(thing);
	hbb_list_free(l);

	// tree testing
		hbb_tree_node *t = {0};

	int a = 15;
	int b = 11;
	int c = 24;
	int d = 5;
	int e = 19;
	int f = 25;
	int g = 6;
	int h = 1;

	hbb_tree_insert(&t, &a, sizeof(int), compare_int);
	hbb_tree_insert(&t, &b, sizeof(int), compare_int);
	hbb_tree_insert(&t, &c, sizeof(int), compare_int);
	hbb_tree_insert(&t, &d, sizeof(int), compare_int);
	hbb_tree_insert(&t, &e, sizeof(int), compare_int);
	hbb_tree_insert(&t, &f, sizeof(int), compare_int);
	hbb_tree_insert(&t, &g, sizeof(int), compare_int);

	if (hbb_tree_find_number(t, &a, compare_int))
		printf("did find number!\n");
	if (hbb_tree_find_number(t, &d, compare_int))
		printf("did find number!\n");
	if (hbb_tree_find_number(t, &c, compare_int))
		printf("did find number!\n");
	if (hbb_tree_find_number(t, &h, compare_int))
		printf("did find number!\n");
	hbb_tree_print(t, 0);
	hbb_tree_free(t);
	return 0;
}
