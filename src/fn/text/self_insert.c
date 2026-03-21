// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<self_insert.c>>

#include "internal/_rl42.h"
#include "internal/_utils.h"
#include "internal/_display.h"

#undef darray_insert
#ifdef __RL42_USE_INTERNAL_CONTAINERS
#define darray_insert(arr, i, val)	(__dar_ins(arr, i, 1, val))
#else
#define darray_insert(arr, i, val)	(_dar_ins(arr, i, 1, val))
#endif

u8	self_insert(rl42_line *line) {
	if (NEED_REPEAT) {
		if (!repeat(line, self_insert, NULL))
			return 0;
	} else {
		if (!darray_insert(line->line, line->i, darray_get(line->keyseq, 0)))
			return 0;
		line->i++;
	}
	return (~state_flags & STATE_REPEAT) ? term_display_line(line, 0) : 1;
}
