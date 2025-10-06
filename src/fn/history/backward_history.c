// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<backward_history.c>>

#include <stdlib.h>

#include "internal/_rl42.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"
#include "internal/_history.h"

#include "internal/fn/history.h"

extern rl42_hist_node	*current;
static rl42_hist_node	*start;

rl42_fn(backward_history) {
	rl42_hist_node	*next;

	if (NEED_REPEAT) {
		start = current;
		if (!repeat(line, backward_history, forward_history))
			return 0;
	} else {
		next = hist_get_next_node(current, BACKWARD);
		if (next == current)
			return 2;
		if (state_flags & STATE_REPEAT && start != current) {
			current = next;
			return 1;
		}
		if (current->edit)
			free((void *)current->edit);
		current->edit = rl42str_to_cstr(line->line);
		if (!current->edit)
			return 0;
		current = next;
		if (state_flags & STATE_REPEAT)
			return 1;
	}
	vector_delete(line->line);
	line->line = cstr_to_rl42str((current->edit) ? current->edit : current->line);
	if (!line->line)
		return 0;
	line->i = vector_size(line->line);
	return term_display_line(line, 0);
}
