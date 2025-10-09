// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<kill_region.c>>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_defs.h"
#include "internal/_kill.h"
#include "internal/_rl42.h"
#include "internal/_display.h"

rl42_fn(kill_region) {
	if (!user.set)
		return 1;
	if (user.pos < line->i) {
		add_mark(kill_start, user.pos);
		add_mark(kill_end, line->i);
	} else {
		add_mark(kill_start, line->i);
		add_mark(kill_end, user.pos);
	}
	if (!kill_region_internal(line))
		return 0;
	kill_start.set = 0;
	kill_start.pos = 0;
	if (user.pos < line->i)
		line->i = user.pos;
	return term_display_line(line, 0);
}
