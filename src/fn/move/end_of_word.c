// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<end_of_word.c>>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_defs.h"
#include "internal/_term.h"
#include "internal/_utils.h"

rl42_fn(end_of_word) {
	if (line->i > 0 && line->i != darray_size(line->line) && is_space(line_at(line, line->i)) && !is_space(line_at(line, line->i - 1)))
		return 1;
	move_to_end_of_word(line);
	return term_cursor_move_to_i(line);
}
