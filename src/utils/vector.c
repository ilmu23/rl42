// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<vector.c>>

#include <alloca.h>
#include <stdlib.h>
#include <string.h>

#include "internal/_vector.h"

struct __vec {
	void	(*free)(void *);
	size_t	element_size;
	size_t	capacity;
	size_t	elements;
	u8		*data;
};

#define index(vec, i)	((void *)((uintptr_t)vec->data + (vec->element_size * i)))

static inline void	_set_element(vector vec, const size_t i, const void *val);

static void	__free(void **blk) {
	free(*blk);
}

vector	__vec_new(const size_t size, const size_t count, void (*_free)(void *)) {
	vector	out;

	out = (size) ? malloc(sizeof(*out)) : NULL;
	if (out) {
		out->data = malloc(size * count);
		if (!out->data) {
			free(out);
			return NULL;
		}
		out->element_size = size;
		out->capacity = count;
		out->elements = 0;
		out->free = (_free != free) ? _free : (void (*)(void *))__free;
	}
	return out;
}

void	__vec_del(vector vec) {
	if (vec) {
		if (vec->elements)
			__vec_clr(vec);
		free(vec->data);
		free(vec);
	}
}

u8	__vec_psh(vector vec, const void *val) {
	if (vec->elements == vec->capacity && !__vec_rsz(vec, vec->capacity * 2))
		return 0;
	_set_element(vec, vec->elements++, val);
	return 1;
}

void	__vec_pop(vector vec) {
	if (vec->elements) {
		vec->elements--;
		if (vec->free)
			vec->free (index(vec, vec->elements));
	}
}

void	*__vec_get(cvector vec, const size_t i) {
	return (i < vec->elements) ? index(vec, i) : VECTOR_OUT_OF_BOUNDS;
}

u8	__vec_set(vector vec, const size_t i, const void *val) {
	if (i >= vec->elements)
		return 0;
	_set_element(vec, i, val);
	return 1;
}

u8	__vec_swp(vector vec, const size_t i1, const size_t i2, u8 (*swap)(void *, void *)) {
	u8	*tmp;

	if (i1 >= vec->elements || i2 >= vec->elements)
		return 0;
	if (i1 != i2) {
		if (swap)
			return swap(index(vec, i1), index(vec, i2));
		tmp = alloca(vec->element_size);
		memcpy(tmp, index(vec, i1), vec->element_size);
		memcpy(index(vec, i1), index(vec, i2), vec->element_size);
		memcpy(index(vec, i2), tmp, vec->element_size);
	}
	return 1;
}

size_t	__vec_sze(cvector vec) {
	return vec->elements;
}

size_t	__vec_cap(cvector vec) {
	return vec->capacity;
}

u8	__vec_rsz(vector vec, const size_t size) {
	if (size < vec->elements && vec->free) {
		do
			vec->free(index(vec, --vec->elements));
		while (size < vec->elements);
	}
	vec->capacity = size;
	vec->data = realloc(vec->data, vec->capacity * vec->element_size);
	return (vec->data) ? 1 : 0;
}

u8	__vec_stf(vector vec) {
	return __vec_rsz(vec, vec->elements);
}

void	__vec_clr(vector vec) {
	if (vec->free) {
		do
			vec->free(index(vec, --vec->elements));
		while (vec->elements);
	}
	vec->elements = 0;
}

u8	__vec_ins(vector vec, const size_t i, const void *val) {
	size_t	_i;

	if (i >= vec->elements)
		return __vec_psh(vec, val);
	if (vec->elements == vec->capacity && !__vec_rsz(vec, vec->capacity * 2))
		return 0;
	for (_i = vec->elements; _i > i; _i--)
		_set_element(vec, _i, __vec_get(vec, _i - 1));
	_set_element(vec, i, val);
	vec->elements++;
	return 1;
}

u8	__vec_ers(vector vec, const size_t i) {
	size_t	_i;

	if (i >= vec->elements)
		return 0;
	if (i != vec->elements - 1) {
		if (vec->free)
			vec->free(index(vec, i));
		for (_i = i; _i < vec->elements - 1; _i++)
			_set_element(vec, _i, __vec_get(vec, _i + 1));
		vec->elements--;
	} else
		__vec_pop(vec);
	return 1;
}

static inline void	_set_element(vector vec, const size_t i, const void *val) {
	memmove(index(vec, i), val, vec->element_size);
}
