// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<darray.c>>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal/_utils.h"
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
	const u32	*tmp;
	darray		darray;
	size_t		i;
	u32			vals[5] = {42, 21, 1, 23, 814};
	i32			chk;
	i32			rv;

	rv = 1;
	info("Test 1 ---- u32 darray\n");
	darray = darray(u32, 5, NULL);
	if (!darray)
		return error("Failed to create a darray of size 5\n");
	for (i = 0; i < 5; i++)
		if (!darray_push(darray, vals[i]))
			return error("Failed to add element #%zu\n", i + 1);
	tmp = darray_get(darray, 0);
	chk = memcmp(tmp, vals, sizeof(vals));
	if (chk != 0)
		rv = 0;
	fprintf(stderr, "%sraw data comparison %s" ENDL, hl(chk == 0), (chk == 0) ? "OK" : "KO");
	for (i = 0; i < 5; i++) {
		tmp = darray_get(darray, 0);
		if (*tmp != vals[i])
			rv = 0;
		fprintf(stderr, "%sdarray[%zu]: %u" ENDL, hl(*tmp == vals[i]), i, *tmp);
		if (!darray_erase(darray, 0))
			rv = error("Failed to remove element #%zu\n", i);
	}
	darray_delete(darray);
	return rv;
}

static inline i32	_test2(void) {
	const char	**tmp;
	const char	*duped;
	const char	*vals[5] = {"ayy", "lmao", "hello", "there", "hello"};
	const char	*expected[4];
	darray		darray;
	size_t		i;
	i32			chk;
	i32			rv;

	rv = 1;
	info("Test 2 ---- dynamic resizing / element freeing\n");
	darray = darray(char *, 2, free);
	if (!darray)
		return error("Failed to create a darray of size 2\n");
	for (i = 0; i < 5; i++) {
		duped = _strdup(vals[i]);
		if (!duped) {
			error("UNABLE TO ALLOCATE MEMORY");
			exit(1);
		}
		if (i != 2)
			expected[i - ((i > 2) ? 1 : 0)] = duped;
		if (!darray_push(darray, duped))
			return error("Failed to add element #%zu\n", i);
	}
	if (!darray_erase(darray, 2))
		return error("Failed to remove element #2\n");
	tmp = darray_get(darray, 0);
	if (!tmp)
		return error("Failed to get raw data\n");
	chk = memcmp(tmp, expected, sizeof(expected));
	if (chk != 0)
		rv = 0;
	fprintf(stderr, "%sraw data comparison %s" ENDL, hl(chk == 0), (chk == 0) ? "OK" : "KO");
	for (i = 0; i < 4; i++) {
		tmp = darray_get(darray, i);
		if (*tmp != expected[i])
			rv = 0;
		fprintf(stderr, "%sdarray[%zu]: %s" ENDL, hl(*tmp == expected[i]), i, *tmp);
	}
	darray_delete(darray);
	return rv;
}

static inline i32	_test3(void) {
	const u32	*tmp;
	darray		darray;
	size_t		i;
	u32			vals[5] = {42, 21, 1, 23, 814};
	u32			expected[5] = {814, 21, 23, 1, 42};
	i32			chk;
	i32			rv;

	rv = 1;
	info("Test 3 ---- insertion / out of bounds access / element swapping\n");
	darray = darray(u32, 1, NULL);
	if (!darray)
		return error("Failed to create a darray of size 1\n");
	for (i = 0; i < 5; i++)
		if (!darray_insert(darray, 0, vals[4 - i]))
			return error("Failed to add element #%zu\n", 4 - i + 1);
	if (!darray_swap(darray, 0, 4, NULL))
		return error("Failed to swap elements 1 and 5\n");
	if (!darray_swap(darray, 2, 3, NULL))
		return error("Failed to swap elements 3 and 4\n");
	if (!darray_swap(darray, 1, 1, NULL))
		return error("Failed to swap element 2 with itself\n");
	tmp = darray_get(darray, 0);
	chk = memcmp(tmp, expected, sizeof(vals));
	if (chk != 0)
		rv = 0;
	fprintf(stderr, "%sraw data comparison %s" ENDL, hl(chk == 0), (chk == 0) ? "OK" : "KO");
	for (i = 0; i < 6; i++) {
		tmp = darray_get(darray, i);
		if (tmp == DARRAY_OUT_OF_BOUNDS) {
			fprintf(stderr, "%sdarray[%zu]: access out of bounds" ENDL, hl(i == 5), i);
			continue ;
		}
		if (*tmp != expected[i])
			rv = 0;
		fprintf(stderr, "%sdarray[%zu]: %u" ENDL, hl(*tmp == expected[i]), i, *tmp);
	}
	darray_delete(darray);
	return rv;
}

static inline i32	_test4(void) {
	const u32	*tmp;
	darray		darray;
	size_t		i;
	u32			vals[10] = {42, 21, 1, 23, 814, 92, 0, 2912, 1, 2198};
	u32			expected[8] = {42, 21, 1, 23, 0, 2912, 1, 2198};
	i32			chk;
	i32			rv;

	rv = 1;
	info("Test 4 ---- insert_n / erase_n\n");
	darray = darray(u32, 1, NULL);
	if (!darray)
		return error("Failed to create a darray of size 1\n");
	if (!darray_insert_n(darray, 0, 5, &vals[5]))
		return error("Failed to insert elements 6 - 10\n");
	if (!darray_insert_n(darray, 0, 5, vals))
		return error("Failed to insert elements 1 - 5\n");
	if (!darray_erase_n(darray, 4, 2))
		return error("Failed to remove 2 elements starting from index 4\n");
	tmp = darray_get(darray, 0);
	chk = (darray_size(darray) == 8) ? memcmp(tmp, expected, sizeof(expected)) : 1;
	if (chk != 0)
		rv = 0;
	fprintf(stderr, "%sraw data comparison %s" ENDL, hl(chk == 0), (chk == 0) ? "OK" : "KO");
	for (i = 0; i < 10; i++) {
		tmp = darray_get(darray, i);
		if (tmp == DARRAY_OUT_OF_BOUNDS) {
			fprintf(stderr, "%sdarray[%zu]: access out of bounds" ENDL, hl(i >= 8), i);
			continue ;
		}
		if (i >= 8 || *tmp != expected[i])
			rv = 0;
		fprintf(stderr, "%sdarray[%zu]: %u" ENDL, hl(*tmp == expected[i]), i, *tmp);
	}
	darray_delete(darray);
	return rv;
}
