// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<darray.c>>

#include <alloca.h>
#include <stdlib.h>
#include <string.h>

#include "internal/_utils.h"
#include "internal/_darray.h"

struct __darray {
	void	(*free)(void *);
	size_t	element_size;
	size_t	capacity;
	size_t	elements;
	u8		*data;
};

#define index(arr, i)	((void *)((uintptr_t)arr->data + (arr->element_size * (i))))

static inline void	_set_element(darray arr, const size_t i, const void *val);

static void	__free(void **blk) {
	free(*blk);
}

darray	__dar_new(const size_t size, const size_t count, void (*_free)(void *)) {
	darray	out;

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

darray	__dar_cpy(cdarray arr, const size_t start, const size_t end, void *(*cpy)(void *)) {
	darray	out;

	out = malloc(sizeof(*out));
	if (out) {
		out->capacity = (end - start < arr->capacity) ? end - start : arr->capacity;
		out->data = malloc(arr->element_size * ((out->capacity) ? out->capacity : 1));
		if (!out->data) {
			free(out);
			return NULL;
		}
		out->elements = (cpy) ? 0 : (arr->elements > out->capacity) ? out->capacity : arr->elements;
		out->element_size = arr->element_size;
		out->free = arr->free;
		if (cpy) while (out->elements < out->capacity) {
			_set_element(out, out->elements, cpy(index(arr, start + out->elements)));
			out->elements++;
		} else
			memcpy(out->data, index(arr, start), out->element_size * out->elements);
		if (out->capacity == 0)
			out->capacity = 1;
	}
	return out;
}

void	__dar_del(darray arr) {
	if (arr) {
		if (arr->elements)
			__dar_clr(arr);
		free(arr->data);
		free(arr);
	}
}

u8	__dar_psh(darray arr, const void *val) {
	if (arr->elements == arr->capacity && !__dar_rsz(arr, arr->capacity * 2))
		return 0;
	_set_element(arr, arr->elements++, val);
	return 1;
}

void	__dar_pop(darray arr) {
	if (arr->elements) {
		arr->elements--;
		if (arr->free)
			arr->free(index(arr, arr->elements));
	}
}

void	*__dar_get(cdarray arr, const size_t i) {
	return (i < arr->elements) ? index(arr, i) : (i == (size_t)-1 && arr->elements) ? index(arr, arr->elements -1) : VECTOR_OUT_OF_BOUNDS;
}

u8	__dar_set(darray arr, const size_t i, const void *val, const u8 free) {
	if (i >= arr->elements)
		return 0;
	if (free && arr->free)
		arr->free(index(arr, i));
	_set_element(arr, i, val);
	return 1;
}

u8	__dar_swp(darray arr, const size_t i1, const size_t i2, u8 (*swap)(void *, void *)) {
	u8	*tmp;

	if (i1 >= arr->elements || i2 >= arr->elements)
		return 0;
	if (i1 != i2) {
		if (swap)
			return swap(index(arr, i1), index(arr, i2));
		tmp = alloca(arr->element_size);
		memcpy(tmp, index(arr, i1), arr->element_size);
		memcpy(index(arr, i1), index(arr, i2), arr->element_size);
		memcpy(index(arr, i2), tmp, arr->element_size);
	}
	return 1;
}

size_t	__dar_sze(cdarray arr) {
	return arr->elements;
}

size_t	__dar_cap(cdarray arr) {
	return arr->capacity;
}

u8	__dar_rsz(darray arr, const size_t size) {
	if (size < arr->elements && arr->free) {
		do
			arr->free(index(arr, --arr->elements));
		while (size < arr->elements);
	}
	arr->capacity = size;
	arr->data = realloc(arr->data, arr->capacity * arr->element_size);
	return (arr->data) ? 1 : 0;
}

u8	__dar_stf(darray arr) {
	return __dar_rsz(arr, arr->elements);
}

void	__dar_clr(darray arr) {
	if (arr->free && arr->elements) {
		do
			arr->free(index(arr, --arr->elements));
		while (arr->elements);
	}
	arr->elements = 0;
}

u8	__dar_ins(darray arr, const size_t i, const size_t n, const void *vals) {
	size_t	_i;

	if (n == 0)
		return 1;
	if (arr->elements + n > arr->capacity) {
		do
			arr->capacity *= 2;
		while (arr->elements + n > arr->capacity);
		if (!__dar_rsz(arr, arr->capacity))
			return 0;
	}
	_i = min(i, arr->elements);
	if (_i != arr->elements)
		memmove(index(arr, _i + n), index(arr, _i), (arr->elements - _i) * arr->element_size);
	memcpy(index(arr, _i), vals, n * arr->element_size);
	arr->elements += n;
	return 1;
}

u8	__dar_ers(darray arr, const size_t i, const size_t n) {
	size_t	_n;

	if (n == 0)
		return 1;
	if (i >= arr->elements)
		return 0;
	if (i != arr->elements - 1) {
		if (arr->free) for (_n = 0; _n < n && i + _n < arr->elements; _n++)
			arr->free(index(arr, i));
		else
			_n = min(n, arr->elements - i);
		memmove(index(arr, i), index(arr, i + _n), (arr->elements - i - _n) * arr->element_size);
		arr->elements -= _n;
	} else
		__dar_pop(arr);
	return 1;
}

static inline void	_set_element(darray arr, const size_t i, const void *val) {
	memmove(index(arr, i), val, arr->element_size);
}
