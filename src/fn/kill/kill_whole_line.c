// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<kill_whole_line.c>>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_kill.h"
#include "internal/_rl42.h"
#include "internal/_vector.h"
#include "internal/_display.h"

rl42_fn(kill_whole_line) {
	add_mark(kill_start, 0);
	add_mark(kill_end, vector_size(line->line));
	if (!kill_region(line))
		return 0;
	kill_start.set = 0;
	kill_end.set = 0;
	line->i = 0;
	return term_display_line(line, 0);
}
