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
#include "internal/_vector.h"
#include "internal/_display.h"

#undef vector_insert
#define vector_insert(vec, i, val)	(__vec_ins(vec, i, val))

u8	self_insert(rl42_line *line) {
	if (NEED_REPEAT) {
		if (!repeat(line, self_insert, NULL))
			return 0;
	} else {
		if (!vector_insert(line->line, line->i, vector_get(line->keyseq, 0)))
			return 0;
		line->i++;
	}
	return (~state_flags & STATE_REPEAT) ? term_display_line(line, 0) : 1;
}
