#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "vector.h"

#define INITIAL_CAPACITY 2

#define item_at(vector, idx) ((uint8_t*)(v)->items + (idx) * (v)->itemsize)

static int vec_grow(struct vector* v, size_t requested_cap)
{
	void*  newptr;
	size_t mycap;

	assert(v != NULL && v->items != NULL && requested_cap > v->capacity);

	mycap = v->capacity * 2 + 1;
	if (requested_cap > mycap) {
		mycap = requested_cap;
	}

	if ((newptr = realloc(v->items, mycap * v->itemsize)) == NULL) {
		return -1;
	}

	v->items    = newptr;
	v->capacity = mycap;

	return 0;
}

static void vec_shrink(struct vector* v)
{
	assert(v != NULL && v->items != NULL);

	if (v->count <= (1/4) * v->capacity) {
		size_t half   = v->itemsize * (v->capacity/2);
		void*  newptr = realloc(v->items, half);

		if (newptr == NULL) {
			return;
		}

		v->items = newptr;
		v->capacity = half;
	}
}

void* vec_get(struct vector* v, size_t idx)
{
	if (idx >= v->count) {
		return NULL;
	}

	return item_at(v, idx);
}

void* vec_front(struct vector* v)
{
	return vec_get(v, 0);
}

void* vec_rear(struct vector* v)
{
	return vec_get(v, v->count-1);
}

int vec_pop(struct vector* v)
{
	if (v->count == 0) {
		return -1;
	}

	v->count -= 1;
	vec_shrink(v);

	return 0;
}

int vec_insert(struct vector* v, void* item, size_t idx)
{
	size_t newcnt;

	assert(v != NULL && v->items != NULL && item != NULL);

	if (idx <= v->count) {
		newcnt = v->count + 1;
	} else {
		newcnt = v->count + (idx - v->count + 1);
	}

	if (newcnt > v->capacity) {
		if (vec_grow(v, newcnt) == -1) {
			return -1;
		}
	}

	if (idx < v->count) {
		size_t nbytes = v->itemsize * (v->count - idx);
		memmove(item_at(v, idx+1), item_at(v, idx), nbytes);
	} else if (idx > v->count) {
		size_t gap = idx - v->count;
		memset(item_at(v, v->count), 0, gap * v->itemsize);
	}

	memcpy(item_at(v, idx), item, v->itemsize);
	v->count = newcnt;

	return 0;
}

int vec_prepend(struct vector* v, void* item)
{
	return vec_insert(v, item, 0);
}

int vec_append(struct vector* v, void* item)
{
	return vec_insert(v, item, v->count);
}

void vec_destroy(struct vector* v)
{
	assert(v != NULL);

	v->itemsize = 0;
	v->count    = 0;
	v->capacity = 0;

	if (v->items == NULL) {
		return;
	}

	free(v->items);
	v->items = NULL;
}

int vec_init(struct vector* v, size_t itemsize)
{
	assert(v != NULL && itemsize > 0);

	if ((v->items = malloc(INITIAL_CAPACITY * itemsize)) == NULL) {
		return -1;
	}

	v->itemsize = itemsize;
	v->count    = 0;
	v->capacity = INITIAL_CAPACITY;

	return 0;
}
