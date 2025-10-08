// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<kill_line.c>>

#include "internal/_kill.h"
#include "internal/_rl42.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"

#include "internal/fn/kill.h"

rl42_fn(kill_line) {
	if (get_numeric_arg(line) < 0)
		return backward_kill_line(line);
	add_mark(kill_start, line->i);
	add_mark(kill_end, vector_size(line->line));
	if (!kill_region_internal(line))
		return 0;
	kill_start.set = 0;
	kill_end.set = 0;
	return term_display_line(line, 0);
}
