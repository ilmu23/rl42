// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<revert_line.c>>

#include <stdlib.h>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_utils.h"
#include "internal/_darray.h"
#include "internal/_display.h"

extern rl42_hist_node	*current;

rl42_fn(revert_line) {
	if (current->edit) {
		free((void *)current->edit);
		current->edit = NULL;
	}
	darray_delete(line->line);
	line->line = cstr_to_rl42str(current->line);
	if (!line->line)
		return 0;
	line->i = darray_size(line->line);
	return term_display_line(line, 0);
}
