// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<history_test.c>>

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "internal/_rl42.h"
#include "internal/_utils.h"
#include "internal/_history.h"
#include "internal/test/defs.h"

#define _HIST_ITEMS_COUNT	7

#define next(node)	(hist_get_next_node(node, FORWARD))
#define prev(node)	(hist_get_next_node(node, BACKWARD))

static inline u8	_create_history(void);

static const char	*hist_items[_HIST_ITEMS_COUNT] = {
	"ayy",
	"lmao",
	"hello",
	"there",
	"hello",
	"cat",
	"8519jlaöahww"
};

i32	main(void) {
	rl42_hist_node	*node;
	size_t			i;
	i32				rv;

	if (!_create_history())
		return 1;
	rl42_init();
	info("Test 1 ---- Start -> End\n");
	for (i = rv = 0, node = hist_get_last_node(); i < _HIST_ITEMS_COUNT; node = next(node), i++) {
		if (strcmp(node->line, hist_items[i]) != 0)
			rv = 1;
		fprintf(stderr, "%sEntry %zu: %s (expected %s)" ENDL, hl(strcmp(node->line, hist_items[i]) == 0),
				node->entry_n, node->line, hist_items[i]);
	}
	info("Test 2 ---- End -> Start\n");
	for (i = _HIST_ITEMS_COUNT, node = hist_get_first_node(); i--; node = prev(node)) {
		if (strcmp(node->line, hist_items[i]) != 0)
			rv = 1;
		fprintf(stderr, "%sEntry %zu: %s (expected %s)" ENDL, hl(strcmp(node->line, hist_items[i]) == 0),
				node->entry_n, node->line, hist_items[i]);
	}
	unlink(".hist");
	return rv;
}

static inline u8	_create_history(void) {
	size_t	i;
	FILE	*file;

	file = fopen(".hist", "w");
	if (!file)
		return 0;
	for (i = 0; i < _HIST_ITEMS_COUNT; i++)
		fprintf(file, "%s\n", hist_items[i]);
	setenv("RL42_HISTORY", ".hist", 1);
	fclose(file);
	return 1;
}
