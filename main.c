#define HBB_QUEUE_IMPLEMENTATION
#include "hbb_queue.h"

#define HBB_ARENA_IMPLEMENTATION
#include "hbb_arena.h"

#define HBB_LIST_IMPLEMENTATION
#include "hbb_list.h"

#define HBB_TREE_IMPLEMENTATION
#include "hbb_tree.h"

#define HBB_RBTREE_IMPLEMENTATION
#include "hbb_rbtree.h"


#include <stdio.h>

hbb_node *create_persistant_list()
{
	hbb_node *l = {0};

	int arr[] = {1, 3, 4};
	int b = 2;

	for (size_t i = 0; i < 3; i++)
		hbb_list_push(&l, &arr[i], sizeof(int));

	hbb_list_append(&l, &b, sizeof(int));

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

void hbb_rbtree_print(hbb_rbtree_node *t, size_t level)
{
	if (t == NULL) {
		print_tabs(level);
		printf("---empty---\n");
		return;
	}
	print_tabs(level);
	printf("value = %d\n", *(int*)t->el);
	print_tabs(level);
	printf("color = %s\n", t->color ? "RED" : "BLACK");
	print_tabs(level);
	printf("left\n");

	hbb_rbtree_print(t->left, level+1);
	print_tabs(level);
	printf("right\n");

	hbb_rbtree_print(t->right, level+1);
	print_tabs(level);
	printf("done\n");
}


#define NUM_ELMS(x) (sizeof(x) / sizeof(x[0]))



int main(void)
{
	// hbb_node *l = create_persistant_list();

	// hbb_list_print(l, print_int);

	// hbb_node *thing = hbb_list_pop(&l);
	// printf("\n");
	// printf("thing %d\n", *(int*)thing->el);

	// hbb_list_print(l, print_int);

	// int x = 1;
	// int y = 3;
	// hbb_list_remove(&l, &y, int);
	// hbb_list_remove(&l, &x, int);

	// printf("\n");
	// hbb_list_print(l, print_int);
	// hbb_free_node(thing);
	// hbb_list_free(l);

	// tree testing
	hbb_tree_traverser *t = hbb_tree_create(compare_int, sizeof(int));

	int arr[] = {12, 18, 5, 2 ,9, 15, 19, 17};

	for (size_t i = 0; i < NUM_ELMS(arr); i++)
		hbb_tree_insert(t, &arr[i]);

	hbb_tree_delete(t, &arr[1]);
	if (hbb_tree_find_number(t, &arr[0]))
		printf("did find number!\n");
	// if (hbb_tree_find_number(t, &d))
	// 	printf("did find number!\n");
	// if (hbb_tree_find_number(t, &c))
	// 	printf("did find number!\n");
	// if (hbb_tree_find_number(t, &h))
	// 	printf("did find number!\n");
	hbb_tree_print(t->root, 0);
	hbb_tree_free(t);

	// int arr[] = {12, 8, 15, 5, 9, 13, 19, 8, 11};
	// hbb_rbtree_node *rbt = {0};
	// for (size_t i = 0; i < NUM_ELMS(arr); i++)
	// 	hbb_rbtree_insert(&rbt, &arr[i], sizeof(int), compare_int);

	// hbb_rbtree_print(rbt, 0);
	// hbb_rbtree_free(rbt);
	return 0;
}
