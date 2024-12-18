#ifndef VECTOR_H_
#define VECTOR_H_

#include <stddef.h>

struct vector {
	void*  items;
	size_t itemsize;
	size_t count;
	size_t capacity;
};

void* vec_get(struct vector*, size_t idx);

void* vec_front(struct vector *v);

void* vec_rear(struct vector*);

int vec_pop(struct vector *v);

int vec_insert(struct vector*, void* item, size_t idx);

int vec_prepend(struct vector*, void* item);

int vec_append(struct vector*, void* item);

void vec_destroy(struct vector*);

int vec_init(struct vector*, size_t itemsize);

#endif /* VECTOR_H_ */
