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
#include "internal/_display.h"

#ifdef __RL42_USE_INTERNAL_TERMINFO
#include "internal/_terminfo.h"
#else
#include <ti42/ti42.h>
#endif

extern rl42_numeric_arg	n_arg;

rl42_fn(clear_screen) {
	const char	*seq;
	i16			diff_x;
	i16			diff_y;

	if (n_arg.set)
		return term_display_line(line, 0);
	seq = term_get_seq(ti42_clear);
	if (seq != TI42_ABS_STR) {
		diff_x = line->root->row - line->prompt.root->row;
		diff_y = line->root->col - line->prompt.root->col;
		((rl42_cursor_pos *)line->prompt.root)->row = 1;
		((rl42_cursor_pos *)line->prompt.root)->col = 1;
		((rl42_cursor_pos *)line->root)->row = line->prompt.root->row + diff_x;
		((rl42_cursor_pos *)line->root)->col = line->prompt.root->col + diff_y;
		ti42_tputs(seq, 1, term_putchar_unbuffered);
		return term_display_line(line, 0);
	}
	return 1;
}
