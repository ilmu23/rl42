// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<backward_word.c>>

#include <ctype.h>

#include "internal/_rl42.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_vector.h"

#include "internal/fn/move.h"

rl42_fn(backward_word) {
	if (NEED_REPEAT) {
		if (!repeat(line, backward_word, forward_word))
			return 0;
	} else {
		if (line->i == 0)
			return 2;
		if (isspace(*(u32 *)vector_get(line->line, line->i - 1))) do
			line->i--;
		while (line->i > 0 && isspace(*(u32 *)vector_get(line->line, line->i)));
		while (line->i > 0 && !isspace(*(u32 *)vector_get(line->line, line->i - 1)))
			line->i--;
	}
	return (~state_flags & STATE_REPEAT) ? term_cursor_move_to_i(line) : 1;
}
