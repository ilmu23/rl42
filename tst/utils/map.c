// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<map.c>>

#include <time.h>
#include <stdio.h>
#include <string.h>

#include "internal/_map.h"
#include "internal/test/defs.h"

#define _REMOVE_CHANCE	40

#define rand_range(min, max)	(rand() % (max - min + 1) + min)

static inline char	*_strdup(const char *s);
static inline u8	_test1(void);
static inline u8	_test2(void);
static inline u8	_test3(void);

i32	main(void) {
	struct timespec	ts;
	i32				rv;

	rv = 0;
	timespec_get(&ts, TIME_UTC);
	srand(ts.tv_nsec);
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

static inline u8	_test1(void) {
	size_t	i;
	map		*map;
	i32		*element;
	i32		keys[5] = {1, 2, 3, 4, 5};
	i32		vals[5] = {42, 21, 1, 23, 814};
	i32		rv;

	rv = 1;
	fprintf(stderr, SGR_INFO "Test 1 ----" ENDL);
	map = map_new(42);
	if (!map) {
		fprintf(stderr, SGR_KO "Failed to create a map of size 42" ENDL);
		return 0;
	}
	for (i = 0; i < 5; i++) {
		if (!map_add(map, keys[i], &vals[i])) {
			fprintf(stderr, SGR_KO "Failed to add element with key %d" ENDL, keys[i]);
			return 0;
		}
	}
	for (i = 0; i < 5; i++) {
		element = map_find(map, keys[i]);
		if (element != &vals[i])
			rv = 0;
		fprintf(stderr, "%smap_find(%d): ", hl(element == &vals[i]), keys[i]);
		if (element == MAP_NOT_FOUND)
			fprintf(stderr, "NOT FOUND" ENDL);
		else
			fprintf(stderr, "%p (%d)" ENDL, (void *)element, *element);
		if (!map_remove(map, keys[i], NULL)) {
			fprintf(stderr, SGR_KO "Failed to remove element corresponding to key %d" ENDL, keys[i]);
			rv = 0;
		}
	}
	if (!map_delete(map, NULL)) {
		fprintf(stderr, SGR_KO "Failed to delete map" ENDL);
		rv = 0;
	}
	return rv;
}

static inline u8	_test2(void) {
	size_t	i;
	char	*vals[5] = {"ayy", "lmao", "hello", "there", "hello"};
	char	*element;
	map		*map;
	i32		keys[5] = {42, 21, 1, 23, 814};
	i32		rv;

	rv = 1;
	fprintf(stderr, SGR_INFO "Test 2 ----" ENDL);
	map = map_new(2);
	if (!map) {
		fprintf(stderr, SGR_KO "failed to create a map of size 2" ENDL);
		return 0;
	}
	for (i = 0; i < 5; i++) {
		element = _strdup(vals[i]);
		if (!element) {
			fprintf(stderr, SGR_KO "UNABLE TO ALLOCATE MEMORY" ENDL);
			exit(1);
		}
		if (!map_add(map, keys[i], element)) {
			fprintf(stderr, SGR_KO "Failed to add element with key %d" ENDL, keys[i]);
			return 0;
		}
	}
	if (!map_remove(map, keys[2], free)) {
		fprintf(stderr, SGR_KO "failed to remove element corresponding to key %d" ENDL, keys[i]);
		rv = 0;
	}
	for (i = 0; i < 5; i++) {
		element = map_find(map, keys[i]);
		if (i == 2) {
			if (element != MAP_NOT_FOUND)
				rv = 0;
		} else if (strcmp(element, vals[i]) != 0)
			rv = 0;
		fprintf(stderr, "%smap_find(%d): ",
				hl((i == 2 && element == MAP_NOT_FOUND) || strcmp(element, vals[i]) == 0), keys[i]);
		if (element == MAP_NOT_FOUND)
			fprintf(stderr, "NOT FOUND" ENDL);
		else
			fprintf(stderr, "%s" ENDL, element);
	}
	if (!map_delete(map, free)) {
		fprintf(stderr, SGR_KO "Failed to delete map" ENDL);
		rv = 0;
	}
	return rv;
}

static inline u8	_test3(void) {
	size_t	i;
	map		*map;
	u64		*element;
	u64		removed;
	u64		vals[42];
	i32		keys[42];
	i32		rv;

	rv = 1;
	fprintf(stderr, SGR_INFO "Test 3 ----" ENDL);
	i = rand_range(2, 517121);
	map = map_new(i);
	if (!map) {
		fprintf(stderr, SGR_KO "Failed to create a map of size %zu" ENDL, i);
		return 0;
	}
	for (i = 0; i < 42; i++) {
		keys[i] = rand_range(1, 0x10FFFF);
		vals[i] = rand_range(1, UINT64_MAX);
	}
	for (i = 0; i < 42; i++) {
		if (!map_add(map, keys[i], &vals[i])) {
			fprintf(stderr, SGR_KO "Failed to add element with key %d" ENDL, keys[i]);
			return 0;
		}
	}
	for (i = removed = 0; i < 42; i++) {
		if (rand_range(1, 100) < _REMOVE_CHANCE) {
			if (!map_remove(map, keys[i], NULL)) {
				fprintf(stderr, SGR_KO "Failed to remove element corresponding to key %d" ENDL, keys[i]);
				rv = 0;
			} else
				removed |= 1L << i;
		}
	}
	for (i = 0; i < 42; i++) {
		element = map_find(map, keys[i]);
		if (removed & 1L << i) {
			if (element != MAP_NOT_FOUND)
				rv = 0;
		} else if (element != &vals[i])
			rv = 0;
		fprintf(stderr, "%smap_find(%d): ",
				hl((removed & 1L << i && element == MAP_NOT_FOUND) || element == &vals[i]), keys[i]);
		if (element == MAP_NOT_FOUND)
			fprintf(stderr, "NOT FOUND" ENDL);
		else
			fprintf(stderr, "%p (%#lX)" ENDL, (void *)element, *element);
		if (!(removed & 1L << i) && !map_remove(map, keys[i], NULL)) {
			fprintf(stderr, SGR_KO "Failed to remove element corresponding to key %d" ENDL, keys[i]);
			rv = 0;
		}
	}
	if (!map_delete(map, NULL)) {
		fprintf(stderr, SGR_KO "Failed to delete map" ENDL);
		rv = 0;
	}
	return rv;
}
