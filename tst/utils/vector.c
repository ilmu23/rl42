// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<vector.c>>

#include <stdio.h>
#include <string.h>

#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/test/defs.h"

static inline char	*_strdup(const char *s);

static inline i32	_test1(void);
static inline i32	_test2(void);
static inline i32	_test3(void);
static inline i32	_test4(void);

i32	main(void) {
	i32	rv;

	rv = 0;
	if (!_test1())
		rv = 1;
	if (!_test2())
		rv = 1;
	if (!_test3())
		rv = 1;
	if (!_test4())
		rv = 1;
	return rv;
}

static inline char	*_strdup(const char *s) {
	size_t	len;
	char	*out;

	len = strlen(s);
	out = malloc((len + 1) * sizeof(*out));
	if (out)
		memcpy(out, s, len + 1);
	return out;
}

static inline i32	_test1(void) {
	uintptr_t	element;
	vector		*vector;
	size_t		i;
	u32			vals[5] = {42, 21, 1, 23, 814};
	i32			rv;

	rv = 1;
	info("Test 1 ---- unsorted vector\n");
	vector = vector_new(5, NONE);
	if (!vector)
		return error("Failed to create a vector of size 5\n");
	for (i = 0; i < 5; i++)
		if (!vector_add(vector, vals[i]))
			return error("Failed to add element #%zu\n", i + 1);
	for (i = 0; i < 5; i++) {
		element = vector_get(vector, 0);
		if (element != vals[i])
			rv = 0;
		fprintf(stderr, "%svector[%zu]: %lu" ENDL, hl(element == vals[i]), i, element);
		if (!vector_remove(vector, 0, NULL)) {
			error("Failed to remove element #%zu\n", i);
			rv = 0;
		}
	}
	if (!vector_delete(vector, NULL))
		return error("Failed to delete vector\n");
	return rv;
}

static inline i32	_test2(void) {
	uintptr_t	element;
	vector		*vector;
	size_t		i;
	u32			vals[5] = {42, 21, 1, 23, 814};
	u32			expected[5] = {1, 21, 23, 42, 814};
	i32			rv;

	rv = 1;
	info("Test 2 ---- ascending sorted vector\n");
	vector = vector_new(5, ASCENDING);
	if (!vector)
		return error("Failed to create a vector of size 5\n");
	for (i = 0; i < 5; i++)
		if (!vector_add(vector, vals[i]))
			return error("Failed to add element #%zu\n", i + 1);
	for (i = 0; i < 5; i++) {
		element = vector_get(vector, 0);
		if (element != expected[i])
			rv = 0;
		fprintf(stderr, "%svector[%zu]: %lu" ENDL, hl(element == expected[i]), i, element);
		if (!vector_remove(vector, 0, NULL)) {
			error("Failed to remove element #%zu\n", i);
			rv = 0;
		}
	}
	if (!vector_delete(vector, NULL))
		return error("Failed to delete vector\n");
	return rv;
}

static inline i32	_test3(void) {
	uintptr_t	element;
	vector		*vector;
	size_t		i;
	u32			vals[5] = {42, 21, 1, 23, 814};
	u32			expected[5] = {814, 42, 23, 21, 1};
	i32			rv;

	rv = 1;
	info("Test 3 ---- descending sorted vector\n");
	vector = vector_new(5, DESCENDING);
	if (!vector)
		return error("Failed to create a vector of size 5\n");
	for (i = 0; i < 5; i++)
		if (!vector_add(vector, vals[i]))
			return error("Failed to add element #%zu\n", i + 1);
	for (i = 0; i < 5; i++) {
		element = vector_get(vector, 0);
		if (element != expected[i])
			rv = 0;
		fprintf(stderr, "%svector[%zu]: %lu" ENDL, hl(element == expected[i]), i, element);
		if (!vector_remove(vector, 0, NULL)) {
			error("Failed to remove element #%zu\n", i);
			rv = 0;
		}
	}
	if (!vector_delete(vector, NULL))
		return error("Failed to delete vector\n");
	return rv;
}

static inline i32	_test4(void) {
	uintptr_t	element;
	vector		*vector;
	size_t		i;
	const char	*vals[5] = {"ayy", "lmao", "hello", "there", "hello"};
	const char	*expected[4];
	i32			rv;

	rv = 1;
	info("Test 4 ---- element freeing\n");
	vector = vector_new(2, NONE);
	if (!vector)
		return error("Failed to create a vector of size 2\n");
	for (i = 0; i < 5; i++) {
		element = (uintptr_t)_strdup(vals[i]);
		if (!element) {
			error("UNABLE TO ALLOCATE MEMORY");
			exit(1);
		}
		if (i != 2)
			expected[i - ((i > 2) ? 1 : 0)] = (const char *)element;
		if (!vector_add(vector, element))
			return error("Failed to add element #%zu\n", i);
	}
	if (!vector_remove(vector, 2, free))
		return error("Failed to remove element #2\n");
	for (i = 0; i < vector->size; i++) {
		element = vector_get(vector, i);
		if (element != (uintptr_t)expected[i])
			rv = 0;
		fprintf(stderr, "%svector[%zu]: %s" ENDL, hl(element == (uintptr_t)expected[i]), i, (char *)element);
	}
	if (!vector_delete(vector, free))
		return error("Failed to delete vector\n");
	return rv;
}
