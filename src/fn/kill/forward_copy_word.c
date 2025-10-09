// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<forward_copy_word.c>>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_defs.h"
#include "internal/_kill.h"
#include "internal/_utils.h"
#include "internal/_display.h"

rl42_fn(forward_copy_word) {
	u8	rv;

	add_mark(kill_start, line->i);
	if (!move_to_end_of_word(line)) {
		kill_start.set = 0;
		return 1;
	}
	add_mark(kill_end, line->i);
	rv = kill_copy_region(line);
	line->i = kill_start.pos;
	kill_start.set = 0;
	kill_end.set = 0;
	return (rv) ? term_display_line(line, 0) : 0;
}
