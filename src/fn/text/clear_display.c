// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<clear_display.c>>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_term.h"
#include "internal/_display.h"

#ifndef __RL42_USE_EXTERNAL_TERMINFO
#include "internal/_terminfo.h"
#else
#include <ti42/ti42.h>
#endif

#define _CLEAR_SCROLLBACK_BUFFER	"\x1b[3J"

rl42_fn(clear_display) {
	const char	*seq;
	i16			row_diff;
	i16			col_diff;

	seq = term_get_seq(ti_clear);
	if (seq != TI_ABS_STR) {
		row_diff = line->root->row - line->prompt.root->row;
		col_diff = line->root->col - line->prompt.root->col;
		((rl42_cursor_pos *)line->prompt.root)->row = 1;
		((rl42_cursor_pos *)line->prompt.root)->col = 1;
		((rl42_cursor_pos *)line->root)->row = line->prompt.root->row + row_diff;
		((rl42_cursor_pos *)line->root)->col = line->prompt.root->col + col_diff;
		ti_tputs(seq, 1, term_putchar_unbuffered);
		ti_tputs(_CLEAR_SCROLLBACK_BUFFER, 1, term_putchar_unbuffered);
		term_display_line(line, 0);
	}
	return 1;
}
