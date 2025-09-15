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
#include <stdlib.h>
#include <string.h>

#include "internal/_map.h"
#include "internal/test/defs.h"

#define _REMOVE_CHANCE	40

static inline char	*_strdup(const char *s);

static inline u8	_test1(void);
static inline u8	_test2(void);
static inline u8	_test3(void);
static inline u8	_test4(void);

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

static inline u8	_test1(void) {
	size_t	i;
	map		map;
	i32		*element;
	i32		keys[5] = {1, 2, 3, 4, 5};
	i32		vals[5] = {42, 21, 1, 23, 814};
	i32		rv;

	rv = 1;
	fprintf(stderr, SGR_INFO "Test 1 ---- i32 map" ENDL);
	map = map(i32, 42, INTEGER, NULL);
	if (!map) {
		fprintf(stderr, SGR_KO "Failed to create a map of size 42" ENDL);
		return 0;
	}
	for (i = 0; i < 5; i++) {
		if (!map_set(map, keys[i], vals[i])) {
			fprintf(stderr, SGR_KO "Failed to add element with key %d" ENDL, keys[i]);
			return 0;
		}
	}
	for (i = 0; i < 5; i++) {
		element = map_get(map, keys[i]);
		if (element == MAP_NOT_FOUND || *element != vals[i]) {
			fputs(SGR_KO, stderr);
			rv = 0;
		} else
			fputs(SGR_OK, stderr);
		fprintf(stderr, "map_find(%d): ", keys[i]);
		if (element == MAP_NOT_FOUND)
			fputs("NOT FOUND" ENDL, stderr);
		else
			fprintf(stderr, "%d" ENDL, *element);
		if (!map_erase(map, keys[i])) {
			fprintf(stderr, SGR_KO "Failed to remove element corresponding to key %d" ENDL, keys[i]);
			rv = 0;
		}
	}
	map_delete(map);
	return rv;
}

static inline u8	_test2(void) {
	size_t	i;
	char	*vals[5] = {"ayy", "lmao", "hello", "there", "hello"};
	char	**element;
	char	*tmp;
	map		map;
	i32		keys[5] = {42, 21, 1, 23, 814};
	i32		rv;

	rv = 1;
	fprintf(stderr, SGR_INFO "Test 2 ---- string map with integer keys / growing map" ENDL);
	map = map(char *, 2, INTEGER, free);
	if (!map) {
		fprintf(stderr, SGR_KO "failed to create a map of size 2" ENDL);
		return 0;
	}
	for (i = 0; i < 5; i++) {
		tmp = _strdup(vals[i]);
		if (!tmp) {
			fprintf(stderr, SGR_KO "UNABLE TO ALLOCATE MEMORY" ENDL);
			exit(1);
		}
		if (!map_set(map, keys[i], tmp)) {
			fprintf(stderr, SGR_KO "Failed to add element with key %d" ENDL, keys[i]);
			return 0;
		}
	}
	if (!map_erase(map, keys[2])) {
		fprintf(stderr, SGR_KO "failed to remove element corresponding to key %d" ENDL, keys[2]);
		rv = 0;
	}
	for (i = 0; i < 5; i++) {
		element = map_get(map, keys[i]);
		if (i == 2) {
			if (element != MAP_NOT_FOUND)
				rv = 0;
		} else if (element == MAP_NOT_FOUND || strcmp(*element, vals[i]) != 0)
			rv = 0;
		fprintf(stderr, "%smap_find(%d): ", hl((i == 2 && element == MAP_NOT_FOUND)
				|| (element != MAP_NOT_FOUND && strcmp(*element, vals[i]) == 0)), keys[i]);
		if (element == MAP_NOT_FOUND)
			fputs("NOT FOUND" ENDL, stderr);
		else
			fprintf(stderr, "%s" ENDL, *element);
	}
	map_delete(map);
	return rv;
}

static inline u8	_test3(void) {
	size_t	i;
	map		map;
	u64		*element;
	u64		removed;
	u64		vals[42];
	i32		keys[42];
	i32		rv;

	rv = 1;
	fprintf(stderr, SGR_INFO "Test 3 ---- randomized u64 map" ENDL);
	i = rand_range(2, 517121);
	map = map(u64, i, INTEGER, NULL);
	if (!map) {
		fprintf(stderr, SGR_KO "Failed to create a map of size %zu" ENDL, i);
		return 0;
	}
	for (i = 0; i < 42; i++) {
		keys[i] = rand_range(1, 0x10FFFF);
		vals[i] = rand_range(1, UINT64_MAX);
	}
	for (i = 0; i < 42; i++) {
		if (!map_set(map, keys[i], vals[i])) {
			fprintf(stderr, SGR_KO "Failed to add element with key %d" ENDL, keys[i]);
			return 0;
		}
	}
	for (i = removed = 0; i < 42; i++) {
		if (rand_range(1, 100) < _REMOVE_CHANCE) {
			if (!map_erase(map, keys[i])) {
				fprintf(stderr, SGR_KO "Failed to remove element corresponding to key %d" ENDL, keys[i]);
				rv = 0;
			} else
				removed |= 1L << i;
		}
	}
	for (i = 0; i < 42; i++) {
		element = map_get(map, keys[i]);
		if (removed & 1L << i) {
			if (element != MAP_NOT_FOUND) {
				fputs(SGR_KO, stderr);
				rv = 0;
			} else
				fputs(SGR_OK, stderr);
		} else {
			if (element == MAP_NOT_FOUND || *element != vals[i]) {
				fputs(SGR_KO, stderr);
				rv = 0;
			} else
				fputs(SGR_OK, stderr);
		}
		fprintf(stderr, "map_find(%d): ", keys[i]);
		if (element == MAP_NOT_FOUND)
			fputs("NOT FOUND" ENDL, stderr);
		else
			fprintf(stderr, "%lu" ENDL, *element);
	}
	map_delete(map);
	return rv;
}

static inline u8	_test4(void) {
	size_t	i;
	char	*vals[5] = {"this", "is", "a", "value", "dwahkdwhajd"};
	char	*keys[5] = {"this", "is", "a", "key", "sadhjlqawhrfjlahgwrjgr"};
	char	**element;
	char	*tmp;
	map		map;
	i32		rv;

	rv = 1;
	fprintf(stderr, SGR_INFO "Test 2 ---- string map with string keys" ENDL);
	map = map(char *, 12, INTEGER, free);
	if (!map) {
		fprintf(stderr, SGR_KO "failed to create a map of size 12" ENDL);
		return 0;
	}
	for (i = 0; i < 5; i++) {
		tmp = _strdup(vals[i]);
		if (!tmp) {
			fprintf(stderr, SGR_KO "UNABLE TO ALLOCATE MEMORY" ENDL);
			exit(1);
		}
		if (!map_set(map, keys[i], tmp)) {
			fprintf(stderr, SGR_KO "Failed to add element with key \"%s\"" ENDL, keys[i]);
			return 0;
		}
	}
	for (i = 0; i < 5; i++) {
		element = map_get(map, keys[i]);
		if (element == MAP_NOT_FOUND || strcmp(*element, vals[i]) != 0)
			rv = 0;
		fprintf(stderr, "%smap_find(\"%s\"): ", hl(element != MAP_NOT_FOUND && strcmp(*element, vals[i]) == 0), keys[i]);
		if (element == MAP_NOT_FOUND)
			fputs("NOT FOUND" ENDL, stderr);
		else
			fprintf(stderr, "%s" ENDL, *element);
	}
	map_delete(map);
	return rv;
}
