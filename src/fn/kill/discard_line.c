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
extern rl42_mark		user;

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
	line->i = SIZE_MAX;
	term_cursor_move_to(line, line->root->row + 1, line->root->col + calculate_cursor_offset(line) + ((line->prompt.sprompt) ? (__vec_sze(line->prompt.sprompt)) + 1 : 0));
	term_cursor_get_pos((i16 *)&line->prompt.root->row, (i16 *)&line->prompt.root->col);
	((rl42_cursor_pos *)line->prompt.root)->col = 1;
	term_display_line(line, DISPLAY_PROMPT_ONLY);
	term_cursor_get_pos((i16 *)&line->root->row, (i16 *)&line->root->col);
	vector_clear(line->line);
	user.set = 0;
	line->i = 0;
	return 1;
}
