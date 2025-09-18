// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<list.c>>

#include <time.h>
#include <stdio.h>
#include <string.h>

#include "internal/_list.h"
#include "internal/_utils.h"
#include "internal/test/defs.h"

#ifndef REMOVE_CHANCE
# define REMOVE_CHANCE 40
#endif

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
	list_node	node;
	size_t		i;
	list		list;
	u32			vals[5] = {42, 21, 1, 23, 814};
	u32			expected[4] = {42, 21, 23, 814};
	u32			val;
	u8			rv;

	rv = 1;
	info("Test 1 ---- Basic list\n");
	list = list(u32, 5, NULL);
	if (!list)
		return error("Failed to create a list of size 5\n");
	for (i = 0; i < 5; i++)
		if (!list_push_back(list, vals[i]))
			return error("Failed to add element #%zu\n", i + 1);
	list_erase(list, list_nth(list, 2));
	node = list_first(list);
	for (i = 0; i < 4 && node; i++, node = list_next(list, node)) {
		val = *(u32 *)node->data;
		if (val != expected[i])
			rv = 0;
		fprintf(stderr, "%sNode #%zu: %u" ENDL, hl(val == expected[i]), i + 1, val);
	}
	list_delete(list);
	return rv;
}

static inline u8	_test2(void) {
	const char	*vals[4] = {"lmao", "ayy", "there", "hello"};
	const char	*expected[4];
	const char	*val;
	const char	*dup;
	list_node	node;
	size_t		i;
	list		list;
	u8			rv;

	rv = 1;
	info("Test 2 ---- Front pushing / Element freeing\n");
	list = list(char *, 2, free);
	if (!list)
		return error("Failed to create a list of size 2\n");
	for (i = 0; i < 4; i++) {
		dup = _strdup(vals[i]);
		if (!dup)
			exit(error("UNABLE TO ALLOCATE MEMORY"));
		expected[3 - i] = dup;
		if (!list_push_front(list, dup))
			return error("Failed to add element #%zu\n", i + 1);
	}
	node = list_first(list);
	for (i = 0; i < 4; i++, node = list_next(list, node)) {
		val = *(const char **)node->data;
		if (val != expected[i])
			rv = 0;
		fprintf(stderr, "%sNode #%zu: %s" ENDL, hl(val == expected[i]), i + 1, val);
	}
	list_delete(list);
	return rv;
}

static inline u8	_test3(void) {
	list_node	node;
	list_node	prev;
	vector		vals;
	size_t		i;
	size_t		removed;
	list		list;
	u32			val;
	u32			chk;
	u8			rv;

	rv = 1;
	vals = vector(u32, 50, NULL);
	if (!vals)
		exit(error("UNABLE TO ALLOCATE MEMORY"));
	info("Test 3 ---- Randomized list\n");
	list = list(u32, 5, NULL);
	if (!list)
		return error("Failed to create a list of size 5\n");
	for (i = 0; i < 50; i++) {
		val = rand_range(1, 4200);
		if (!vector_push(vals, val) || !list_push_back(list, val))
			return error("Failed to add element #%zu\n", i + 1);
	}
	for (i = removed = 0, node = list_first(list); node; i++) {
		prev = node;
		node = list_next(list, node);
		if (rand_range(1, 100) < REMOVE_CHANCE) {
			vector_erase(vals, i - removed++);
			list_erase(list, prev);
		}
	}
	node = list_first(list);
	for (i = 0, node = list_first(list); node; i++) {
		val = *(u32 *)node->data;
		chk = *(u32 *)vector_get(vals, i);
		if (val != chk)
			rv = 0;
		fprintf(stderr, "%sNode #%.2zu: %u" ENDL, hl(val == chk), i + 1, val);
		node = list_next(list, node);
	}
	vector_delete(vals);
	list_delete(list);
	return rv;
}

static inline u8	_test4(void) {
	list_node	start;
	list_node	mid;
	list_node	end;
	list_node	node;
	size_t		i;
	list		list;
	u32			vals[7] = {42, 81, 2, 3, 91, 0, 8219};
	u32			val;
	u8			rv;

	rv = 1;
	info("Test 4 --- Insertion / Random resize\n");
	list = list(u32, 7, NULL);
	if (!list)
		return error("Failed to create list of size 3\n");
	if (!list_push_back(list, vals[3]))
		return error("Failed to add element #4\n");
	mid = list_first(list);
	if (!list_insert_before(list, mid, vals[0]))
		return error("Failed to add element #1\n");
	if (!list_insert_after(list, mid, vals[6]))
		return error("Failed to add element #7\n");
	start = list_first(list);
	end = list_last(list);
	if (!list_insert_after(list, start, vals[1]))
		return error("Failed to add element #2\n");
	if (!list_insert_before(list, mid, vals[2]))
		return error("Failed to add element #3\n");
	if (!list_insert_after(list, mid, vals[4]))
		return error("Failed to add element #5\n");
	if (!list_insert_before(list, end, vals[5]))
		return error("Failed to add element #6\n");
	if (!list_resize(list, rand_range(1, 10)))
		return error("Failed to resize list\n");
	for (i = 0, node = list_first(list); node; node = list_next(list, node), i++) {
		val = *(u32 *)node->data;
		if (val != vals[i])
			rv = 0;
		fprintf(stderr, "%sNode #%zu: %u" ENDL, hl(val == vals[i]), i + 1, val);
	}
	list_delete(list);
	return rv;
}
