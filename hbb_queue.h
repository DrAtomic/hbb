#ifndef HBB_QUEUE_H
#define HBB_QUEUE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	unsigned ctr;
	void *data;
} hbb_queue_entry;

typedef struct {
	unsigned insertion_ctr;
	int alloc, count;
	hbb_queue_entry *array;
} hbb_queue;

void hbb_queue_put(hbb_queue *q, void *thing);
void *hbb_queue_get(hbb_queue *queue);
void hbb_clear_queue(hbb_queue *queue);

#ifdef HBB_QUEUE_IMPLEMENTATION

#define alloc_count(x) (((x)+16)*3/2)

#define hbb_append(item, count, alloc)							\
	do {										\
		if ((count) > alloc) {							\
			alloc = alloc_count(alloc);					\
			(item) = realloc((item), sizeof(*(item)) * (alloc));		\
			if ((item) == NULL) {						\
				fprintf(stderr, "%s: Failed: to realloc\n", __func__);	\
				exit(1);						\
			}								\
		}									\
	} while (0)

void hbb_queue_put(hbb_queue *q, void *thing)
{
	hbb_append(q->array, q->count + 1, q->alloc);
	q->array[q->count].ctr = q->insertion_ctr++;
	q->array[q->count].data = thing;
	q->count++;
}

void *hbb_queue_get(hbb_queue *queue)
{
	if (!queue->count)
		return NULL;
	return queue->array[--queue->count].data;
}

void hbb_queue_clear(hbb_queue *queue)
{
	free(queue->array);
	queue->count = 0;
	queue->alloc = 0;
}

#endif /* HBB_QUEUE_IMPLEMENTATION */
#endif /* HBB_QUEUE_H */
