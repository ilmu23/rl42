// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<copy_word.c>>

#include <ctype.h>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_defs.h"
#include "internal/_kill.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"

rl42_fn(copy_word) {
	size_t	i;
	u8		rv;

	if (line->i == vector_size(line->line) || isspace(*(u32 *)vector_get(line->line, line->i)))
		return 1;
	i = line->i;
	move_to_start_of_word(line);
	add_mark(kill_start, line->i);
	if (!move_to_end_of_word(line)) {
		kill_start.set = 0;
		return 1;
	}
	add_mark(kill_end, line->i);
	rv = kill_copy_region(line);
	kill_start.set = 0;
	kill_end.set = 0;
	line->i = i;
	return (rv) ? term_display_line(line, 0) : 0;
}
