// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<vector.c>>

#include "internal/_vector.h"

static enum {
	SORT_STAGE_1,
	SORT_STAGE_2
}	sort_stage;

static inline u8	_ascending_sort(const uintptr_t *e1, const uintptr_t *e2);
static inline u8	_descending_sort(const uintptr_t *e1, const uintptr_t *e2);

vector	*vector_new(const size_t size, const vector_sort_mode smode) {
	vector	*out;

	if (!size)
		return NULL;
	out = malloc(sizeof(*out));
	if (out) {
		*out = (vector) {
			.sort_mode = smode,
			.data = malloc(size * sizeof(*out->data)),
			.capacity = size,
			.size = 0
		};
		if (!out->data) {
			free(out);
			out = NULL;
		}
	}
	return out;
}

u8	vector_add(vector *vec, const uintptr_t data) {
	uintptr_t	*new_arr;
	size_t		i;

	if (vec->size == vec->capacity) {
		new_arr = malloc(vec->capacity * 2 * sizeof(*new_arr));
		if (!new_arr)
			return 0;
		memcpy(new_arr, vec->data, vec->capacity * sizeof(*new_arr));
		free(vec->data);
		vec->data = new_arr;
		vec->capacity *= 2;
	}
	switch (vec->sort_mode) {
		case ASCENDING:
//			_lower_bound_ascending(NULL, NULL);
			sort_stage = SORT_STAGE_1;
			i = vector_find(vec, data, _ascending_sort);
			break ;
		case DESCENDING:
//			_lower_bound_descending(NULL, NULL);
			sort_stage = SORT_STAGE_1;
			i = vector_find(vec, data, _descending_sort);
			break ;
		case NONE:
			i = vec->size;
	}
	if (i == VECTOR_NOT_FOUND)
		i = (sort_stage == SORT_STAGE_1) ? 0 : vec->size;
	if (i != vec->size)
		memmove(&vec->data[i + 1], &vec->data[i], (vec->size - i) * sizeof(*vec->data));
	vec->data[i] = data;
	vec->size++;
	return 1;
}

u8	vector_remove(vector *vec, const size_t i, void (*_free)(void *)) {
	if (i >= vec->size)
		return 0;
	if (_free)
		_free((void *)vec->data[i]);
	if (i < vec->size - 1)
		memmove(&vec->data[i], &vec->data[i + 1], (vec->size - i - 1) * sizeof(*vec->data));
	vec->size--;
	return 1;
}

size_t	vector_find(const vector *vec, const uintptr_t data, u8 (*cmp)(const uintptr_t *, const uintptr_t *)) {
	size_t	search_width;
	size_t	i;

	if (cmp) {
		for (i = 0; i < vec->size; i++)
			if (cmp(&data, &vec->data[i]))
				break ;
	} else {
		switch (vec->sort_mode) {
			case ASCENDING:
				for (i = search_width = vec->size / 2; data != vec->data[i];) {
					search_width /= 2;
					if (data < vec->data[i])
						i += search_width;
					else
						i -= search_width;
				}
				break ;
			case DESCENDING:
				for (i = search_width = vec->size / 2; data != vec->data[i];) {
					search_width /= 2;
					if (data < vec->data[i])
						i -= search_width;
					else
						i += search_width;
				}
				break ;
			case NONE:
				for (i = 0; i < vec->size; i++)
					if (data == vec->data[i])
						break ;
		}
	}
	return (i < vec->size) ? i : VECTOR_NOT_FOUND;
}

uintptr_t	vector_get(const vector *vec, const size_t i) {
	return (i < vec->size) ? vec->data[i] : VECTOR_OUT_OF_BOUNDS;
}

u8	vector_delete(vector *vec, void (*_free)(void *)) {
	size_t	i;

	if (_free)
		for (i = 0; i < vec->size; i++)
			_free((void *)vec->data[i]);
	free(vec->data);
	free(vec);
	return 1;
}

static inline u8	_ascending_sort(const uintptr_t *e1, const uintptr_t *e2) {

	u8					rv;

	switch (sort_stage) {
		case SORT_STAGE_1:
			if (*e1 > *e2)
				sort_stage = SORT_STAGE_2;
			return 0;
		case SORT_STAGE_2:
			rv = *e1 < *e2;
			if (rv)
				sort_stage = SORT_STAGE_1;
			return rv;
	}
	return 0;
}

static inline u8	_descending_sort(const uintptr_t *e1, const uintptr_t *e2) {
	u8					rv;

	switch (sort_stage) {
		case SORT_STAGE_1:
			if (*e1 < *e2)
				sort_stage = SORT_STAGE_2;
			return 0;
		case SORT_STAGE_2:
			rv = *e1 > *e2;
			if (rv)
				sort_stage = SORT_STAGE_1;
			return rv;
	}
	return 0;
}
