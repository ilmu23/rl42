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


static const char	*histfile_name;

static size_t	first_new;
static size_t	entries;
static list		history;

static void	_free_hist_node(rl42_hist_node *node);

rl42_hist_node	*hist_get_next_node(const rl42_hist_node *node, const rl42_direction direction) {
	list_node	lnode;

	switch (direction) {
		case FORWARD:
			lnode = (node->entry_n != list_size(history)) ? list_nth(history, entries - (node->entry_n + 1)) : NULL;
			break ;
		case BACKWARD:
			lnode = (node->entry_n != 0) ? list_nth(history, entries - (node->entry_n - 1)) : NULL;
			break ;
		default:
			lnode = NULL;
	}
	return (lnode) ? (rl42_hist_node *)lnode->data : (rl42_hist_node *)node;
}

rl42_hist_node	*hist_get_nth_node(i64 n) {
	rl42_hist_node	*out;
	rl42_direction	direction;
	i64				i;
	u8				neg;

	if (n < 0) {
		neg = 1;
		n = -n;
	} else
		neg = 0;
	if (n < (i64)entries / 2) {
		i = 1;
		direction = (!neg) ? BACKWARD : FORWARD;
		out = (!neg) ? hist_get_first_node() : hist_get_last_node();
		while (i++ < n)
			out = hist_get_next_node(out, direction);
	} else {
		i = (i64)entries;
		direction = (!neg) ? FORWARD : BACKWARD;
		out = (!neg) ? hist_get_last_node() : hist_get_first_node();
		while (i-- > n)
			out = hist_get_next_node(out, direction);
	}
	return out;
}

rl42_hist_node	*hist_get_first_node(void) {
	return (rl42_hist_node *)list_first(history)->data;
}

rl42_hist_node	*hist_get_last_node(void) {
	return (rl42_hist_node *)list_last(history)->data;
}

void	hist_remove_node(rl42_hist_node *node) {
	list_erase(history, list_nth(history, entries-- - node->entry_n));
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
	} else
		snprintf(buf, 4096, "%s", fname);
	file = fopen(buf, "r");
	if (!file)
		return 0;
	histfile_name = strdup(buf);
	for (rv = 1, line = fgets(buf, 4096, file); rv && line; line = fgets(buf, 4096, file))
		if (!hist_add_line(strndup(line, strlen(line) - 1)))
			rv = 0;
	fclose(file);
	first_new = entries;
	return rv;
}

void	hist_clean(void) {
	rl42_hist_node	*node;
	FILE			*file;

	if (first_new != entries) {
		file = fopen(histfile_name, "a");
		for (node = hist_get_first_node(); node->entry_n > first_new; node = hist_get_next_node(node, BACKWARD))
			;
		do {
			node = hist_get_next_node(node, FORWARD);
			fprintf(file, "%s\n", node->line);
		} while (node->entry_n != entries);
		fclose(file);
	}
	free((void *)histfile_name);
	list_delete(history);
}

static void	_free_hist_node(rl42_hist_node *node) {
	free((void *)node->line);
	free((void *)node->edit);
}
