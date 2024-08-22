#ifndef HBB_ARENA_H
#define HBB_ARENA_H

#include <stdint.h>
#include <stdlib.h>


typedef struct {
	size_t capacity;
	uint8_t *memory;
	size_t used;
} hbb_memory;

void hbb_arena_init(void);
void hbb_arena_clear(void);
void hbb_arena_push(void **data, size_t size);

#ifdef HBB_ARENA_IMPLEMENTATION

hbb_memory back_buffer;

void hbb_arena_init(void)
{
	back_buffer.memory = calloc(1, 256);
	back_buffer.capacity = 256;
	back_buffer.used = 0;
}

void hbb_arena_clear(void)
{
	back_buffer.capacity = 0;
	back_buffer.used = 0;
	free(back_buffer.memory);
}

void hbb_arena_push(void **data, size_t size)
{
	*data = back_buffer.memory + back_buffer.used;
	back_buffer.used += size;
}

#endif /* HBB_ARENA_IMPLEMENTATION */
#endif /* HBB_ARENA_H */
