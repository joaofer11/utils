#ifndef VECTOR_H_
#define VECTOR_H_

#include <stddef.h>

/* This struct represents a dynamic array. */
struct vector {
        void *data;   /* Elements of this vector. */
        size_t cnt;   /* How many elements are stored in this vector. */
        size_t cap;   /* How many elements this vector can store. */
        size_t esize; /* How many bytes each element takes. */
};

#define V_INIT(esize) {NULL, 0, 0, (esize)}

int vec_swap(struct vector* v, size_t to, size_t from);

void *v_at(struct vector *v, size_t idx);

void *v_back(struct vector *v);

void *v_front(struct vector *v);

int v_delete_at(struct vector *v, size_t idx);

int v_delete(struct vector *v);

int v_add_at(struct vector *v, void *data, size_t cnt, size_t idx);

int v_add(struct vector *v, void *data, size_t cnt);

#endif /* VECTOR_H_ */
