// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_darray.h>>

#pragma once

#include <stddef.h>

#include "defs.h"

typedef struct __darray *		darray;
typedef const struct __darray *	cdarray;

#define DARRAY_OUT_OF_BOUNDS	((void *)1)

#define darray(type, count, free)	(__dar_new(sizeof(type), count, free))
darray	__dar_new(const size_t size, const size_t count, void (*free)(void *));

#define darray_copy(darray, cpy)					(__dar_cpy(darray, 0, SIZE_MAX, cpy))
#define darray_copy_range(darray, start, end, cpy)	(__dar_cpy(darray, start, end, cpy))
darray	__dar_cpy(cdarray arr, const size_t start, const size_t end, void *(*cpy)(void *));

#define	darray_delete(darray)	(__dar_del(darray))
void	__dar_del(darray);

#define	darray_push(darray, value)	(__dar_psh(darray, (const void *)&value))
u8		__dar_psh(darray arr, const void *val);

#define	darray_pop(darray)	(__dar_pop(darray))
void	__dar_pop(darray arr);

#define darray_first(darray)	(__dar_get(darray, 0))
#define darray_start(darray)	(darray_first(darray))
#define darray_last(darray)		(__dar_get(darray, (size_t)-1))
#define darray_end(darray)		(darray_last(darray))
#define	darray_get(darray, i)	(__dar_get(darray, i))
void	*__dar_get(cdarray arr, const size_t i);

#define	darray_set(darray, i, value)		(__dar_set(darray, i, (const void *)&value, 0))
#define darray_replace(darray, i, value)	(__dar_set(darray, i, (const void *)&value, 1))
u8		__dar_set(darray arr, const size_t i, const void *val, const u8 free);

#define darray_swap(darray, i1, i2, swapper)	(__dar_swp(darray, i1, i2, swapper))
u8		__dar_swp(darray arr, const size_t i1, const size_t i2, u8 (*swap)(void *, void *));

#define	darray_size(darray)	(__dar_sze(darray))
size_t	__dar_sze(cdarray arr);

#define	darray_capacity(darray)	(__dar_cap(darray))
size_t	__dar_cap(cdarray arr);

#define darray_resize(darray, size)	(__dar_rsz(darray, size))
u8		__dar_rsz(darray arr, const size_t size);

#define darray_shrink_to_fit(darray)	(__dar_stf(darray))
u8		__dar_stf(darray arr);

#define darray_clear(darray)	(__dar_clr(darray))
void	__dar_clr(darray arr);

#define darray_insert(darray, i, value)				(__dar_ins(darray, i, 1, (const void *)&value))
#define darray_insert_n(darray, start, n, values)	(__dar_ins(darray, start, n, (const void *)values))
u8		__dar_ins(darray arr, const size_t i, const size_t n, const void *vals);

#define darray_erase(darray, i)				(__dar_ers(darray, i, 1))
#define darray_erase_n(darray, start, n)	(__dar_ers(darray, start, n))
u8		__dar_ers(darray arr, const size_t i, const size_t n);
