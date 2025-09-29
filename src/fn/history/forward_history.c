// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<forward_history.c>>

#include <stdlib.h>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"
#include "internal/_history.h"

extern rl42_hist_node	*current;

rl42_fn(forward_history) {
	rl42_hist_node	*next;

	next = hist_get_next_node(current, FORWARD);
	if (next == current)
		return 1;
	if (current->edit)
		free((void *)current->edit);
	current->edit = rl42str_to_cstr(line->line);
	if (!current->edit)
		return 0;
	current = next;
	vector_delete(line->line);
	line->line = cstr_to_rl42str((current->edit) ? current->edit : current->line);
	if (!line)
		return 0;
	line->i = vector_size(line->line);
	return term_display_line(line, 0);
}
