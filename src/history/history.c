// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<history.c>>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "internal/_list.h"
#include "internal/_history.h"

#define _DEFAULT_HIST_FILE	".rl42_history"

#define hist_node(n, l, e)	((rl42_hist_node){.entry_n = n, .line = l, .edit = e})

size_t	first_new;
size_t	entries;
list	history;

static void	_free_hist_node(rl42_hist_node *node);

rl42_hist_node	*hist_get_next_node(const rl42_hist_node *node, const rl42_direction direction) {
	list_node	lnode;

	switch (direction) {
		case FORWARD:
			lnode = list_nth(history, entries - (node->entry_n + 1));
			break ;
		case BACKWARD:
			lnode = list_nth(history, entries - (node->entry_n - 1));
			break ;
		default:
			lnode = NULL;
	}
	return (lnode) ? (rl42_hist_node *)lnode->data : (rl42_hist_node *)node;
}

rl42_hist_node	*hist_get_first_node(void) {
	return (rl42_hist_node *)list_first(history)->data;
}

rl42_hist_node	*hist_get_last_node(void) {
	return (rl42_hist_node *)list_last(history)->data;
}

u8	hist_add_line(const char *line) {
	return list_push_front(history, hist_node(++entries, line, NULL));
}

u8	hist_load(const char *fname) {
	FILE	*file;
	char	*line;
	char	buf[4096];
	u8		rv;

	if (history)
		return 1;
	history = list(rl42_hist_node, 500, (void (*)(void *))_free_hist_node);
	if (!history)
		return 0;
	if (!fname) {
		snprintf(buf, 4096, "%s/" _DEFAULT_HIST_FILE, getenv("HOME"));
		file = fopen(buf, "r");
	} else
		file = fopen(fname, "r");
	if (!file)
		return 0;
	for (rv = 1, line = fgets(buf, 4096, file); rv && line; line = fgets(buf, 4096, file))
		if (!hist_add_line(strndup(line, strlen(line) - 1)))
			rv = 0;
	fclose(file);
	first_new = entries;
	return rv;
}

static void	_free_hist_node(rl42_hist_node *node) {
	free((void *)node->line);
	free((void *)node->edit);
}
