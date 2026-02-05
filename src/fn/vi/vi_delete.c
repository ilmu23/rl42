// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<vi_delete.c>>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_kb.h"
#include "internal/_darray.h"

#include "internal/fn/kill.h"
#include "internal/fn/misc.h"
#include "internal/fn/move.h"
#include "internal/fn/text.h"

rl42_fn(vi_delete) {
	rl42_fn_match	match;
	u8				rv;

	rv = 1;
	match.fn = NULL;
	darray_clear(line->keyseq);
	do {
		match = kb_match_seq(line, match.fn, kb_listen((match.fn && match.fn->f) ? AMBIGUOUS_TIMEOUT : -1));
		if (match.fn && match.run) {
			if (match.fn->f != numeric_argument) {
				if (match.fn->f == beginning_of_line)
					rv = backward_kill_line(line);
				else if (match.fn->f == end_of_line)
					rv = kill_line(line);
				else if (match.fn->f == backward_word)
					rv = backward_kill_word(line);
				else if (match.fn->f == forward_word)
					rv = kill_word(line);
				else if (match.fn->f == backward_char)
					rv = backward_delete_char(line);
				else if (match.fn->f == forward_char)
					rv = delete_char(line);
				break ;
			}
			rv = match.fn->f(line);
			darray_clear(line->keyseq);
		}
	} while (rv);
	return rv;
}
