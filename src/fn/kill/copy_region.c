// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<copy_region.c>>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_defs.h"
#include "internal/_kill.h"
#include "internal/_rl42.h"
#include "internal/_display.h"

rl42_fn(copy_region) {
	u8	rv;

	if (!user.set)
		return 1;
	if (user.pos < line->i) {
		add_mark(kill_start, user.pos);
		add_mark(kill_end, line->i);
	} else {
		add_mark(kill_start, line->i);
		add_mark(kill_end, user.pos);
	}
	rv = kill_copy_region(line);
	kill_start.set = 0;
	kill_end.pos = 0;
	return (rv) ? term_display_line(line, 0) : 0;
}
