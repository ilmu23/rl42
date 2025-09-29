// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<backward_word.c>>

#include <ctype.h>

#include "function.h"

#include "internal/_term.h"
#include "internal/_vector.h"

rl42_fn(backward_word) {
	if (line->i == 0)
		return 1;
	if (isspace(*(u32 *)vector_get(line->line, line->i - 1))) do
		line->i--;
	while (line->i > 0 && isspace(*(u32 *)vector_get(line->line, line->i)));
	while (line->i > 0 && !isspace(*(u32 *)vector_get(line->line, line->i - 1)))
		line->i--;
	return term_cursor_move_to_i(line);
}
