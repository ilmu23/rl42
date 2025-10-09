// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<delete_horizontal_space.c>>

#include <ctype.h>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_defs.h"
#include "internal/_kill.h"
#include "internal/_rl42.h"
#include "internal/_vector.h"
#include "internal/_display.h"

rl42_fn(delete_horizontal_space) {
	size_t	len;
	u8		rv;

	len = vector_size(line->line);
	if (line->i != len && !isspace(*(u32 *)vector_get(line->line, line->i)))
		return 1;
	if (line->i > 0 && isspace(*(u32 *)vector_get(line->line, line->i - 1))) do
		line->i--;
	while (line->i > 0 && isspace(*(u32 *)vector_get(line->line, line->i - 1)));
	add_mark(kill_start, line->i);
	while (line->i < len && isspace(*(u32 *)vector_get(line->line, line->i)))
		line->i++;
	add_mark(kill_end, line->i);
	state_flags |= STATE_KILL_DONT_UPDATE_RING;
	rv = kill_region_internal(line);
	state_flags &= ~STATE_KILL_DONT_UPDATE_RING;
	line->i = kill_start.pos;
	kill_start.set = 0;
	kill_end.set = 0;
	return (rv) ? term_display_line(line, 0) : 0;
}
