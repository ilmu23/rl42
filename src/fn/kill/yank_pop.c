// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<yank_pop.c>>

#include "internal/_kb.h"
#include "internal/_kill.h"
#include "internal/_rl42.h"
#include "internal/_vector.h"
#include "internal/_display.h"

#include "internal/fn/kill.h"

rl42_fn(yank_pop) {
	rl42_fn_match	match;
	u8				rv;

	if (prev_fn != yank)
		return 1;
	rv = 0;
	state_flags |= STATE_KILL_DONT_UPDATE_RING;
	if (!kill_rotate_ring() || !kill_region_internal(line))
		goto _yank_pop_ret;
	line->i -= kill_end.pos - kill_start.pos;
	rv = yank(line);
	if (!rv || !term_display_line(line, 0))
		goto _yank_pop_ret;
	match.fn = NULL;
	vector_clear(line->keyseq);
	do {
		match = kb_match_seq(line, match.fn, kb_listen((match.fn && match.fn->f) ? AMBIGUOUS_TIMEOUT : -1));
		if (match.run) {
			if (match.fn->f == yank_pop) {
				rv &= kill_rotate_ring();
				rv &= kill_region_internal(line);
				line->i -= kill_end.pos - kill_start.pos;
				rv &= yank(line);
				rv &= term_display_line(line, 0);
			} else
				break ;
			vector_clear(line->keyseq);
			prev_fn = match.fn->f;
			match.fn = NULL;
		}
	} while (rv);
_yank_pop_ret:
	state_flags &= ~STATE_KILL_DONT_UPDATE_RING;
	kill_start.set = 0;
	kill_end.set = 0;
	return (match.fn) ? match.fn->f(line) : rv;
}
