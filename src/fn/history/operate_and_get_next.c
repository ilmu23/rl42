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
#include "internal/_utils.h"
#include "internal/_display.h"
#include "internal/_history.h"
#include "internal/_terminfo.h"

extern rl42_hist_node	*current;

rl42_fn(operate_and_get_next) {
	current = hist_get_next_node(current, FORWARD);
	state_flags |= STATE_SAVE_HIST_POSITION;
	term_display_line(line, 0);
	ti_tputs("\n", 1, __putchar);
	return 0;
}
