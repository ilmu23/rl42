// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<backward_kill_word.c>>

#include "internal/_defs.h"
#include "internal/_kill.h"
#include "internal/_rl42.h"
#include "internal/_utils.h"
#include "internal/_display.h"

#include "internal/fn/kill.h"

rl42_fn(backward_kill_word) {
	u8	rv;

	rv = 1;
	if (NEED_REPEAT) {
		if (!repeat(line, backward_kill_word, kill_word))
			return 0;
	} else {
		add_mark(kill_end, line->i);
		if (!move_to_start_of_word(line)) {
			kill_end.set = 0;
			return 2;
		}
		add_mark(kill_start, line->i);
		rv = kill_region_internal(line);
	}
	kill_start.set = 0;
	kill_end.set = 0;
	return (rv) ? term_display_line(line, 0) : 0;
}
