// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<discard_line.c>>

#include <stdlib.h>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_term.h"
#include "internal/_vector.h"
#include "internal/_display.h"
#include "internal/_history.h"

extern rl42_hist_node	*current;

rl42_fn(discard_line) {
	rl42_hist_node	*node;
	rl42_hist_node	*prev;

	node = hist_get_first_node();
	current = node;
	for (prev = NULL; node != prev; prev = node, node = hist_get_next_node(node, BACKWARD)) {
		if (node->edit) {
			free((void *)node->edit);
			node->edit = NULL;
		}
	}
	term_cursor_move_to(line, line->root.row, line->root.col + vector_size(line->line));
	term_cursor_get_pos(&line->prompt.root.row, &line->prompt.root.col);
	line->prompt.root.row++;
	line->prompt.root.col = 1;
	term_display_line(line, DISPLAY_PROMPT_ONLY);
	term_cursor_get_pos(&line->root.row, &line->root.col);
	vector_clear(line->line);
	line->i = 0;
	return 1;
}
