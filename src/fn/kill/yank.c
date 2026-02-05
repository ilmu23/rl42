// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<yank.c>>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_defs.h"
#include "internal/_kill.h"
#include "internal/_darray.h"
#include "internal/_display.h"

rl42_fn(yank) {
	cdarray	text;
	size_t	len;

	text = kill_get_top_of_ring();
	if (!text)
		return 1;
	add_mark(kill_start, line->i);
	len = darray_size(text);
	if (!darray_insert_n(line->line, line->i, len, darray_start(text)))
		return 0;
	line->i += len;
	add_mark(kill_end, line->i);
	return term_display_line(line, 0);
}
