// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<operate_and_get_next.c>>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_rl42.h"
#include "internal/_display.h"
#include "internal/_history.h"

extern rl42_numeric_arg	n_arg;
extern rl42_hist_node	*current;

rl42_fn(operate_and_get_next) {
	current = (!n_arg.set) ? hist_get_next_node(current, FORWARD) : hist_get_nth_node(n_arg.val * ((n_arg.neg) ? -1 : 1));
	state_flags |= STATE_SAVE_HIST_POSITION;
	term_display_line(line, 0);
	return 0;
}
