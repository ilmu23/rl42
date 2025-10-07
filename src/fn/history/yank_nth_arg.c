// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<yank_nth_arg.c>>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_vector.h"
#include "internal/_display.h"
#include "internal/_history.h"

extern rl42_numeric_arg	n_arg;
extern rl42_hist_node	*current;

rl42_fn(yank_nth_arg) {
	i64	n;

	if (!current)
		return 1;
	if (n_arg.set) {
		n = (!n_arg.neg) ? n_arg.val + 1 : -n_arg.val - 1;
		vector_delete(line->prompt.sprompt);
		line->prompt.sprompt = NULL;
		n_arg.set = 0;
	} else
		n = 1;
	if (!hist_yank_arg(line, hist_get_next_node(current, BACKWARD), n))
		return 0;
	return term_display_line(line, 0);
}
