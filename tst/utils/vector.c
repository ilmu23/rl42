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

i32	main(void) {
	i32	rv;

	rv = 0;
	if (!_test1())
		rv = 1;
	if (!_test2())
		rv = 1;
	if (!_test3())
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
	vector		vector;
	size_t		i;
	u32			vals[5] = {42, 21, 1, 23, 814};
	i32			chk;
	i32			rv;

	rv = 1;
	info("Test 1 ---- u32 vector\n");
	vector = vector(u32, 5, NULL);
	if (!vector)
		return error("Failed to create a vector of size 5\n");
	for (i = 0; i < 5; i++)
		if (!vector_push(vector, vals[i]))
			return error("Failed to add element #%zu\n", i + 1);
	tmp = vector_get(vector, 0);
	chk = memcmp(tmp, vals, sizeof(vals));
	if (chk != 0)
		rv = 0;
	fprintf(stderr, "%sraw data comparison %s" ENDL, hl(chk == 0), (chk == 0) ? "OK" : "KO");
	for (i = 0; i < 5; i++) {
		tmp = vector_get(vector, 0);
		if (*tmp != vals[i])
			rv = 0;
		fprintf(stderr, "%svector[%zu]: %u" ENDL, hl(*tmp == vals[i]), i, *tmp);
		if (!vector_erase(vector, 0))
			rv = error("Failed to remove element #%zu\n", i);
	}
	vector_delete(vector);
	return rv;
}

static inline i32	_test2(void) {
	const char	**tmp;
	const char	*duped;
	const char	*vals[5] = {"ayy", "lmao", "hello", "there", "hello"};
	const char	*expected[4];
	vector		vector;
	size_t		i;
	i32			chk;
	i32			rv;

	rv = 1;
	info("Test 2 ---- dynamic resizing / element freeing\n");
	vector = vector(char *, 2, free);
	if (!vector)
		return error("Failed to create a vector of size 2\n");
	for (i = 0; i < 5; i++) {
		duped = _strdup(vals[i]);
		if (!duped) {
			error("UNABLE TO ALLOCATE MEMORY");
			exit(1);
		}
		if (i != 2)
			expected[i - ((i > 2) ? 1 : 0)] = duped;
		if (!vector_push(vector, duped))
			return error("Failed to add element #%zu\n", i);
	}
	if (!vector_erase(vector, 2))
		return error("Failed to remove element #2\n");
	tmp = vector_get(vector, 0);
	if (!tmp)
		return error("Failed to get raw data\n");
	chk = memcmp(tmp, expected, sizeof(expected));
	if (chk != 0)
		rv = 0;
	fprintf(stderr, "%sraw data comparison %s" ENDL, hl(chk == 0), (chk == 0) ? "OK" : "KO");
	for (i = 0; i < 4; i++) {
		tmp = vector_get(vector, i);
		if (*tmp != expected[i])
			rv = 0;
		fprintf(stderr, "%svector[%zu]: %s" ENDL, hl(*tmp == expected[i]), i, *tmp);
	}
	vector_delete(vector);
	return rv;
}

static inline i32	_test3(void) {
	const u32	*tmp;
	vector		vector;
	size_t		i;
	u32			vals[5] = {42, 21, 1, 23, 814};
	u32			expected[5] = {814, 21, 23, 1, 42};
	i32			chk;
	i32			rv;

	rv = 1;
	info("Test 3 ---- insertion / out of bounds access / element swapping\n");
	vector = vector(u32, 1, NULL);
	if (!vector)
		return error("Failed to create a vector of size 1\n");
	for (i = 0; i < 5; i++)
		if (!vector_insert(vector, 0, vals[4 - i]))
			return error("Failed to add element #%zu\n", 4 - i + 1);
	if (!vector_swap(vector, 0, 4))
		return error("Failed to swap elements 1 and 5\n");
	if (!vector_swap(vector, 2, 3))
		return error("Failed to swap elements 3 and 4\n");
	if (!vector_swap(vector, 1, 1))
		return error("Failed to swap element 2 with itself\n");
	tmp = vector_get(vector, 0);
	chk = memcmp(tmp, expected, sizeof(vals));
	if (chk != 0)
		rv = 0;
	fprintf(stderr, "%sraw data comparison %s" ENDL, hl(chk == 0), (chk == 0) ? "OK" : "KO");
	for (i = 0; i < 6; i++) {
		tmp = vector_get(vector, i);
		if (tmp == VECTOR_OUT_OF_BOUNDS) {
			fprintf(stderr, "%svector[%zu]: access out of bounds" ENDL, hl(i == 5), i);
			continue ;
		}
		if (*tmp != expected[i])
			rv = 0;
		fprintf(stderr, "%svector[%zu]: %u" ENDL, hl(*tmp == expected[i]), i, *tmp);
	}
	vector_delete(vector);
	return rv;
}
