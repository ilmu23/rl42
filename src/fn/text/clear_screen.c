// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<clear_screen.c>>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_display.h"
#include "internal/_terminfo.h"

extern rl42_numeric_arg	n_arg;

rl42_fn(clear_screen) {
	const char	*seq;
	i16			diff_x;
	i16			diff_y;

	if (n_arg.set)
		return term_display_line(line, 0);
	seq = term_get_seq(ti_clear);
	if (seq != TI_ABS_STR) {
		diff_x = line->root.row - line->prompt.root.row;
		diff_y = line->root.col - line->prompt.root.col;
		line->prompt.root.row = 1;
		line->prompt.root.col = 1;
		line->root.row = line->prompt.root.row + diff_x;
		line->root.col = line->prompt.root.col + diff_y;
		ti_tputs(seq, 1, __putchar);
		return term_display_line(line, 0);
	}
	return 1;
}
