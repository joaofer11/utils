#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "vector.h"

#define V_AT(v, idx) ((void*)((uint8_t*)(v)->data + (idx) * (v)->esize))

static int v_grow(struct vector* v, size_t extra_space)
{
        void* newdata;
        size_t newcp;

        assert(v != NULL && extra_space >= 1);

        if (v->cap == 0) {
                newcp = extra_space;
        } else {
                newcp = v->cap < 4096/v->esize
                        ? v->cap + v->cap
                        : v->cap + v->cap/2;

                newcp += extra_space;
        }

        newdata = v->data == NULL
                ? malloc(newcp * v->esize)
                : realloc(v->data, newcp * v->esize);
        
        if (newdata == NULL)
                return -1;

        v->data = newdata;
        v->cap = newcp;

        return 0;
}

void* v_front(struct vector* v)
{
        if (v->cnt > 0)
                return v->data;

        return NULL;
}

void* v_back(struct vector* v)
{
        if (v->cnt > 0)
                return V_AT(v, v->cnt-1);

        return NULL;
}

void* v_at(struct vector* v, size_t idx)
{
        if (idx < v->cnt)
                return V_AT(v, idx);

        return NULL;
}

int v_delete_at(struct vector* v, size_t idx)
{
        if (idx >= v->cnt) 
                return -1;

        v->cnt -= 1;

        memmove(V_AT(v, idx), V_AT(v, idx+1), v->cnt * v->esize);

        return 0;
}

int v_delete(struct vector* v)
{
        if (v->cnt < 1)
                return -1;

        v->cnt -= 1;

        return 0;
}

int v_swap(struct vector* v, size_t to, size_t from)
{
        if (from < v->cnt && to < v->cnt && from != to) {
                uint8_t buf[v->esize];

                memmove(buf, V_AT(v, to), v->esize);
                memmove(V_AT(v, to), V_AT(v, from), v->esize);
                memmove(V_AT(v, from), buf, v->esize);

                return 0;
        }

        return -1;
}

int v_add_at(struct vector* v, void* data, size_t n_elements, size_t idx)
{
        size_t newcnt = v->cnt + n_elements;

        assert(v != NULL && n_elements >= 1);

        if (newcnt > v->cap
         && v_grow(v, n_elements) == -1)
                return -1;

        memmove(V_AT(v, idx + n_elements), V_AT(v, idx), v->esize * (v->cnt - idx));
        memcpy(V_AT(v, idx), data, n_elements * v->esize);

        v->cnt = newcnt;
        return 0;
}

int v_add(struct vector* v, void* data, size_t n_elements)
{
        size_t newcount = v->cnt + n_elements;

        assert(v != NULL && n_elements >= 1);

        if (newcount > v->cap
         && v_grow(v, n_elements) == -1)
                return -1;

        memcpy(V_AT(v, v->cnt), data, n_elements * v->esize); 

        v->cnt = newcount;

        return 0;
}
