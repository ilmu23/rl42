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

#include "internal/_kill.h"
#include "internal/_rl42.h"
#include "internal/_vector.h"
#include "internal/_display.h"

rl42_fn(yank) {
	cvector	text;
	size_t	len;
	size_t	i;

	text = kill_get_top_of_ring();
	if (!text)
		return 1;
	add_mark(kill_start, line->i);
	for (i = 0, len = vector_size(text); i < len; i++)
		if (!__vec_ins(line->line, line->i++, vector_get(text, i)))
			return 0;
	add_mark(kill_end, line->i);
	return term_display_line(line, 0);
}
