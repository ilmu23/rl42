// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<forward_word.c>>

#include <ctype.h>

#include "internal/_rl42.h"
#include "internal/_term.h"
#include "internal/_vector.h"

#include "internal/fn/move.h"

rl42_fn(forward_word) {
	size_t	len;

	if (NEED_REPEAT) {
		if (!repeat(line, forward_word, backward_word))
			return 0;
	} else {
		len = vector_size(line->line);
		if (line->i == len)
			return 2;
		if (isspace(*(u32 *)vector_get(line->line, line->i))) do
			line->i++;
		while (line->i < len && isspace(*(u32 *)vector_get(line->line, line->i)));
		while (line->i < len && !isspace(*(u32 *)vector_get(line->line, line->i)))
			line->i++;
	}
	return (~state_flags & STATE_REPEAT) ? term_cursor_move_to_i(line) : 1;
}
