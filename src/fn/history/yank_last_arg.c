// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<yank_last_arg.c>>

#include "internal/_kb.h"
#include "internal/_defs.h"
#include "internal/_kill.h"
#include "internal/_rl42.h"
#include "internal/_vector.h"
#include "internal/_display.h"
#include "internal/_history.h"

#include "internal/fn/misc.h"

extern rl42_hist_node	*current;

#define _fn_is_valid(fn)	(fn == yank_last_arg || fn == numeric_argument)

rl42_fn(yank_last_arg)	{
	rl42_hist_node	*next;
	rl42_hist_node	*prev;
	rl42_direction	direction;
	rl42_fn_match	match;
	i64				n;
	u8				rv;

	if (!current)
		return 1;
	if (n_arg.set) {
		n = (!n_arg.neg) ? n_arg.val + 1 : -n_arg.val - 1;
		vector_delete(line->prompt.sprompt);
		line->prompt.sprompt = NULL;
		n_arg.set = 0;
	} else
		n = INT64_MAX;
	direction = BACKWARD;
	next = hist_get_next_node(current, direction);
	if (current == next)
		return 1;
	rv = 0;
	add_mark(kill_start, line->i);
	if (!hist_yank_arg(line, next, n) || !term_display_line(line, 0))
		goto _yank_last_arg_ret;
	add_mark(kill_end, line->i);
	rv = 1;
	match.fn = NULL;
	vector_clear(line->keyseq);
	state_flags |= STATE_KILL_DONT_UPDATE_RING;
	do {
		match = kb_match_seq(line, match.fn, kb_listen((match.fn && match.fn->f) ? AMBIGUOUS_TIMEOUT : -1));
		if (match.run) {
			if (match.fn->f == yank_last_arg) {
				prev = next;
				if (n_arg.set) {
					if (n_arg.neg)
						direction ^= BACKWARD;
					vector_delete(line->prompt.sprompt);
					line->prompt.sprompt = NULL;
					n_arg.set = 0;
				}
				next = hist_get_next_node(next, direction);
				if (next == prev)
					continue ;
				rv &= kill_region_internal(line);
				line->i -= kill_end.pos - kill_start.pos;
				rv &= hist_yank_arg(line, next, n);
				rv &= term_display_line(line, 0);
				add_mark(kill_end, line->i);
			} else if (match.fn->f == numeric_argument) {
				rv = numeric_argument(line);
			} else
				break ;
			vector_clear(line->keyseq);
			prev_fn = match.fn->f;
			match.fn = NULL;
		}
	} while (rv == 1);
_yank_last_arg_ret:
	state_flags &= ~STATE_KILL_DONT_UPDATE_RING;
	kill_start.set = 0;
	kill_end.set = 0;
	return (match.fn) ? match.fn->f(line) : rv;
}
