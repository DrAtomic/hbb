#define HBB_QUEUE_IMPLEMENTATION
#include "hbb_queue.h"

#define HBB_ARENA_IMPLEMENTATION
#include "hbb_arena.h"

#define HBB_LIST_IMPLEMENTATION
#include "hbb_list.h"

#include <stdio.h>

hbb_node *create_persistant_list()
{
	hbb_node *l = {0};

	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	hbb_list_append(&l, &a, sizeof(int));
	hbb_list_append(&l, &b, sizeof(int));
	hbb_list_push(&l, &c, sizeof(int));
	hbb_list_push(&l, &d, sizeof(int));
	hbb_list_push(&l, &c, sizeof(int));
	hbb_list_push(&l, &c, sizeof(int));
	hbb_list_push(&l, &c, sizeof(int));

	return l;
}

void print_int(void *el) {
	printf("%d\n", *(int *)el);
}

int main(void)
{
	hbb_node *l = create_persistant_list();

	hbb_list_print(l, print_int);
	hbb_list_free(l);
	return 0;
}
