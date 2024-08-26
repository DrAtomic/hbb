
#define HBB_TREE_IMPLEMENTATION
#include "hbb_tree.h"

int compare_int(void *a, void *b)
{
	return *(int *)a < *(int*)b ? -1 : *(int*)a > *(int*)b;
}

void print_tabs(size_t num_of_tabs)
{
	for (size_t i = 0; i < num_of_tabs; i++)
		printf("\t");
}

// validate tree
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
	hbb_tree_node *t = {0};

	int x = 15;
	int y = 11;
	int z = 24;
	int a = 5;
	int b = 19;
	int c = 25;
	int d = 6;
	int k = 1;

	hbb_tree_insert(&t, &x, sizeof(int), compare_int);
	hbb_tree_insert(&t, &y, sizeof(int), compare_int);
	hbb_tree_insert(&t, &z, sizeof(int), compare_int);
	hbb_tree_insert(&t, &a, sizeof(int), compare_int);
	hbb_tree_insert(&t, &b, sizeof(int), compare_int);
	hbb_tree_insert(&t, &c, sizeof(int), compare_int);
	hbb_tree_insert(&t, &d, sizeof(int), compare_int);

	if (hbb_tree_find_number(t, &x, compare_int))
		printf("did find number!\n");
	if (hbb_tree_find_number(t, &d, compare_int))
		printf("did find number!\n");
	if (hbb_tree_find_number(t, &c, compare_int))
		printf("did find number!\n");
	if (hbb_tree_find_number(t, &k, compare_int))
		printf("did find number!\n");
	hbb_tree_print(t, 0);
	hbb_tree_free(t);
	return 0;
}
